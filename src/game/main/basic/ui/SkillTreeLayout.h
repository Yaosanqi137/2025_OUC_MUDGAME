// File: D:/CODE/2025_OUC_MUDGAME/src/game/main/basic/ui/SkillTreeLayout.h

#ifndef SKILLTREELAYOUT_H
#define SKILLTREELAYOUT_H

#include <ftxui/component/component.hpp>

class Game;

/**
 * @class SkillTreeLayout
 * @brief 实现了只读技能树可视化功能的FTXUI组件。
 */
class SkillTreeLayout : public ftxui::ComponentBase {
public:
    explicit SkillTreeLayout(Game& game_logic, bool& isShowingFlag);
    ftxui::Element OnRender() override;

private:
    // 辅助函数，用于生成FTXUI的graph元素
    [[nodiscard]] ftxui::Element buildSkillTreeGraph() const;

    Game& game_logic_;
    bool& isShowingFlag_;

    int currentPage_ = 0;

    // UI组件
    ftxui::Component exitButton_;
    ftxui::Component mainContainer_;
    ftxui::Component pagePrevButton_;
    ftxui::Component pageNextButton_;

    // Add this helper function declaration
    [[nodiscard]] const std::vector<std::vector<std::string>>& getBlueprint() const;
};

#endif // SKILLTREELAYOUT_H