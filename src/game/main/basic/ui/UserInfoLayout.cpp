#include "UserInfoLayout.h"
#include "../Game.h"
#include "../../class/entity/Player.h"
#include "../GameTime.h"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"
#include <sstream>
#include <iomanip>

using namespace ftxui;

UserInfoLayout::UserInfoLayout(Game& game_logic, bool& isShowingFlag)
    : game_logic_(game_logic), player_(game_logic.getPlayer()), isShowingFlag_(isShowingFlag) {

    // 创建退出按钮
    exitButton_ = Button("返回", [this] {
        isShowingFlag_ = false;
    });

    // 创建主容器
    mainContainer_ = Container::Vertical({exitButton_});

    // 添加到组件
    Add(mainContainer_);
}

std::string UserInfoLayout::formatGameTime() const {
    unsigned int gameHour = GameTime::getHour();
    unsigned int gameMinute = GameTime::getMinute();
    unsigned int gameDay = GameTime::getDay();
    unsigned int gameMonth = GameTime::getMonth();
    unsigned int gameYear = GameTime::getYear();

    std::stringstream ss;
    ss << gameYear << "年" << gameMonth << "月" << gameDay << "日 "
       << std::setw(2) << std::setfill('0') << gameHour << ":"
       << std::setw(2) << std::setfill('0') << gameMinute;
    return ss.str();
}

std::string UserInfoLayout::formatPlayerStats() const {
    std::stringstream ss;
    ss << "玩家姓名: " << player_.getName() << "\n"
       << "健康度: " << std::fixed << std::setprecision(1) << (player_.getHealth()) << "%\n"
       << "饥饿度: " << std::fixed << std::setprecision(1) << (player_.getHunger() * 100) << "%\n"
       << "所在位置: " << player_.getLocation();
    return ss.str();
}

std::string UserInfoLayout::formatMoney() const {
    std::stringstream ss;
    ss << "💰 金钱: " << std::fixed << std::setprecision(2) << player_.getSavings() << " 元";
    return ss.str();
}

Element UserInfoLayout::Render() {
    // 游戏时间区域
    auto timeSection = vbox({
        text("⏰ 当前时间") | bold | hcenter,
        separator(),
        text(formatGameTime()) | hcenter | color(Color::Cyan),
        text(" ")
    }) | border | flex;

    // 玩家状态区域
    auto statsSection = vbox({
        text("👤 玩家信息") | bold | hcenter,
        separator(),
        paragraph(formatPlayerStats()) | color(Color::White),
        text(" ")
    }) | border | flex;

    // 金钱区域
    auto moneySection = vbox({
        text("💰 财务状况") | bold | hcenter,
        separator(),
        text(formatMoney()) | hcenter | color(Color::Yellow),
        text(" ")
    }) | border | flex;

    // 主要内容布局
    auto mainContent = vbox({
        timeSection,
        text(" "),
        statsSection,
        text(" "),
        moneySection,
        text(" "),
        exitButton_->Render() | hcenter
    });

    // 窗口包装
    auto window = vbox({
        filler(),
        hbox({
            filler(),
            mainContent | size(WIDTH, LESS_THAN, 50) | size(HEIGHT, LESS_THAN, 25),
            filler()
        }),
        filler()
    });

    return window | clear_under;
}
