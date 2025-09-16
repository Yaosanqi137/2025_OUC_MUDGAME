#ifndef SHOP_LAYOUT_H
#define SHOP_LAYOUT_H

#include "ftxui/component/component.hpp"
#include <vector>
#include <memory>
#include <array>

class Game;
class Player;
class UsefulItem;

struct ShopItem {
    std::string name;
    std::string description;
    int price;
    std::string itemType;
};

class ShopLayout : public ftxui::ComponentBase {
public:
    explicit ShopLayout(Game& game_logic, bool& isShowingFlag);

    ftxui::Element OnRender() override;

private:
    void initializeShopItems();
    void purchaseItem(int itemIndex);
    [[nodiscard]] int getTotalPages() const;

    Game& game_logic_;
    Player& player_;
    bool& isShowingFlag_;

    // 商店物品数据
    std::vector<ShopItem> shopItems_;
    int selectedItemIndex_ = -1;
    int currentPage_ = 0;

    // UI组件
    static constexpr int itemsPerPage_ = 6;
    std::array<ftxui::Component, itemsPerPage_> itemButtons_;
    ftxui::Component exitButton_;
    ftxui::Component buyButton_;
    ftxui::Component pagePrevButton_;
    ftxui::Component pageNextButton_;
    ftxui::Component mainContainer_;
};

#endif // SHOP_LAYOUT_H