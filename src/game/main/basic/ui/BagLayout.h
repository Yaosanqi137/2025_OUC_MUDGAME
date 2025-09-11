#ifndef BAGLAYOUT_H
#define BAGLAYOUT_H

#include "ftxui/component/component.hpp"
#include <vector>
#include <string>
#include <array>

// Forward declaration
class Game;

struct Item {
    std::string id;
    std::string name;
    std::string description;
    std::string icon;
    int amount;
    int type; // 0: 普通物品, 1: 食物, 2: 药品
};

class BagLayout : public ftxui::ComponentBase {
public:
    explicit BagLayout(Game& game_logic);

    ftxui::Element Render() override;
    void show();
    void hide();
    [[nodiscard]] bool isShowing() const;

    void setItemAmount(const int amount, Item* item);

private:
    void initializeItems();
    [[nodiscard]] int getTotalPages() const;

    Game& game_logic_;
    bool isShowing_ = false;

    // --- 物品数据 ---
    std::vector<Item*> items_;
    int selectedItemIndex_ = -1;
    int currentPage_ = 0;

    // --- UI组件作为成员变量，保证其生命周期 ---
    static constexpr int itemsPerPage_ = 10;
    std::array<ftxui::Component, itemsPerPage_> itemButtons_;
    ftxui::Component exitButton_;
    ftxui::Component pagePrevButton_;
    ftxui::Component pageNextButton_;

    // [核心修复] 使用一个主容器来统一管理所有可交互的子组件
    // 这是确保所有按钮都能接收事件的关键
    ftxui::Component mainContainer_;
};

#endif // BAGLAYOUT_H