#include "Terminal.h"
#include <memory>

// === 单例实现 ===

Terminal& Terminal::getInstance() {
    static Terminal instance;
    return instance;
}

void Terminal::init() {
    // 确保实例已创建
    getInstance();
}

Terminal::Terminal() {
    // 获取控制台句柄
    hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 保存原始控制台状态
    GetConsoleScreenBufferInfo(hConsoleOutput, &originalCSBI);
    GetConsoleMode(hConsoleInput, &originalConsoleMode);
    originalAttributes = originalCSBI.wAttributes;
    
    // 设置UTF-8输出编码
    SetConsoleOutputCP(CP_UTF8);
    
    // 启用虚拟终端处理 (如果支持)
    DWORD mode;
    if (GetConsoleMode(hConsoleOutput, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsoleOutput, mode);
    }
    
    // 设置无缓冲输入
    DWORD inputMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(hConsoleInput, inputMode);
}

Terminal::~Terminal() {
    restoreOriginalState();
}

void Terminal::restoreOriginalState() {
    // 恢复原始控制台模式
    SetConsoleMode(hConsoleInput, originalConsoleMode);
    SetConsoleMode(hConsoleOutput, originalConsoleMode);
    
    // 恢复原始属性
    SetConsoleTextAttribute(hConsoleOutput, originalAttributes);
    
    // 恢复原始光标位置
    SetConsoleCursorPosition(hConsoleOutput, originalCSBI.dwCursorPosition);
}

// === 输入处理 ===

char Terminal::getKey() {
    // 使用Windows API的非阻塞输入
    DWORD numberOfEvents = 0;
    INPUT_RECORD inputRecord;
    
    // 检查输入事件
    if (!GetNumberOfConsoleInputEvents(hConsoleInput, &numberOfEvents)) {
        return 0;
    }
    
    if (numberOfEvents > 0) {
        // 读取事件但不移除
        PeekConsoleInput(hConsoleInput, &inputRecord, 1, &numberOfEvents);
        
        if (inputRecord.EventType == KEY_EVENT && 
            inputRecord.Event.KeyEvent.bKeyDown) {
            
            // 读取并丢弃事件
            ReadConsoleInput(hConsoleInput, &inputRecord, 1, &numberOfEvents);
            return inputRecord.Event.KeyEvent.uChar.AsciiChar;
        }
    }
    return 0;
}

std::string Terminal::getLine() {
    // 使用标准输入获取完整行
    const int bufferSize = 256;
    wchar_t wbuffer[bufferSize];
    DWORD charsRead;
    
    // 读取宽字符输入
    if (ReadConsoleW(hConsoleInput, wbuffer, bufferSize - 1, &charsRead, NULL)) {
        // 转换为UTF-8
        wbuffer[charsRead] = L'\0';
        int size = WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, NULL, 0, NULL, NULL);
        if (size > 0) {
            std::string buffer(size, 0);
            WideCharToMultiByte(CP_UTF8, 0, wbuffer, -1, &buffer[0], size, NULL, NULL);
            // 移除结尾的换行符
            if (!buffer.empty() && buffer.back() == '\n') {
                buffer.pop_back();
            }
            if (!buffer.empty() && buffer.back() == '\r') {
                buffer.pop_back();
            }
            return buffer;
        }
    }
    return "";
}

// === 输出控制 ===

void Terminal::clearScreen() {
    COORD topLeft = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD written;
    
    // 获取控制台大小
    GetConsoleScreenBufferInfo(hConsoleOutput, &csbi);
    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    
    // 填充空格
    FillConsoleOutputCharacterW(hConsoleOutput, L' ', consoleSize, topLeft, &written);
    FillConsoleOutputAttribute(hConsoleOutput, originalAttributes, consoleSize, topLeft, &written);
    
    // 复位光标
    SetConsoleCursorPosition(hConsoleOutput, topLeft);
}

void Terminal::setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(hConsoleOutput, coord);
}

void Terminal::print(const std::string& text) {
    // 转换为宽字符
    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
    if (wchars_num <= 0) return;
    
    
    wchar_t* wstr = new wchar_t[wchars_num];
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, wstr, wchars_num);
    
    // 输出宽字符串
    DWORD written;
    WriteConsoleW(hConsoleOutput, wstr, wcslen(wstr), &written, NULL);
    
    delete[] wstr;
}

// === 终端属性 ===

int Terminal::getWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 80; // 默认宽度
}

int Terminal::getHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return 24; // 默认高度
}

// === 颜色控制 ===

void Terminal::setTextColor(Color color) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &csbi)) {
        WORD attributes = csbi.wAttributes;
        attributes &= 0xFFF0; // 清除前景色
        attributes |= static_cast<WORD>(color);
        SetConsoleTextAttribute(hConsoleOutput, attributes);
    }
}

void Terminal::resetTextColor() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &csbi)) {
        WORD attributes = csbi.wAttributes;
        attributes &= 0xFFF0; // 清除前景色
        attributes |= (originalAttributes & 0x000F); // 恢复原始前景色
        SetConsoleTextAttribute(hConsoleOutput, attributes);
    }
}

void Terminal::setBackgroundColor(Color color) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &csbi)) {
        WORD attributes = csbi.wAttributes;
        attributes &= 0xFF0F; // 清除背景色
        attributes |= (static_cast<WORD>(color) << 4);
        SetConsoleTextAttribute(hConsoleOutput, attributes);
    }
}

void Terminal::resetBackgroundColor() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsoleOutput, &csbi)) {
        WORD attributes = csbi.wAttributes;
        attributes &= 0xFF0F; // 清除背景色
        attributes |= (originalAttributes & 0x00F0); // 恢复原始背景色
        SetConsoleTextAttribute(hConsoleOutput, attributes);
    }
}