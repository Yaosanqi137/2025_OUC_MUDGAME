#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <string>

#include <ftxui/component/component.hpp>

// 告知 Game 类存在
class Game;

class View {
public:
    // 构造函数接收一个 Game 逻辑控制器的引用
    explicit View(Game& game_logic);

    // 主菜单
    void showMainMenu();

    // 加载栏
    static void showLoadingScreen(const std::string& subtitle);

    // 游戏界面
    void showGameScreen();

    // 游戏介绍
    static void showGameIntroScreen();

    // 主UI: 迁移至/src/game/basic/ui/GameLayout.h(.cpp)
    // ftxui::Component makeGameLayout();

    /**
     * @brief 显示一个临时的、会自动关闭的弹窗消息。
     * @param message 要显示的消息内容。
     * @param duration_seconds 弹窗显示的秒数，默认为3秒。
     */
    static void showTemporaryPopup(const std::string& message, unsigned int duration_seconds = 3);

private:
    // 保存 Game 对象的引用，以便在按钮点击时调用其方法
    Game& game_logic_;
};

#endif // GAME_VIEW_H