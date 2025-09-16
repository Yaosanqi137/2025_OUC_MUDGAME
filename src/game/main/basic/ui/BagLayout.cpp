#include "BagLayout.h"
#include "../Game.h"
#include "../../class/entity/Player.h"
#include "../../class/item/AbstractItem.h"
#include "../../class/item/Food.h"
#include "../../class/item/UsefulItem.h"
#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <memory>

using namespace ftxui;

BagLayout::BagLayout(Game& game_logic, bool& isShowingFlag, std::vector<std::shared_ptr<AbstractItem>>& displayableItems)
    : game_logic_(game_logic), player_(game_logic.getPlayer()), isShowingFlag_(isShowingFlag), displayableItems_(displayableItems) {
    
    // 创建一个临时列表，用于收集所有需要交互的组件
    std::vector<Component> interactive_components;

    // 1. 初始化物品按钮 (10个)
    for (int i = 0; i < itemsPerPage_; ++i) {
        auto on_click = [this, i] {
            int globalIndex = i + currentPage_ * itemsPerPage_;
            if (globalIndex < static_cast<int>(displayableItems_.size())) {
                selectedItemIndex_ = globalIndex;
            }
        };

        auto option = ButtonOption();
        option.transform = [this, i](const EntryState& s) {
            int globalIndex = i + currentPage_ * itemsPerPage_;
            if (globalIndex >= static_cast<int>(displayableItems_.size())) {
                return filler();
            }

            const auto& item = displayableItems_[globalIndex];
            auto element = vbox({
                text("📦") | center | bold, // 使用通用图标
                text(item->getName()) | center | size(WIDTH, LESS_THAN, 10),
                text("x" + std::to_string(item->getAmount())) | center | color(Color::Green)
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
    exitButton_ = Button(" [ 退 出 ] ", [this] { isShowingFlag_ = false; selectedItemIndex_ = -1; currentPage_ = 0; });
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

    // 3. 使用物品按钮
    useButton_ = Button(" [ 使用物品 ] ", [this] {
        if (selectedItemIndex_ >= 0 && selectedItemIndex_ < static_cast<int>(displayableItems_.size())) {
            const auto& selectedItem = displayableItems_[selectedItemIndex_];
            // 判断物品是否可用
            if (selectedItem->isUsable()) {
                player_.useItem(selectedItem->getName());
                if (selectedItem->getName() == "禁药") {
                    player_.setExMaxHealth(20 + player_.getExMaxHealth());
                    player_.addHealth(10000);
                    player_.setExMaxFatigue(player_.getExMaxFatigue() + 20);
                    player_.addFatigue(10000);
                    game_logic_.getDialog().addMessage("<SYSTEM>", "你使用了禁药，最大生命值和体力提升了20点！");
                }
                refreshItems(); // 使用后刷新物品列表
                // 如果物品用完了，重置选择
                if (selectedItemIndex_ >= static_cast<int>(displayableItems_.size())) {
                    selectedItemIndex_ = -1;
                }
            }
        }
    });

    interactive_components.push_back(exitButton_);
    interactive_components.push_back(pagePrevButton_);
    interactive_components.push_back(pageNextButton_);
    interactive_components.push_back(useButton_);

    mainContainer_ = Container::Vertical(interactive_components);

    // 将主容器作为 Bag 的唯一子组件添加。
    Add(mainContainer_);
}

void BagLayout::refreshItems() {
    // 从Player获取可显示的物品（数量大于0的物品）
    displayableItems_ = player_.getDisplayableItems();
}

int BagLayout::getTotalPages() const {
    if (displayableItems_.empty()) return 1;
    return (displayableItems_.size() + itemsPerPage_ - 1) / itemsPerPage_;
}

std::string BagLayout::getItemTypeString(const std::shared_ptr<AbstractItem>& item) const {
    // 通过动态转换确定物品类型
    if (std::dynamic_pointer_cast<Food>(item)) {
        return "食物";
    } else if (std::dynamic_pointer_cast<UsefulItem>(item)) {
        return "道具";
    } else {
        return "物品";
    }
}

Element BagLayout::OnRender() {
    // 每次渲染前刷新物品列表
    refreshItems();

    if (displayableItems_.empty()) {
        auto empty_bag_view = vbox({
            filler(),
            text("背包中空空如也") | bold | hcenter,
            separator(),
            exitButton_->Render() | hcenter,
            filler()
        }) | flex;
        return window(text(" 背包 ") | bold, empty_bag_view) | clear_under;
    }

    // Render函数只负责"布局"，不负责"创建"组件

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

            if (global_index >= static_cast<int>(displayableItems_.size())) {
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
    bool canUseItem = false;
    bool isUsable = false;
    if (selectedItemIndex_ >= 0 && selectedItemIndex_ < static_cast<int>(displayableItems_.size())) {
        const auto& item = displayableItems_[selectedItemIndex_];
        itemDetailName = "名称: " + item->getName();
        itemDetailDesc = "描述: " + item->getIntro();
        itemDetailAmount = "数量: " + std::to_string(item->getAmount());
        itemDetailClass = "类型: " + getItemTypeString(item);
        canUseItem = item->getAmount() > 0;
        isUsable = item->isUsable();
    }

    auto detailPanel = vbox({
        text("物品详情") | bold | center,
        separator(),
        paragraph(itemDetailName) | size(WIDTH, LESS_THAN, 48) | flex,
        paragraph(itemDetailDesc) | size(WIDTH, EQUAL, 48) | flex,
        paragraph(itemDetailAmount) | size(WIDTH, LESS_THAN, 48) | flex,
        paragraph(itemDetailClass) | size(WIDTH, LESS_THAN, 48) | flex,
        separator(),
        (canUseItem && isUsable ? useButton_->Render() : text("无法使用") | color(Color::GrayDark)) | center
    }) | border | size(WIDTH, EQUAL, 50) | size(HEIGHT, GREATER_THAN, 20);

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
