#include "SettingsLayout.h"
#include "../Game.h"
#include "../Configuration.h"
#include <string>

using namespace ftxui;

SettingsLayout::SettingsLayout(Game& game_logic)
    : game_logic_(game_logic), difficultyEntries_{" 简单 ", " 普通 ", " 困难 "} {

    // --- 初始化UI组件 ---
    difficultyRadiobox_ = Radiobox(&difficultyEntries_, &tempDifficultyIndex_);

    InputOption speedInputOption;
    speedInputOption.on_change = [this] {
        // 过滤绑定的字符串，只保留数字
        tempSpeedStr_.erase(
            std::remove_if(tempSpeedStr_.begin(), tempSpeedStr_.end(),
                           [](char c) { return !std::isdigit(c); }),
            tempSpeedStr_.end()
        );
        // 从过滤后的字符串更新整数值
        try {
            int speed = std::stoi(tempSpeedStr_);
            if (!tempSpeedStr_.empty()) {
                if (speed < 10) {
                    speed = 10;
                }
                if (speed > 200) {
                    speed = 200;
                }
                tempTypewriterSpeed_ = speed;
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
        if (tempTypewriterSpeed_ < 10) {
            tempTypewriterSpeed_ = 10;
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
        hide(); // 应用后隐藏
    }, ButtonOption::Animated());

    backButton_ = Button("   返 回   ", [this] {
        hide(); // 直接隐藏
    }, ButtonOption::Animated());

    // --- 将子组件装配到容器中 ---
    auto speedControl = Container::Horizontal({
        speedDecrementButton_,
        speedInput_,
        speedIncrementButton_,
    });

    mainContainer_ = Container::Vertical({
        difficultyRadiobox_,
        speedControl,
        Container::Horizontal({applyButton_, backButton_})
    });

    Add(mainContainer_);
}

void SettingsLayout::show() {
    loadSettings();
    isShowing_ = true;
}

void SettingsLayout::hide() {
    isShowing_ = false;
}

bool SettingsLayout::isShowing() const {
    return isShowing_;
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

void SettingsLayout::applyAndSaveChanges() {
    // 将临时变量中的设置应用到全局配置并保存
    auto* config = Configuration::getInstance();
    config->setDifficultyLevel(tempDifficultyIndex_ + 1);
    config->setTypewriterSpeed(tempTypewriterSpeed_);
    config->saveConfig();
}

Element SettingsLayout::Render() {
    if (!isShowing_) {
        return text(""); // 如果不可见，不渲染任何东西
    }
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
    }) | border;

    return layout | clear_under | bgcolor(Color::Black);
}