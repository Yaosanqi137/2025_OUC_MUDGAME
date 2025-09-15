#ifndef CAFESCENE_H
#define CAFESCENE_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"

namespace CafeScene {
    inline const DialogNode _12000001(12000001, "女仆", "菜单来咯，等一下让我来施展魔法哦",{
        Choice("女仆咖啡 - 60元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 60) {
                game_logic_.getPlayer().addSavings(-60);
                Food coffee(Food::FoodType::COFFEE);
                coffee.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(coffee));
                game_logic_.getDialog().addMessage("女仆", "好喝魔法变变变！希里希里，哗啦哗啦");
                game_logic_.getDialog().addMessage("女仆", "好喝的咖啡来了~");
            } else {
                game_logic_.getDialog().addMessage("女仆", "小哥哥，您身上的钱似乎不太够呢");
            }
        }),
        Choice("爱心面包 - 70元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 70) {
                game_logic_.getPlayer().addSavings(-70);
                Food bread(Food::FoodType::BREAD);
                bread.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(bread));
                game_logic_.getDialog().addMessage("女仆", "跟我一起念咒语：希里希里，哗啦哗啦");
                game_logic_.getDialog().addMessage("女仆", "好吃魔法变变变！爱心面包来了~");
            } else {
                game_logic_.getDialog().addMessage("女仆", "小哥哥，您身上的钱似乎不太够呢");
            }
        }),
        Choice("瓦学弟蛋包饭 - 150元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 150) {
                game_logic_.getPlayer().addSavings(-150);
                Food waxuedi(Food::FoodType::WAXUEDI);
                waxuedi.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(waxuedi));
                game_logic_.getDialog().addMessage("女仆", "极柔极柔，浓香浓香");
                game_logic_.getDialog().addMessage("女仆", "惊奇的心动魔法");
                game_logic_.getDialog().addMessage("女仆", "蛋包饭");
                game_logic_.getDialog().addMessage("女仆", "变得美味吧");
                game_logic_.getDialog().addMessage("", "仿佛真的有什么东西飘进蛋包饭里了");
                game_logic_.getDialog().addMessage("<PLAYER_NAME>", "番茄酱恰到好处的酸味");
                game_logic_.getDialog().addMessage("<PLAYER_NAME>", "和酱汁的甘甜融合为一");
                game_logic_.getDialog().addMessage("<PLAYER_NAME>", "摇身一变，成为暖心的家常风味，滑润顺口！");
            } else {
                game_logic_.getDialog().addMessage("女仆", "小哥哥，您身上的钱似乎不太够呢");
            }
        }),
        Choice("退出", 0, [](const Game& game_logic_) {
            game_logic_.getDialog().addMessage("女仆", "感谢光临！期待您下次品尝我们的精品！");
        })
    });

    /**
     * @brief 将咖啡馆场景的所有对话节点注册到数据库中。
     * @param db 对话数据库的引用。
     */
    inline void registerAllDialogs(std::map<unsigned int, const DialogNode*>& db) {
        db[12000001] = &_12000001;
    }

    /// @brief 创建一个静态的注册器实例，以实现自动注册。
    inline DialogRegistrar registrar(registerAllDialogs);

} // namespace CafeScene

#endif // CAFESCENE_H
