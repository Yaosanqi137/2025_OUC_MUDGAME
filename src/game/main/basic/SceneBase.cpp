#include "SceneBase.h"
#include "Terminal.h"

// 获取边框字符（可自定义）
void SceneBase::getBorderChars(std::string& topLeft, std::string& topRight,
                               std::string& bottomLeft, std::string& bottomRight,
                               std::string& horizontal, std::string& vertical) const {
    topLeft = "+"; topRight = "+";
    bottomLeft = "+"; bottomRight = "+";
    horizontal = "-"; vertical = "|";
}

// 绘制基本边框
void SceneBase::drawBorder(int x, int y, int width, int height) const {
    std::string topLeft, topRight, bottomLeft, bottomRight, horizontal, vertical;
    getBorderChars(topLeft, topRight, bottomLeft, bottomRight, horizontal, vertical);

    // 上边框
    terminal.setCursorPosition(x, y);
    terminal.print(topLeft);
    for (int i = 0; i < width - 2; i++) {
        terminal.print(horizontal);
    }
    terminal.print(topRight);

    // 左右边框
    for (int j = 1; j < height - 1; j++) {
        terminal.setCursorPosition(x, y + j);
        terminal.print(vertical);

        terminal.setCursorPosition(x + width - 1, y + j);
        terminal.print(vertical);
    }

    // 下边框
    terminal.setCursorPosition(x, y + height - 1);
    terminal.print(bottomLeft);
    for (int i = 0; i < width - 2; i++) {
        terminal.print(horizontal);
    }
    terminal.print(bottomRight);
}

// 绘制带标题的边框
void SceneBase::drawTitledBorder(int x, int y, int width, int height,
                                const std::string& title) const {
    drawBorder(x, y, width, height);

    // 添加标题
    std::string titleText = " " + title + " ";
    int titleX = x + (width - titleText.length()) / 2;

    terminal.setCursorPosition(titleX, y);
    terminal.print(titleText);
}