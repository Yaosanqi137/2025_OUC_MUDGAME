#ifndef SHOPSCENE_H
#define SHOPSCENE_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"
#include "../../class/item/Food.h"

namespace ShopScene {
    // 商店购买对话节点ID范围: 10000000-10999999

    // 烤肉购买成功对话
    inline const DialogNode _10000010(10000010, "老板", "上好的即食烤肉（预制菜）");

    // 烤肉钱不够对话
    inline const DialogNode _10000012(10000012, "老板", "客官，您的钱不够啊！");

    // 苏打水购买成功对话
    inline const DialogNode _10000020(10000020, "老板", "这个苏打水喝了延年益寿啊");

    // 苏打水钱不够对话
    inline const DialogNode _10000022(10000022, "老板", "客官，您的钱不够啊！");

    // 能量饮料购买成功对话
    inline const DialogNode _10000030(10000030, "老板", "累了，困了，喝？");

    // 能量饮料钱不够对话
    inline const DialogNode _10000032(10000032, "老板", "客官，您的钱不够啊！");

    // 冷冻披萨购买成功对话
    inline const DialogNode _10000040(10000040, "老板", "方便快捷的冷冻披萨");

    // 冷冻披萨钱不够对话
    inline const DialogNode _10000042(10000042, "老板", "客官，您的钱不够啊！");

    // 退出对话
    inline const DialogNode _10000050(10000050, "老板", "欢迎下次光临！");

    // 商店老板欢迎语
    inline const DialogNode _10000001(10000001, "老板", "欢迎光临！我们这里有新鲜的食物和饮料，看看有什么需要的吧！", {
        Choice("烤肉 - 20元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 20) {
                game_logic_.getPlayer().addSavings(-20);
                Food meat(Food::FoodType::MEAT);
                meat.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(meat));
                game_logic_.getStoryController().processNodeByID(10000010);
            } else {
                game_logic_.getStoryController().processNodeByID(10000012);
            }
        }),
        Choice("苏打水 - 6元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 6) {
                game_logic_.getPlayer().addSavings(-6);
                Food soda(Food::FoodType::SODA);
                soda.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(soda));
                game_logic_.getStoryController().processNodeByID(10000020);
            } else {
                game_logic_.getStoryController().processNodeByID(10000022);
            }
        }),
        Choice("能量饮料 - 14元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 14) {
                game_logic_.getPlayer().addSavings(-14);
                Food energyDrink(Food::FoodType::ENERGY_DRINK);
                energyDrink.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(energyDrink));
                game_logic_.getStoryController().processNodeByID(10000030);
            } else {
                game_logic_.getStoryController().processNodeByID(10000032);
            }
        }),
        Choice("冷冻披萨 - 9元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 9) {
                game_logic_.getPlayer().addSavings(-9);
                Food pizza(Food::FoodType::FROZEN_PIZZA);
                pizza.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(pizza));
                game_logic_.getStoryController().processNodeByID(10000040);
            } else {
                game_logic_.getStoryController().processNodeByID(10000042);
            }
        }),
        Choice("退出", 0, [](const Game& game_logic_) {
            game_logic_.getStoryController().processNodeByID(10000050);
        })
    });

    /**
     * @brief 将商店场景的所有对话节点注册到数据库中。
     * @param db 对话数据库的引用。
     */
    inline void registerAllDialogs(std::map<unsigned int, const DialogNode*>& db) {
        db[10000001] = &_10000001;
        db[10000010] = &_10000010;
        db[10000012] = &_10000012;
        db[10000020] = &_10000020;
        db[10000022] = &_10000022;
        db[10000030] = &_10000030;
        db[10000032] = &_10000032;
        db[10000040] = &_10000040;
        db[10000042] = &_10000042;
        db[10000050] = &_10000050;
    }

    /// @brief 创建一个静态的注册器实例，以实现自动注册。
    inline DialogRegistrar registrar(registerAllDialogs);

} // namespace ShopScene

#endif // SHOPSCENE_H
