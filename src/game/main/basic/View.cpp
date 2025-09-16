#include "Dialog.h"
#include "Game.h"
#include "View.h"
#include "ui/GameLayout.h"
#include "ui/SettingsLayout.h"
#include "Configuration.h"

#include "FTXUI/component/component.hpp"
#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"

#include <atomic>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

View::View(Game& game_logic) : game_logic_(game_logic) {}

void View::showMainMenu() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    // --- 状态标志：定义所有互斥的UI层 ---
    bool showSettings = false;
    bool showMenu = true; // 主菜单默认显示

    // --- 定义交互组件和状态 ---
    auto settingsLayout = Make<SettingsLayout>(game_logic_, showSettings);

    auto* settingsPtr = settingsLayout.get();

    std::vector<std::string> menuItems = {
        game_logic_.isFirstNewGame ? "开始新游戏" : "继续游戏", "读取存档", "游戏介绍", "游戏设置", "退出游戏"
    };

    // 按钮的回调函数现在通过 game_logic_ 引用来调用 Game 类的方法
    auto menu= Container::Vertical({
        Button(menuItems[0], [&] {
            screen.ExitLoopClosure()();
            // 如果是新游戏，调用 startNewGame()；否则，调用 loadGame()。
            game_logic_.isFirstNewGame ? game_logic_.startNewGame() : game_logic_.loadGame(); }, ButtonOption::Animated()),
        // 暂时移除读档按钮：目前只有一个存档，功能与第一个按钮的继续游戏重复了
        // Button(menuItems[1], [&] { screen.ExitLoopClosure()(); game_logic_.loadGame(); }, ButtonOption::Animated()),
        Button(menuItems[2], [&] { game_logic_.showGameIntro(); }, ButtonOption::Animated()),
        Button(menuItems[3], [&showSettings, settingsPtr] {
            settingsPtr->loadSettings(); // 显示前加载最新设置
            showSettings = true;
        }, ButtonOption::Animated()),
        Button(menuItems[4], [&] { game_logic_.exitGame(); }, ButtonOption::Animated())
    });

    auto mainMenuLayout = Renderer(menu, [&] {
        Element layout = vbox({
            vbox({
                text("    :=   #%-  :.                                 :%*           .::.::.   -%*      ") | color(Color::Red),
                text("    -@+ -@%  *@+      *@@@@@@@@@@@@@@@#           #@%.         *@##*@@.  %@%###*. ") | color(Color::Red),
                text("  @@@@@@@@@@@@@@@#           +@%           ********@#*****-    *@:  @@.:@@+::#@#  ") | color(Color::Red),
                text("      :%@=                   +@@           ############@@@*    *@@@@@@#@@@@-=@%   ") | color(Color::Red),
                text(".###%@@@####@@@####          +@%                      #@@-     .::@@::    +@@*    ") | color(Color::Red),
                text("  :*@@%-=+**+#@%-      #@@@@@@@@@@@@@@              +@@+       +# @@.   -%@%#@@+. ") | color(Color::Red),
                text("=@@%= -:=@%    #@@@=         +@%                  =@@#         #@ @@@@@@@#:  .#@%.") | color(Color::Red),
                text("    @@@@@@@@@@@@             +@@                %@@*           #@ @@   .@@@@@@@@: ") | color(Color::Red),
                text("  ####%#%@@##%####           +@%            +#@@@=             #@ @@+*+.@@    @@: ") | color(Color::Red),
                text("        :@%          #@@@@@@@@@@@@@@@@@%  *@@++%@%*=====+++*+ *@@@@@#*=.@@++=+@@: ") | color(Color::Red),
                text("      #@@@+          :::::::::::::::::::   *     -*@@@@@%@%@. .:        @@==+=@@. ") | color(Color::Red)
            }) | center | border,

            menu->Render() | flex | center,

            separator(),
            hbox(text("使用方向键上下移动，Enter键选择") | color(Color::Blue) | center),
            hbox(text("游戏中请尽量不要命令行界面大小, 全屏游玩体验最佳") | color(Color::Blue) | center)
        });

        return layout;
    });

    auto topLevelContainer = Container::Stacked({
        // 底层：主菜单
        Maybe(mainMenuLayout, &showMenu),
        // 顶层：设置菜单
        Maybe(settingsLayout, &showSettings),
    });

    auto finalRenderer = Renderer(topLevelContainer, [&] {
        // A. 状态同步：确保 menu 和 settings 互斥
        //    这是连接我们两个布尔状态的关键。
        showMenu = !showSettings;

        // B. 焦点管理
        if (showSettings) {
            settingsLayout->TakeFocus();
        } else {
            menu->TakeFocus();
        }

        // C. 构建视觉布局
        return topLevelContainer->Render() | border;
    });

    screen.Loop(finalRenderer);
}

void View::showGameIntroScreen() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    auto exitButton = Button(" 退 出 ", screen.ExitLoopClosure(), ButtonOption::Animated());

    auto gameIntro = vbox({
        text("《拳王之路》—— 你的拳头，决定你的天下！") | bold | color(Color::RedLight) | center,
        separatorEmpty(),

        hflow(
            paragraph("《拳王之路》是一款融合了硬核格斗、深度经营与角色养成的多元化角色扮演游戏。"),
            paragraph("在这里，你并非只是一个只会出拳的机器。你将扮演一位怀揣拳王梦想的格斗新星，"),
            paragraph("从零开始，全面主宰自己的职业生涯")
            ),
        separatorEmpty(),

        vbox({
            text("• 作为运动员：") | bold | color(Color::Cyan),
            hflow(
                paragraph("你需要在健身房里挥汗如雨，进行刻苦训练，提升力量、速度、耐力。"),
                paragraph("学习全新的拳法组合，研究对手的弱点，制定专属战术，在擂台上用实力击败每一个敌人。")
                ),
        }),
        separatorEmpty(),

        vbox({
            text("• 作为管理者：") | bold | color(Color::Cyan),
            hflow(
                paragraph("你必须精明地经营自己的事业。合理安排时间平衡好训练和打工收入，"),
                paragraph("管理个人收支。每一步决策都至关重要，都影响着你的发展轨迹。")
                ),
        }),
        separatorEmpty(),

        vbox({
            text("• 作为追梦者：") | bold | color(Color::Cyan),
            hflow(
                paragraph("你将从籍籍无名的俱乐部底层开始打拼，通过一场场胜利积累声望，"),
                paragraph("最终在世界级的格斗殿堂中争夺至高无上的「拳王」金腰带。")
                ),
        }),
        separatorEmpty(),

        hflow(
            paragraph("然而，通往巅峰的道路绝非一帆风顺。你需要把握转瞬即逝的机遇，"),
            paragraph("应对突如其来的伤病与强大的对手。你的智慧、毅力与抉择，将共同谱写属于你的传奇。")
            ),
        separatorEmpty(),
        separator(),

        vbox({
            text("游戏基础操作介绍：") | bold | color(Color::Yellow),
            text("1. 游戏内使用输入 /help 获取命令提示"),
            text("2. 在遇到需要玩家自由操纵人物的场景，使用 wasd 键以移动人物"),
            text("3. 使用键盘 tab 键切换按钮或输入框，或者直接使用鼠标光标选取"),
        }) | border,
    });

    auto renderer = Renderer(exitButton, [&] {
        return vbox({
                   text("游 戏 介 绍") | bold | color(Color::RedLight) | center,
                   separator(),
                   gameIntro | vscroll_indicator | yframe | flex ,
                   separator(),
                   exitButton->Render() | center,
               }) |
               size(WIDTH, LESS_THAN, 120) | size(HEIGHT, LESS_THAN, 35) | border | center;
    });

    screen.Loop(renderer);
}

void View::showLoadingScreen(const std::string& subtitle) {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();
    float progress = 0.0f;

    auto component = Renderer([&] {
        return vbox({
            text("加 载 中") | color(Color::Green) | bold | center,
            text(subtitle) | color(Color::White) | center,
            separator(),
            gauge(progress) | flex | color(Color::Green)
        }) | border | size(WIDTH, EQUAL, 60) | size(HEIGHT, EQUAL, 8) | center;
    });

    std::thread loadingThread([&] {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(2000, 5000);
        int totalDurationMs = distrib(gen);

        int totalSteps = 100;
        int sleepPerStepMs = totalDurationMs / totalSteps;

        for (int i = 0; i < totalSteps; ++i) {
            float linearTime = static_cast<float>(i + 1) / totalSteps;
            progress = 1.0f - (1.0f - linearTime) * (1.0f - linearTime);

            screen.PostEvent(Event::Custom);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepPerStepMs));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        screen.ExitLoopClosure()();
    });

    screen.Loop(component);
    loadingThread.join();
}

void View::showGameScreen() {
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    // 创建我们的自定义主组件
    auto gameLayout = Make<GameLayout>(game_logic_);

    // 使用配置文件中的打字机速度
    std::atomic<bool> refreshRunning{true};
    std::thread refreshThread([&] {
        while (refreshRunning) {
            screen.PostEvent(Event::Custom);
            // 从配置文件获取打字机速度
            int typewriterSpeed = game_logic_.getConfig().getTypewriterSpeed();
            std::this_thread::sleep_for(std::chrono::milliseconds(typewriterSpeed));
        }
    });

    screen.Loop(gameLayout); // 启动包含事件处理的循环

    // 退出循环后停止刷新线程
    refreshRunning = false;
    if (refreshThread.joinable()) {
        refreshThread.join();
    }
}
void View::showTemporaryPopup(const std::string& message, unsigned int duration_seconds) {
    using namespace ftxui;

    // 1. 使用 Fullscreen 模式，这样我们才有空间进行居中
    auto screen = ScreenInteractive::Fullscreen();

    // 2. 定义弹窗本身的内容和样式
    auto popupContent = vbox({
        text(" 提 示 ") | bold | hcenter,
        separator(),
        paragraph(" " + message + " ") | hcenter, // 使用 paragraph 以支持自动换行
    }) | border | size(WIDTH, LESS_THAN, 50);

    // 3. 创建一个渲染器组件，它负责将弹窗居中显示
    auto finalComponent = Renderer([&] {
        // 使用 dbox 将弹窗(前景)绘制在一个空的填充物(背景)之上
        // 并使用 | center 将弹窗居中
        return dbox({
            filler(),               // 背景：一个空的、会填满整个屏幕的元素
            popupContent | center, // 前景：我们的弹窗，被放置在屏幕中央
        });
    });

    // 4. 计时器线程逻辑保持不变
    std::thread timerThread([&screen, duration_seconds] {
        std::this_thread::sleep_for(std::chrono::seconds(duration_seconds));
        screen.ExitLoopClosure()();
    });

    // 5. 启动临时的事件循环
    screen.Loop(finalComponent);

    // 6. 等待计时器线程结束
    timerThread.join();
}