#pragma once

#include "ftxui/component/component.hpp"
#include <vector>
#include <memory>
#include <array>
#include <functional>

// Forward declaration
class Game;
class Player;
class UsefulItem;

struct ShopItem {
    std::string name;
    std::string description;
    int price;
    std::string itemType; // 用于创建物品实例
};

class ShopLayout : public ftxui::ComponentBase {
public:
    explicit ShopLayout(Game& game_logic);

    ftxui::Element Render() override;
    void show();
    void hide();
    [[nodiscard]] bool isShowing() const;

private:
    void initializeShopItems();
    void purchaseItem(int itemIndex);
    [[nodiscard]] int getTotalPages() const;

    Game& game_logic_;
    Player& player_;
    bool isShowing_ = false;

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
