#include <iostream>

#include "basic/Game.h"
#include "basic/Terminal.h"

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        // 捕获并显示异常
        std::cerr << "\n游戏运行时错误: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
