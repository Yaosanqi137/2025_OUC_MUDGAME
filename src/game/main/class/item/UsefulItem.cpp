#include "UsefulItem.h"
#include <stdexcept>

// 私有构造函数
UsefulItem::UsefulItem(ItemType type, const std::string& name, const std::string& intro, int price, int durability, bool isUsable)
    : AbstractItem(name, intro), type_(type), price_(price), durability_(durability) {
    price_ = price;
    durability_ = durability;
    isUsable_ = isUsable;
}

// 公共构造函数
UsefulItem::UsefulItem(ItemType type) {
    switch (type) {
        case ItemType::BOXING_GLOVES:
            *this = createBoxingGloves();
            break;
        case ItemType::BOXING_GYM_PASS:
            *this = createBoxingGymPass();
            break;
        case ItemType::CARD:
            *this = createCard();
            break;
        default:
            throw std::invalid_argument("Unknown useful item type");
    }
}

int UsefulItem::getPrice() const {
    return price_;
}

void UsefulItem::use(Player& user) {
    // 在这里实现 use 的具体逻辑
    // 例如，减少耐久度。如果耐久度降为0，可以从玩家背包中移除
    // 注意：这里的 user 可能是为了将来的扩展，比如某些道具有特殊效果
    decreaseDurability();
    if (amount_ > 0) {
        amount_--;
    }
}

UsefulItem::ItemType UsefulItem::getItemType() const {
    return type_;
}

int UsefulItem::getDurability() const {
    return durability_;
}


// 静态工厂方法
UsefulItem UsefulItem::createBoxingGloves(bool isUsable) {
    return {ItemType::BOXING_GLOVES, "拳套", "一副基础的拳击手套，提供了基础的保护。", 50, 10, isUsable};
}

UsefulItem UsefulItem::createBoxingGymPass(bool isUsable) {
    return {ItemType::BOXING_GYM_PASS, "拳击馆通行证", "一张可以在拳击馆使用的通行证。", 500, 15, isUsable};
}

UsefulItem UsefulItem::createCard(bool isUsable) {
    return {ItemType::CARD, "弗兰克的名片", "一张闪闪发光的名片，上面写着弗兰克，拳击手经纪人", 5, 1, isUsable};
}

// 辅助函数，减少耐久度
bool UsefulItem::decreaseDurability() {
    if (durability_ > 0) {
        durability_--;
    }
    return durability_ > 0;
}