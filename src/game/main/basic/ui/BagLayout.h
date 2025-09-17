#ifndef BAGLAYOUT_H
#define BAGLAYOUT_H

#include <ftxui/component/component.hpp>
#include <vector>
#include <string>
#include <array>
#include <memory>

class Game;
class Player;
class AbstractItem;

class BagLayout : public ftxui::ComponentBase {
public:
    explicit BagLayout(Game& game_logic, bool& isShowingFlag, std::vector<std::shared_ptr<AbstractItem>>& displayableItems);

    ftxui::Element OnRender() override;

private:
    void refreshItems(); // 从Player获取最新的物品列表
    [[nodiscard]] int getTotalPages() const;
    [[nodiscard]] std::string getItemTypeString(const std::shared_ptr<AbstractItem>& item) const;

    Game& game_logic_;
    Player& player_;
    bool& isShowingFlag_;

    // --- 物品数据 ---
    std::vector<std::shared_ptr<AbstractItem>>& displayableItems_; // 从Player获取的可显示物品
    int selectedItemIndex_ = -1;
    int currentPage_ = 0;

    // --- UI组件作为成员变量，保证其生命周期 ---
    static constexpr int itemsPerPage_ = 10;
    std::array<ftxui::Component, itemsPerPage_> itemButtons_;
    ftxui::Component exitButton_;
    ftxui::Component pagePrevButton_;
    ftxui::Component pageNextButton_;
    ftxui::Component useButton_;  // 新增使用物品按钮

    // 使用一个主容器来统一管理所有可交互的子组件
    ftxui::Component mainContainer_;
};

#endif // BAGLAYOUT_H