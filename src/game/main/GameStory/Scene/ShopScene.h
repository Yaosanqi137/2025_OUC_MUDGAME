#ifndef SHOPSCENE_H
#define SHOPSCENE_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"
#include "../../class/item/Food.h"

namespace ShopScene {
    // 商店购买对话节点ID范围: 10000000-10999999

    // 商店老板欢迎语
    inline const DialogNode _10000001(10000001, "老板", "欢迎光临！我们这里有新鲜的食物和饮料，看看有什么需要的吧！", {
        Choice("烤肉 - 20元", 10000002, [](Game& game) {
            if (game.getPlayer().getSavings() >= 20) {
                game.getPlayer().addSavings(-20);
                Food meat(Food::FoodType::MEAT);
                meat.setAmount(1);
                game.getPlayer().addItem(std::make_shared<Food>(meat));
                game.getDialog().addMessage("老板", "上好的即食烤肉（预制菜）");
            } else {
                game.getDialog().addMessage("老板", "客官，您的钱不够啊！");
            }
        }),
        Choice("苏打水 - 6元", 10000003, [](Game& game) {
            if (game.getPlayer().getSavings() >= 6) {
                game.getPlayer().addSavings(-6);
                Food soda(Food::FoodType::SODA);
                soda.setAmount(1);
                game.getPlayer().addItem(std::make_shared<Food>(soda));
                game.getDialog().addMessage("老板", "这个苏打水喝了延年益寿啊");
            } else {
                game.getDialog().addMessage("老板", "客官，您的钱不够啊！");
            }
        }),
        Choice("能量饮料 - 14元", 10000004, [](Game& game) {
            if (game.getPlayer().getSavings() >= 14) {
                game.getPlayer().addSavings(-14);
                Food energyDrink(Food::FoodType::ENERGY_DRINK);
                energyDrink.setAmount(1);
                game.getPlayer().addItem(std::make_shared<Food>(energyDrink));
                game.getDialog().addMessage("老板", "累了，困了，喝？");
            } else {
                game.getDialog().addMessage("老板", "客官，您的钱不够啊！");
            }
        }),
        Choice("冷冻披萨 - 9元", 10000005, [](Game& game) {
            if (game.getPlayer().getSavings() >= 9) {
                game.getPlayer().addSavings(-9);
                Food pizza(Food::FoodType::FROZEN_PIZZA);
                pizza.setAmount(1);
                game.getPlayer().addItem(std::make_shared<Food>(pizza));
                game.getDialog().addMessage("老板", "方便快捷的冷冻披萨");
            } else {
                game.getDialog().addMessage("老板", "客官，您的钱不够啊！");
            }
        }),
        Choice("退出", 0, [](Game& game) {
            game.getDialog().addMessage("老板", "欢迎下次光临！");
        })
    });

    /**
     * @brief 将商店场景的所有对话节点注册到数据库中。
     * @param db 对话数据库的引用。
     */
    inline void registerAllDialogs(std::map<unsigned int, const DialogNode*>& db) {
        db[10000001] = &_10000001;
    }

    /// @brief 创建一个静态的注册器实例，以实现自动注册。
    inline DialogRegistrar registrar(registerAllDialogs);

} // namespace ShopScene

#endif // SHOPSCENE_H
