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
        BOXING_GYM_PASS,    // 拳击馆通行证
        CARD,               // 弗兰克的名片
        BANNED_DRUG         // 禁药
    };

    UsefulItem(ItemType type);
    ~UsefulItem() override = default;

    int getPrice() const override;
    bool use(Player& user) override;
    const std::string& getIntro() const override;
    ItemType getItemType() const;
    int getDurability() const;
    void setDurability(int value);

    static UsefulItem createBoxingGloves(bool isUsable = false);
    static UsefulItem createBoxingGymPass(bool isUsable = false);
    static UsefulItem createCard(bool isUsable = false);
    static UsefulItem createBannedDrug(bool isUsable = true);

private:
    ItemType type_;
    int price_;
    int durability_;
    UsefulItem(ItemType type, const std::string& name, const std::string& intro, int price, int durability, bool isUsable);

    bool decreaseDurability();
};

#endif // USEFULITEM_H