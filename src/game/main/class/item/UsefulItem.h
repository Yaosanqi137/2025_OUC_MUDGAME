#ifndef USEFULITEM_H
#define USEFULITEM_H

#include "AbstractItem.h"
#include <string>

class Player;

class UsefulItem : public AbstractItem {
public:
    // 枚举物品类型
    enum class ItemType {
        BOXING_GLOVES,      // 拳套
        BOXING_GYM_PASS     // 拳击馆通行证
    };

    UsefulItem(ItemType type);
    ~UsefulItem() override = default;

    int getPrice() const override;
    void use(Player& user) override; // 注意：use方法现在需要一个Player引用

    ItemType getItemType() const;
    int getDurability() const;

    static UsefulItem createBoxingGloves();
    static UsefulItem createBoxingGymPass();

private:
    ItemType type_;
    int price_;
    int durability_;

    UsefulItem(ItemType type, const std::string& name, const std::string& intro, int price, int durability);

    bool decreaseDurability();
};

#endif // USEFULITEM_H