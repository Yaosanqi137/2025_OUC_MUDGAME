#include "BagLayout.h"
#include "../Game.h"
#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"
#include <algorithm>
#include <string>
#include <vector>

using namespace ftxui;

BagLayout::BagLayout(Game& game_logic) : game_logic_(game_logic) {
    initializeItems();

    // --- 在构造函数中创建所有持久化组件 ---
    
    // 创建一个临时列表，用于收集所有需要交互的组件
    std::vector<Component> interactive_components;

    // 1. 初始化物品按钮 (10个)
    for (int i = 0; i < itemsPerPage_; ++i) {
        auto on_click = [this, i] {
            int globalIndex = i + currentPage_ * itemsPerPage_;
            if (globalIndex < static_cast<int>(items_.size())) {
                selectedItemIndex_ = globalIndex;
            }
        };

        auto option = ButtonOption();
        option.transform = [this, i](const EntryState& s) {
            int globalIndex = i + currentPage_ * itemsPerPage_;
            if (globalIndex >= static_cast<int>(items_.size())) {
                return filler();
            }

            const auto& item = items_[globalIndex];
            auto element = vbox({
                text(item->icon) | center | bold,
                text(item->name) | center | size(WIDTH, LESS_THAN, 10),
                text("x" + std::to_string(item->amount)) | center | color(Color::Green)
            });

            if (s.active) {
                element |= bold;
            }
            return element;
        };

        itemButtons_[i] = Button("", on_click, option);
        interactive_components.push_back(itemButtons_[i]);
    }

    // 2. 初始化其他控制按钮
    exitButton_ = Button(" [ 退 出 ] ", [this] { hide(); });
    pagePrevButton_ = Button(" [ 向左翻页 ] ", [this] {
        if (currentPage_ > 0) {
            currentPage_--;
            selectedItemIndex_ = -1;
        }
    });
    pageNextButton_ = Button(" [ 向右翻页 ] ", [this] {
        if (currentPage_ < getTotalPages() - 1) {
            currentPage_++;
            selectedItemIndex_ = -1;
        }
    });

    interactive_components.push_back(exitButton_);
    interactive_components.push_back(pagePrevButton_);
    interactive_components.push_back(pageNextButton_);

    mainContainer_ = Container::Vertical(interactive_components);

    // 将主容器作为 Bag 的唯一子组件添加。
    Add(mainContainer_);
}

void BagLayout::initializeItems() {
    // 示例物品
    items_ = {};
        // TODO: 考虑放些初始物品
}

int BagLayout::getTotalPages() const {
    if (items_.empty()) return 1;
    return (items_.size() + itemsPerPage_ - 1) / itemsPerPage_;
}

Element BagLayout::Render() {
    if (!isShowing_) {
        return text("");
    }

    if (items_.empty()) {
        auto empty_bag_view = vbox({
            filler(),
            text("背包中空空如也") | bold | hcenter,
            separator(),
            exitButton_->Render() | hcenter,
            filler()
        }) | flex;
        return window(text(" 背包 ") | bold, empty_bag_view) | clear_under;
    }

    // Render函数只负责“布局”，不负责“创建”组件

    Elements grid_rows;
    for (int r = 0; r < 5; ++r) {
        Elements row_elements;
        for (int c = 0; c < 2; ++c) {
            int index_on_page = r * 2 + c;
            int global_index = index_on_page + currentPage_ * itemsPerPage_;

            auto item_box = itemButtons_[index_on_page]->Render() |
                            size(WIDTH, EQUAL, 12) |
                            size(HEIGHT, EQUAL, 6) |
                            border;

            if (global_index >= static_cast<int>(items_.size())) {
                item_box |= color(Color::GrayDark);
            } else if (global_index == selectedItemIndex_) {
                item_box |= color(Color::GreenLight);
            }
            row_elements.push_back(item_box);
        }
        grid_rows.push_back(hbox(std::move(row_elements)));
    }
    auto itemGrid = vbox(std::move(grid_rows));

    std::string itemDetailName = "请选择一个物品查看详情";
    std::string itemDetailDesc;
    std::string itemDetailAmount;
    std::string itemDetailClass;
    if (selectedItemIndex_ >= 0 && selectedItemIndex_ < static_cast<int>(items_.size())) {
        const auto& item = items_[selectedItemIndex_];
        itemDetailName = "名称: " + item->name;
        itemDetailDesc = "描述: " + item->description;
        itemDetailAmount = "数量: " + std::to_string(item->amount);
        itemDetailClass = "类型: " + std::string(item->type == 0 ? "普通物品" : (item->type == 1 ? "食物" : "药品"));
    }
    auto detailPanel = vbox({
        text("物品详情") | bold | center,
        separator(),
        paragraph(itemDetailName) | vscroll_indicator | frame | flex,
        paragraph(itemDetailDesc) | vscroll_indicator | frame | flex,
        paragraph(itemDetailAmount) | vscroll_indicator | frame | flex,
        paragraph(itemDetailClass) | vscroll_indicator | frame | flex,
    }) | border;

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

    auto mainLayout = vbox({
        hbox({itemGrid | center, separator(), detailPanel | flex}) | flex,
        separator(),
        controlPanel
    });

    return window(text(" 背包 ") | bold, mainLayout) | clear_under;
}

void BagLayout::show() {
    isShowing_ = true;
    selectedItemIndex_ = -1;
    currentPage_ = 0;
}

void BagLayout::hide() {
    isShowing_ = false;
}

bool BagLayout::isShowing() const {
    return isShowing_;
}

void BagLayout::setItemAmount(const int amount, Item* item) {
    auto index = std::ranges::find(items_.begin(), items_.end(), item);
    if (index == items_.end()) {
        items_.push_back(item);
        index = items_.end() - 1;
    }
        (*index)->amount = amount;
        if ((*index)->amount > 7) {
            (*index)->amount = 7;
        } else if ((*index)->amount < 0) {
            (*index)->amount = 0;
        }
}