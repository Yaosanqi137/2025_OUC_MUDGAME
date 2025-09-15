#ifndef PHARMACYSCENE_H
#define PHARMACYSCENE_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"
#include "../../class/item/Medicine.h"

namespace PharmacyScene {
    // 药店购买对话节点ID范围: 11000000-11999999

    // 药店老板欢迎语
    inline const DialogNode _11000001(11000001, "药剂师", "欢迎来到药店！这里有各种神奇的药品，可以提升你的能力，但要小心副作用哦！", {
        Choice("回生丹 - 50元", 11000002, [](Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 50) {
                game_logic_.getPlayer().addSavings(-50);
                Medicine revivalPill(Medicine::MedicineType::REVIVAL_PILL);
                revivalPill.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(revivalPill));
                game_logic_.getDialog().addMessage("<SYSTEM>", "购买成功！获得了回生丹");
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "金钱不足！需要50元");
            }
        }),
        Choice("力量药水 - 200元", 11000003, [](Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 200) {
                game_logic_.getPlayer().addSavings(-200);
                Medicine strengthPotion(Medicine::MedicineType::STRENGTH_POTION);
                strengthPotion.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(strengthPotion));
                game_logic_.getDialog().addMessage("<SYSTEM>", "购买成功！获得了力量药水");
                game_logic_.getDialog().addMessage("<SYSTEM>", "注意：此药水有副作用！");
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "金钱不足！需要200元");
            }
        }),
        Choice("敏捷药水 - 200元", 11000004, [](Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 200) {
                game_logic_.getPlayer().addSavings(-200);
                Medicine agilityPotion(Medicine::MedicineType::AGILITY_POTION);
                agilityPotion.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(agilityPotion));
                game_logic_.getDialog().addMessage("<SYSTEM>", "购买成功！获得了敏捷药水");
                game_logic_.getDialog().addMessage("<SYSTEM>", "注意：此药水有副作用！");
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "金钱不足！需要200元");
            }
        }),
        Choice("耐力药水 - 200元", 11000005, [](Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 200) {
                game_logic_.getPlayer().addSavings(-200);
                Medicine staminaPotion(Medicine::MedicineType::STAMINA_POTION);
                staminaPotion.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(staminaPotion));
                game_logic_.getDialog().addMessage("<SYSTEM>", "购买成功！获得了耐力药水");
                game_logic_.getDialog().addMessage("<SYSTEM>", "注意：此药水有副作用！");
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "金钱不足！需要200元");
            }
        }),
        Choice("技能点药水 - 100元", 11000006, [](Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 100) {
                game_logic_.getPlayer().addSavings(-100);
                Medicine skillPotion(Medicine::MedicineType::SKILL_POINT_POTION);
                skillPotion.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(skillPotion));
                game_logic_.getDialog().addMessage("<SYSTEM>", "购买成功！获得了技能点药水");
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "金钱不足！需要100元");
            }
        }),
        Choice("退出", 0, [](Game& game_logic_) {
            game_logic_.getDialog().addMessage("药剂师", "记住，是药三分毒，适量使用！");
        })
    });

    /**
     * @brief 将药店场景的所有对话节点注册到数据库中。
     * @param db 对话数据库的引用。
     */
    inline void registerAllDialogs(std::map<unsigned int, const DialogNode*>& db) {
        db[11000001] = &_11000001;
    }

    /// @brief 创建一个静态的注册器实例，以实现自动注册。
    inline DialogRegistrar registrar(registerAllDialogs);

} // namespace PharmacyScene

#endif // PHARMACYSCENE_H
