#include "UserInfoLayout.h"
#include "../Game.h"
#include "../../class/entity/Player.h"
#include "../GameTime.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
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

std::string UserInfoLayout::formatMoney() const {
    std::stringstream ss;
    ss << "💰 金钱: " << std::fixed << std::setprecision(2) << player_.getSavings() << " 元";
    return ss.str();
}

Element UserInfoLayout::OnRender() {
    // 游戏时间区域
    auto timeSection = vbox({
        text("时间与位置") | bold | hcenter,
        separator(),
        text(formatGameTime()) | hcenter | color(Color::Cyan),
        text(" "),
        text("所在位置: " + player_.getLocation()) | color(Color::White),
    }) | border | flex;

    // 玩家状态区域 - 修改为多个独立的text元素
    auto statsSection = vbox({
        text("👤 玩家状态") | bold | hcenter,
        separator(),
        text("玩家姓名: " + player_.getName()) | color(Color::White),
        text("生命值: " + std::to_string(static_cast<int>(player_.getHealth()))) | color(Color::White),
        text("饱食度: " + std::to_string(static_cast<int>(player_.getHunger()))) | color(Color::White),
        text("体力值: " + std::to_string(static_cast<int>(player_.getFatigue()))) | color(Color::White),
        text("敏捷度: " + std::to_string(static_cast<int>(player_.getAgility()))) | color(Color::White),
        text("力量: " + std::to_string(static_cast<int>(player_.getStrength()))) | color(Color::White),
        text("耐力: " + std::to_string(static_cast<int>(player_.getStamina()))) | color(Color::White),
        text(" ")
    }) | border | flex;

    // 金钱区域
    auto moneySection = vbox({
        text("银行") | bold | hcenter,
        separator(),
        text(formatMoney()) | hcenter | color(Color::Yellow),
        text(" ")
    }) | border | flex;

    // 技能区域（右下角）
    auto skillSection = vbox({
        text("技能状态") | bold | hcenter,
        separator(),
        text("技能点: " + std::to_string(static_cast<int>(player_.getSkillPoints()))) | color(Color::Green),
        text("已学技能: " + std::to_string(player_.getLearnedSkillNames().size()) + "个") | color(Color::Green),
        text(" ")
    }) | border | flex;

    // 田字型布局：上下两行，每行左右两列
    auto topRow = hbox({
        timeSection | flex,     // 左上：时间
        text(" "),
        statsSection | flex     // 右上：玩家信息
    });

    auto bottomRow = hbox({
        moneySection | flex,    // 左下：金钱
        text(" "),
        skillSection | flex     // 右下：技能状态
    });

    // 主要内容布局 - 田字型
    auto mainContent = vbox({
        topRow | flex,
        text(" "),
        bottomRow | flex,
        text(" "),
        exitButton_->Render() | hcenter
    });

    // 窗口包装
    auto window = vbox({
        filler(),
        hbox({
            filler(),
            mainContent | size(WIDTH, LESS_THAN, 80) | size(HEIGHT, LESS_THAN, 30),
            filler()
        }),
        filler()
    });

    return window | clear_under;
}
