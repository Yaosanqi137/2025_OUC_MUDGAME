#ifndef SETTINGSLAYOUT_H
#define SETTINGSLAYOUT_H

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"
#include <vector>
#include <string>

class Game; // 前向声明

/**
 * @class SettingsLayout
 * @brief 实现了游戏设置界面的FTXUI组件。
 * @details 作为一个可复用的弹出层，允许用户修改游戏配置。
 *          通过回调函数与调用者解耦。
 */
class SettingsLayout : public ftxui::ComponentBase {
public:
    /**
     * @brief SettingsLayout的构造函数。
     * @param game_logic 对Game核心对象的引用。
     * @param isShowingFlag 对显示标志的引用。
     */
    explicit SettingsLayout(Game& game_logic, bool& isShowingFlag);

    ftxui::Element OnRender() override;

    void loadSettings();

private:
    /**
     * @brief 将临时状态中的设置应用并保存到Configuration。
     */
    void applyAndSaveChanges() const;
    void updateSpeedString(); // 辅助函数，同步字符串和整数

    Game& game_logic_;
    bool& isShowingFlag_;

    // --- 临时状态和数据 ---
    int tempDifficultyIndex_ = 1; // 0:简单, 1:普通, 2:困难
    int tempTypewriterSpeed_ = 20;
    std::string tempSpeedStr_; // 用于Input组件的字符串
    std::vector<std::string> difficultyEntries_;

    // --- FTXUI 子组件 ---
    ftxui::Component difficultyRadiobox_;

    ftxui::Component speedInput_;
    ftxui::Component speedDecrementButton_;
    ftxui::Component speedIncrementButton_;

    ftxui::Component applyButton_;
    ftxui::Component backButton_;
    ftxui::Component mainContainer_;
};

#endif // SETTINGSLAYOUT_H