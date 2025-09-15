#ifndef PHARMACYSCENE_H
#define PHARMACYSCENE_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"
#include "../../class/item/Medicine.h"

namespace PharmacyScene {
    // 药店购买对话节点ID范围: 11000000-11999999

    // 回生丹购买成功对话
    inline const DialogNode _11000010(11000010, "药店小姐", "回生丹紧急的时候很好用");

    // 回生丹钱不够对话
    inline const DialogNode _11000012(11000012, "药店小姐", "金钱不足！需要50元");

    // 力量药水购买成功对话
    inline const DialogNode _11000020(11000020, "药店小姐", "购买成功！获得了力量药水", {}, 11000021);
    inline const DialogNode _11000021(11000021, "药店小姐", "注意：此药水有副作用！");

    // 力量药水钱不够对话
    inline const DialogNode _11000022(11000022, "药店小姐", "金钱不足！需要200元");

    // 敏捷药水购买成功对话
    inline const DialogNode _11000030(11000030, "药店小姐", "购买成功！获得了敏捷药水", {}, 11000031);
    inline const DialogNode _11000031(11000031, "药店小姐", "注意：此药水有副作用！");

    // 敏捷药水钱不够对话
    inline const DialogNode _11000032(11000032, "药店小姐", "金钱不足！需要200元");

    // 耐力药水购买成功对话
    inline const DialogNode _11000040(11000040, "药店小姐", "购买成功！获得了耐力药水", {}, 11000041);
    inline const DialogNode _11000041(11000041, "药店小姐", "注意：此药水有副作用！");

    // 耐力药水钱不够对话
    inline const DialogNode _11000042(11000042, "药店小姐", "金钱不足！需要200元");

    // 技能点药水购买成功对话
    inline const DialogNode _11000050(11000050, "药店小姐", "购买成功！获得了技能点药水");

    // 技能点药水钱不够对话
    inline const DialogNode _11000052(11000052, "药店小姐", "金钱不足！需要100元");

    // 退出对话
    inline const DialogNode _11000060(11000060, "药店小姐", "记住，是药三分毒，适量使用！");

    // 药店老板欢迎语
    inline const DialogNode _11000001(11000001, "药店小姐", "欢迎来到药店！这里有各种神奇的药品，可以提升你的能力，但要小心副作用哦！", {
        Choice("回生丹 - 50元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 50) {
                game_logic_.getPlayer().addSavings(-50);
                Medicine revivalPill(Medicine::MedicineType::REVIVAL_PILL);
                revivalPill.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(revivalPill));
                game_logic_.getStoryController().processNodeByID(11000010);
            } else {
                game_logic_.getStoryController().processNodeByID(11000012);
            }
        }),
        Choice("力量药�� - 200元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 200) {
                game_logic_.getPlayer().addSavings(-200);
                Medicine strengthPotion(Medicine::MedicineType::STRENGTH_POTION);
                strengthPotion.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(strengthPotion));
                game_logic_.getStoryController().processNodeByID(11000020);
            } else {
                game_logic_.getStoryController().processNodeByID(11000022);
            }
        }),
        Choice("敏捷药水 - 200元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 200) {
                game_logic_.getPlayer().addSavings(-200);
                Medicine agilityPotion(Medicine::MedicineType::AGILITY_POTION);
                agilityPotion.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(agilityPotion));
                game_logic_.getStoryController().processNodeByID(11000030);
            } else {
                game_logic_.getStoryController().processNodeByID(11000032);
            }
        }),
        Choice("耐力药水 - 200元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 200) {
                game_logic_.getPlayer().addSavings(-200);
                Medicine staminaPotion(Medicine::MedicineType::STAMINA_POTION);
                staminaPotion.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(staminaPotion));
                game_logic_.getStoryController().processNodeByID(11000040);
            } else {
                game_logic_.getStoryController().processNodeByID(11000042);
            }
        }),
        Choice("技能点药水 - 100元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 100) {
                game_logic_.getPlayer().addSavings(-100);
                Medicine skillPotion(Medicine::MedicineType::SKILL_POINT_POTION);
                skillPotion.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Medicine>(skillPotion));
                game_logic_.getStoryController().processNodeByID(11000050);
            } else {
                game_logic_.getStoryController().processNodeByID(11000052);
            }
        }),
        Choice("退出", 0, [](const Game& game_logic_) {
            game_logic_.getStoryController().processNodeByID(11000060);
        })
    });

    /**
     * @brief 将药店场景的所有对话节点注册到数据库中。
     * @param db 对话数据库的引用。
     */
    inline void registerAllDialogs(std::map<unsigned int, const DialogNode*>& db) {
        db[11000001] = &_11000001;
        db[11000010] = &_11000010;
        db[11000012] = &_11000012;
        db[11000020] = &_11000020;
        db[11000021] = &_11000021;
        db[11000022] = &_11000022;
        db[11000030] = &_11000030;
        db[11000031] = &_11000031;
        db[11000032] = &_11000032;
        db[11000040] = &_11000040;
        db[11000041] = &_11000041;
        db[11000042] = &_11000042;
        db[11000050] = &_11000050;
        db[11000052] = &_11000052;
        db[11000060] = &_11000060;
    }

    /// @brief 创建一个静态的注册器实例，以实现自动注册。
    inline DialogRegistrar registrar(registerAllDialogs);

} // namespace PharmacyScene

#endif // PHARMACYSCENE_H
