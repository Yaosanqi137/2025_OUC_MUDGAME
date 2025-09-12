#include "SettingsLayout.h"
#include "../Game.h"
#include "../Configuration.h"
#include <string>

using namespace ftxui;

SettingsLayout::SettingsLayout(Game& game_logic, bool& isShowingFlag)
    : game_logic_(game_logic), isShowingFlag_(isShowingFlag), difficultyEntries_{" 简单 ", " 普通 ", " 困难 "} {

    // --- 初始化UI组件 ---
    difficultyRadiobox_ = Radiobox(&difficultyEntries_, &tempDifficultyIndex_);

    InputOption speedInputOption;
    speedInputOption.on_change = [this] {
        // 过滤绑定的字符串，只保留数字
        std::erase_if(tempSpeedStr_, [](const char c) { return !std::isdigit(c); });
        // 从过滤后的字符串更新整数值
        try {
            if (!tempSpeedStr_.empty()) {
                tempTypewriterSpeed_ = std::stoi(tempSpeedStr_);
            }
        } catch (const std::out_of_range&) {
            // 如果输入的数字超出了int的范围，则设置为最大值
            tempTypewriterSpeed_ = 200;
            updateSpeedString();
        } catch (const std::invalid_argument&) {
            // 如果字符串为空或无效，则不做任何事
        }
    };
    // 只允许输入数字
    speedInput_ = Input(&tempSpeedStr_, "10-200", speedInputOption);

    speedDecrementButton_ = Button("  <  ", [this] {
        tempTypewriterSpeed_ -= 5;
        if (tempTypewriterSpeed_ < 5) {
            tempTypewriterSpeed_ = 5;
        }
        updateSpeedString();
    });

    speedIncrementButton_ = Button("  >  ", [this] {
        tempTypewriterSpeed_ += 5;
        if (tempTypewriterSpeed_ > 200) {
            tempTypewriterSpeed_ = 200;
        }
        updateSpeedString();
    });

    applyButton_ = Button(" 应用并保存 ", [this] {
        applyAndSaveChanges();
        isShowingFlag_ = false;
    }, ButtonOption::Animated());

    backButton_ = Button("   返 回   ", [this] {
        isShowingFlag_ = false;
    }, ButtonOption::Animated());

    // --- 将所有可交互的组件添加到一个扁平的容器中 ---
    // 这个容器的唯一职责是处理事件分发。
    // 它的结构是扁平的，确保了 Tab 和方向键导航的正确性。
    mainContainer_ = Container::Vertical({
        difficultyRadiobox_,
        speedDecrementButton_,
        speedInput_,
        speedIncrementButton_,
        applyButton_,
        backButton_,
    });

    Add(mainContainer_);
}

void SettingsLayout::updateSpeedString() {
    tempSpeedStr_ = std::to_string(tempTypewriterSpeed_);
}

void SettingsLayout::loadSettings() {
    auto* config = Configuration::getInstance();
    tempDifficultyIndex_ = config->getDifficultyLevel() - 1;
    tempTypewriterSpeed_ = config->getTypewriterSpeed();
    updateSpeedString();
}

void SettingsLayout::applyAndSaveChanges() const {
    // 将临时变量中的设置应用到全局配置并保存
    auto* config = Configuration::getInstance();
    config->setDifficultyLevel(tempDifficultyIndex_ + 1);
    config->setTypewriterSpeed(tempTypewriterSpeed_);
    config->saveConfig();
}

Element SettingsLayout::Render() {
    // loadSettings();

    auto speedControlRenderer = hbox({
        text("文本速度 (ms/char): ") | center,
        speedDecrementButton_->Render(),
        speedInput_->Render() | size(WIDTH, EQUAL, 7) | hcenter,
        speedIncrementButton_->Render(),
    }) | center;

    auto layout = vbox({
        filler(),
        text(" 游 戏 设 置 ") | bold | hcenter,
        separatorDouble(),
        text("游戏难度") | bold | hcenter,
        difficultyRadiobox_->Render() | hcenter,
        text(" ·  ·  ·  ·  ·  ·  ·  ·  ·  · ") | center,
        speedControlRenderer,
        text(" ·  ·  ·  ·  ·  ·  ·  ·  ·  · ") | center,
        hbox({
            filler(),
            applyButton_->Render(),
            filler(),
            backButton_->Render(),
            filler(),
        }) | hcenter,
        filler(),
    }) | border | size(WIDTH, GREATER_THAN, 160) | size(HEIGHT, GREATER_THAN, 90);

    return dbox({
        layout | center,
    });
}