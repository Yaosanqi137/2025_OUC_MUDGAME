#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>
#include <windows.h>
#include <conio.h>

/**
 * @class Terminal
 * @brief Windows 终端控制类，处理控制台的输入输出
 */
class Terminal {
public:
    // 禁用复制和赋值
    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    // 单例访问
    static Terminal& getInstance();

    // 初始化
    static void init();

    // 输入处理
    char getKey();
    std::string getLine();

    // 输出控制
    void clearScreen();
    void setCursorPosition(int x, int y);
    void print(const std::string& text);

    // 终端属性
    int getWidth();
    int getHeight();

    // 文本颜色
    enum class Color {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        YELLOW = 6,
        WHITE = 7,
        GRAY = 8,
        BRIGHT_BLUE = 9,
        BRIGHT_GREEN = 10,
        BRIGHT_CYAN = 11,
        BRIGHT_RED = 12,
        BRIGHT_MAGENTA = 13,
        BRIGHT_YELLOW = 14,
        BRIGHT_WHITE = 15
    };

    void setTextColor(Color color);
    void resetTextColor();
    void setBackgroundColor(Color color);
    void resetBackgroundColor();

private:
    // 禁止外部构造与析构
    Terminal();
    ~Terminal();

    // 恢复原始控制台状态
    void restoreOriginalState();

    // 控制台句柄
    HANDLE hConsoleInput;
    HANDLE hConsoleOutput;

    // 保存的原始状态
    CONSOLE_SCREEN_BUFFER_INFO originalCSBI;
    DWORD originalConsoleMode;
    WORD originalAttributes;
};

#endif // TERMINAL_H