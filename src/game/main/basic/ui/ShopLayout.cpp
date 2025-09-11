#include "ShopLayout.h"
#include "../Game.h"
#include "../../class/entity/Player.h"
#include "../../class/item/UsefulItem.h"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"
#include <sstream>
#include <iomanip>

using namespace ftxui;

ShopLayout::ShopLayout(Game& game_logic)
    : game_logic_(game_logic), player_(game_logic.getPlayer()) {

    // 初始化商店物品
    initializeShopItems();

    // 创建物品按钮
    std::vector<Component> interactive_components;

    for (int i = 0; i < itemsPerPage_; ++i) {
        auto on_click = [this, i] {
            int globalIndex = i + currentPage_ * itemsPerPage_;
            if (globalIndex < static_cast<int>(shopItems_.size())) {
                selectedItemIndex_ = globalIndex;
            }
        };

        auto option = ButtonOption();
        option.transform = [this, i](const EntryState& s) {
            int globalIndex = i + currentPage_ * itemsPerPage_;
            if (globalIndex >= static_cast<int>(shopItems_.size())) {
                return filler();
            }

            const auto& item = shopItems_[globalIndex];
            auto element = vbox({
                text("🛒") | center | bold,
                text(item.name) | center | size(WIDTH, LESS_THAN, 12),
                text("💰 " + std::to_string(item.price) + "元") | center | color(Color::Yellow)
            });

            if (s.active) {
                element |= bold;
            }
            if (globalIndex == selectedItemIndex_) {
                element |= color(Color::GreenLight);
            }
            return element;
        };

        itemButtons_[i] = Button("", on_click, option);
        interactive_components.push_back(itemButtons_[i]);
    }

    // 创建控制按钮
    exitButton_ = Button("返回", [this] {
        hide();
    });
    buyButton_ = Button("购买", [this] {
        if (selectedItemIndex_ >= 0 && selectedItemIndex_ < static_cast<int>(shopItems_.size())) {
            purchaseItem(selectedItemIndex_);
        }
    });

    pagePrevButton_ = Button("上一页", [this] {
        if (currentPage_ > 0) {
            currentPage_--;
            selectedItemIndex_ = -1;
        }
    });

    pageNextButton_ = Button("下一页", [this] {
        if (currentPage_ < getTotalPages() - 1) {
            currentPage_++;
            selectedItemIndex_ = -1;
        }
    });

    interactive_components.push_back(exitButton_);
    interactive_components.push_back(buyButton_);
    interactive_components.push_back(pagePrevButton_);
    interactive_components.push_back(pageNextButton_);

    // 创建主容器
    mainContainer_ = Container::Vertical(interactive_components);

    // 添加到组件
    Add(mainContainer_);
}

void ShopLayout::initializeShopItems() {
    // 添加UsefulItem中的所有物品
    shopItems_ = {
        {"拳套", "一副基础的拳击手套，提供了基础的保护。", 50, "拳套"},
        {"拳击馆通行证", "一张可以在拳击馆使用的通行证。", 500, "拳击馆通行证"}
    };
}

void ShopLayout::purchaseItem(int itemIndex) {
    if (itemIndex < 0 || itemIndex >= static_cast<int>(shopItems_.size())) {
        return;
    }

    const auto& item = shopItems_[itemIndex];

    // 检查玩家是否有足够金钱
    if (player_.getSavings() >= item.price) {
        // 扣除金钱
        player_.addSavings(-item.price);

        // 添加物品到背包
        player_.addItemByType(item.itemType, 1);

        // 可以在这里添加购买成功的提示
        // 暂时通过重置选择来表示购买完成
        selectedItemIndex_ = -1;
    } else {
        // 金钱不足，暂时也重置选择
        selectedItemIndex_ = -1;
    }
}

int ShopLayout::getTotalPages() const {
    if (shopItems_.empty()) return 1;
    return (shopItems_.size() + itemsPerPage_ - 1) / itemsPerPage_;
}

Element ShopLayout::Render() {
    if (!isShowing_) {
        return text("");
    }

    if (shopItems_.empty()) {
        auto empty_shop_view = vbox({
            filler(),
            text("商店暂时没有商品") | bold | hcenter,
            separator(),
            exitButton_->Render() | hcenter,
            filler()
        }) | flex;
        return window(text(" 网购平台 ") | bold, empty_shop_view) | clear_under;
    }

    // 商品网格布局（2列3行）
    Elements grid_rows;
    for (int r = 0; r < 3; ++r) {
        Elements row_elements;
        for (int c = 0; c < 2; ++c) {
            int index_on_page = r * 2 + c;
            int global_index = index_on_page + currentPage_ * itemsPerPage_;

            auto item_box = itemButtons_[index_on_page]->Render() |
                            size(WIDTH, EQUAL, 15) |
                            size(HEIGHT, EQUAL, 5) |
                            border;

            if (global_index >= static_cast<int>(shopItems_.size())) {
                item_box |= color(Color::GrayDark);
            } else if (global_index == selectedItemIndex_) {
                item_box |= color(Color::GreenLight);
            }
            row_elements.push_back(item_box);
        }
        grid_rows.push_back(hbox(std::move(row_elements)));
    }
    auto itemGrid = vbox(std::move(grid_rows));

    // 商品详情面板
    std::string itemDetailName = "请选择一个商品查看详情";
    std::string itemDetailDesc;
    std::string itemDetailPrice;
    std::string playerMoney = "您的金钱: " + std::to_string(static_cast<int>(player_.getSavings())) + " 元";
    bool canBuy = false;

    if (selectedItemIndex_ >= 0 && selectedItemIndex_ < static_cast<int>(shopItems_.size())) {
        const auto& item = shopItems_[selectedItemIndex_];
        itemDetailName = "商品: " + item.name;
        itemDetailDesc = "描述: " + item.description;
        itemDetailPrice = "价格: " + std::to_string(item.price) + " 元";
        canBuy = player_.getSavings() >= item.price;
    }

    auto detailPanel = vbox({
        text("商品详情") | bold | center,
        separator(),
        paragraph(itemDetailName) | frame | flex,
        paragraph(itemDetailDesc) | frame | flex,
        paragraph(itemDetailPrice) | frame | flex | color(Color::Yellow),
        separator(),
        text(playerMoney) | center | color(Color::Cyan),
        separator(),
        (canBuy ? buyButton_->Render() : text("金钱不足") | color(Color::Red)) | center
    }) | border | size(WIDTH, EQUAL, 30);

    // 分页控制
    int totalPages = getTotalPages();
    auto pageInfo = text("第 " + std::to_string(currentPage_ + 1) + " 页 / 共 " + std::to_string(totalPages) + " 页");
    auto controlPanel = hbox({
        pagePrevButton_->Render(),
        filler(),
        pageInfo | center,
        filler(),
        pageNextButton_->Render(),
        separator(),
        exitButton_->Render()
    });

    // 主布局
    auto mainLayout = vbox({
        hbox({
            itemGrid | center,
            separator(),
            detailPanel | flex
        }) | flex,
        separator(),
        controlPanel
    });

    return window(text(" 🛒 网购平台 ") | bold, mainLayout) | clear_under;
}

void ShopLayout::show() {
    isShowing_ = true;
    selectedItemIndex_ = -1;
    currentPage_ = 0;
}

void ShopLayout::hide() {
    isShowing_ = false;
}

bool ShopLayout::isShowing() const {
    return isShowing_;
}
