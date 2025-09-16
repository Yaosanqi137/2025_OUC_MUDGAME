#include "SkillTreeLayout.h"
#include "../Game.h"
#include "../../class/entity/Player.h"
#include "ftxui/dom/elements.hpp"
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace ftxui;

SkillTreeLayout::SkillTreeLayout(Game& game_logic, bool& isShowingFlag)
    : game_logic_(game_logic), isShowingFlag_(isShowingFlag), currentPage_(0)  {

    exitButton_ = Button("[  返 回  ]", [this] {
        isShowingFlag_ = false;
        currentPage_ = 0;
    });

    pagePrevButton_ = Button("< 上一页", [this] {
        if (currentPage_ > 0) {
            currentPage_--;
        }
    });

    pageNextButton_ = Button("下一页 >", [this] {
        const std::vector<std::vector<std::string>> blueprint = getBlueprint();
        constexpr int columnsPerPage = 10;
        const int totalColumns = blueprint.empty() ? 0 : blueprint[0].size();
        const int totalPages = (totalColumns + columnsPerPage - 1) / columnsPerPage;
        if (currentPage_ < totalPages - 1) {
            currentPage_++;
        }
    });

    mainContainer_ = Container::Vertical({exitButton_, pagePrevButton_, pageNextButton_});
    Add(mainContainer_);
}

Element SkillTreeLayout::buildSkillTreeGraph() const {
    const auto& player = game_logic_.getPlayer();
    const auto& learnedSkills = player.getLearnedSkillNames();
    const std::set<std::string> learnedSet(learnedSkills.begin(), learnedSkills.end());

    constexpr int CELL_WIDTH = 12;
    constexpr int CELL_HEIGHT = 1;
    constexpr int columnsPerPage = 8;

    // 动态获取技能元素
    auto createSkillElement = [&](const std::string& name) {
        const bool isLearned = learnedSet.contains(name);
        const bool canLearn = player.canLearnSkill(name);

        auto style = color(Color::White);
        if (isLearned) {
            style = color(Color::Green);
        } else if (canLearn) {
            style = color(Color::Yellow);
        }

        return text(name) | style | center | size(WIDTH, EQUAL, CELL_WIDTH) | size(HEIGHT, EQUAL, CELL_HEIGHT);
    };

    // 技能映射
    const std::map<std::string, std::string> skillMap = {
        {"_S01_", "直拳"}, {"_S02_", "踢腿"}, {"_A01_", "高踢腿"}, {"_A02_", "屹立不倒"}, {"_A03_", "反手重拳"},
        {"_A04_", "空手道高踢腿"}, {"_A05_", "空手道踢腿"}, {"_A06_", "空手道劈斩"}, {"_A07_", "闪击"},
        {"_A08_", "屹立不倒2"}, {"_A09_", "迅速"}, {"_A10_", "钝兵挫锐"}, {"_A11_", "屹立不倒3"},
        {"_B01_", "重拳"}, {"_B02_", "肉食跑者"}, {"_B03_", "反手直拳"}, {"_B04_", "爪击"},
        {"_B05_", "自杀式袭击"}, {"_B06_", "折背"}, {"_B07_", "千手不破"}, {"_B08_", "肉食跑者2"},
        {"_B09_", "迅击"}, {"_B10_", "意志"}, {"_B11_", "无限能量"}, {"_B12_", "肉食跑者3"},
        {"_C01_", "上钩拳"}, {"_C02_", "肌肉记忆"}, {"_C03_", "交叉拳"}, {"_C04_", "拳术直拳"},
        {"_C05_", "肌肉记忆2"}, {"_C06_", "蓄力上钩拳"}, {"_C07_", "近战缠斗"}, {"_C08_", "人身重锤"},
        {"_C09_", "激励"}, {"_C10_", "拳击手"}, {"_C11_", "肌肉记忆3"}
    };

    // 连接符映射
    const std::map<std::string, std::string> connectorMap = {
        {"→", "───────────→"},
        {"─", "────────────"},
        {"│", "      │     "},
        {"┤", "──────┤     "},
        {"├", "      ├─────"},
        {"┌", "      ┌─────"},
        {"└", "      └─────"},
        {"┐", "──────┐     "},
        {"┘", "──────┘     "},
        {"┼", "──────┼─────"}
    };

    const auto& blueprint = getBlueprint();
    int totalColumns = blueprint.empty() ? 0 : blueprint[0].size();
    int startCol = currentPage_ * columnsPerPage;
    int endCol = std::min(startCol + columnsPerPage, totalColumns);

    Elements renderedRows;
    for (const auto& rowBlueprint : blueprint) {
        Elements rowElements;
        for (int i = startCol; i < endCol; ++i) {
            const auto& placeholder = rowBlueprint[i];
            if (skillMap.contains(placeholder)) {
                rowElements.push_back(createSkillElement(skillMap.at(placeholder)));
            } else if (placeholder == "_BLANK_") {
                rowElements.push_back(text("") | size(WIDTH, EQUAL, CELL_WIDTH));
            } else if (connectorMap.contains(placeholder)) {
                rowElements.push_back(text(connectorMap.at(placeholder)) | center | size(WIDTH, EQUAL, CELL_WIDTH));
            } else {
                rowElements.push_back(text("") | size(WIDTH, EQUAL, CELL_WIDTH));
            }
        }
        renderedRows.push_back(hbox(std::move(rowElements)));
    }

    return vbox(std::move(renderedRows)) | hcenter;
}

Element SkillTreeLayout::Render() {
    auto title = text(" 技 能 树 ") | bold | hcenter;

    auto legend = hbox({
        text("图例: "),
        text("■") | color(Color::Green), text(" 已学习  "),
        text("■") | color(Color::Yellow), text(" 可学习  "),
        text("■") | color(Color::White), text(" 未激活"),
        text("  |  ") | color(Color::White), text("技能学习请在主界面输入 /skill 以获取帮助") | color(Color::Green),
    }) | hcenter;

    auto treeContent = buildSkillTreeGraph();

    // 导航栏
    const auto& blueprint = getBlueprint();
    constexpr int columnsPerPage = 10;
    const int totalColumns = blueprint.empty() ? 0 : blueprint[0].size();
    const int totalPages = (totalColumns + columnsPerPage - 1) / columnsPerPage;
    const std::string pageInfo = "第 " + std::to_string(currentPage_ + 1) + " / " + std::to_string(totalPages) + " 页";

    auto navigation = hbox({
        pagePrevButton_->Render(),
        filler(),
        text(pageInfo) | center,
        filler(),
        pageNextButton_->Render(),
    });

    auto treeWindow = window(title, vbox(Elements{
        legend,
        separator(),
        treeContent | flex,
    }));

    return dbox(Elements{
        filler() | bgcolor(Color::Black),
        vbox(Elements{
            treeWindow | flex,
            navigation | center,
            exitButton_->Render() | hcenter,
            text(" "),
        }),
    });
}

const std::vector<std::vector<std::string>>& SkillTreeLayout::getBlueprint() const {
    static const std::vector<std::vector<std::string>> blueprint = {
        {"_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "┌", "→", "_A05_", "→", "_A07_", "→", "_A08_", "┐", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_"},
        {"_BLANK_", "┌", "→", "_A01_", "→", "_A02_", "→", "_A03_", "┤", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "├", "→", "_A09_", "→", "_A10_", "→", "_A11_"},
        {"_BLANK_", "│", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "└", "→", "_A04_", "→", "_A06_", "─", "─", "┘", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_"},
        {"_S01_", "┤", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_"},
        {"_BLANK_", "│", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "┌", "→", "_B04_", "→", "_B06_", "→", "_B08_", "┐", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_"},
        {"_BLANK_", "├", "→", "_B01_", "→", "_B02_", "→", "_B03_", "┤", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "├", "→", "_B10_", "→", "_B11_", "→", "_B12_"},
        {"_BLANK_", "│", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "└", "→", "_B05_", "→", "_B07_", "→", "_B09_", "┘", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_"},
        {"_S02_", "┤", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_"},
        {"_BLANK_", "│", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "┌", "→", "_C04_", "→", "_C07_", "→", "_C08_", "┐", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_"},
        {"_BLANK_", "└", "→", "_C01_", "→", "_C02_", "→", "_C03_", "┼", "→", "_C05_", "─", "─", "─", "─", "┼", "→", "_C09_", "→", "_C10_", "→", "_C11_"},
        {"_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "└", "→", "_C06_", "─", "─", "─", "─", "┘", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_", "_BLANK_"},
    };
    return blueprint;
}