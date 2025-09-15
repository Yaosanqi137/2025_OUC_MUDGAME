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

bool UsefulItem::use(Player& user) {
    // 检查物品是否可用
    if (amount_ <= 0) {
        return false;
    }

    // 减少耐久度
    durability_--;

    // 当耐久度减到0时
    if (durability_ <= 0) {
        if (amount_ > 0) {
            // 减少一个数量
            amount_--;

            // 如果还有剩余数量，重置耐久度到初始值
            if (amount_ > 0) {
                // 根据物品类型重置耐久度到初始值
                switch (type_) {
                    case ItemType::BOXING_GLOVES:
                        durability_ = 10;
                        break;
                    case ItemType::BOXING_GYM_PASS:
                        durability_ = 15;
                        break;
                    case ItemType::CARD:
                        durability_ = 1;
                        break;
                }
                return true;
            } else {
                // amount为0，物品已用完
                durability_ = 0;
                return false;
            }
        } else {
            // amount为0，重置durability并返回false
            durability_ = 0;
            return false;
        }
    }

    // 正常使用，耐久度未到0
    return true;
}

UsefulItem::ItemType UsefulItem::getItemType() const {
    return type_;
}

int UsefulItem::getDurability() const {
    return durability_;
}

const std::string& UsefulItem::getIntro() const {
    if (type_ == ItemType::BOXING_GYM_PASS) {
        // 为拳击馆通行证动态生成描述
        static std::string dynamicIntro;
        dynamicIntro = "一张可以在拳击馆使用的通行证。一张能用15次，没了这个，就进不去拳击馆了，现在还剩" + std::to_string(getDurability()) + "次使用次数";
        return dynamicIntro;
    }
    return AbstractItem::getIntro();
}

// 静态工厂方法
UsefulItem UsefulItem::createBoxingGloves(bool isUsable) {
    return {ItemType::BOXING_GLOVES, "拳套", "一副基础的拳击手套，提供了基础的保护。", 50, 10, isUsable};
}

UsefulItem UsefulItem::createBoxingGymPass(bool isUsable) {
    return {ItemType::BOXING_GYM_PASS, "拳击馆通行证", "", 500, 15, isUsable};
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