#ifndef CAFESCENE_H
#define CAFESCENE_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"
#include "../../class/item/Food.h"

namespace CafeScene {
    // 女仆咖啡购买成功对话
    inline const DialogNode _12000010(12000010, "女仆", "好喝魔法变变变！希里希里，哗啦哗啦", {}, 12000011);
    inline const DialogNode _12000011(12000011, "女仆", "好喝的咖啡来了~");

    // 女仆咖啡钱不够对话
    inline const DialogNode _12000012(12000012, "女仆", "小哥哥，您身上的钱似乎不太够呢");

    // 爱心面包购买成功对话
    inline const DialogNode _12000020(12000020, "女仆", "跟我一起念咒语：希里希里，哗啦哗啦", {}, 12000021);
    inline const DialogNode _12000021(12000021, "女仆", "好吃魔法变变变！爱心面包来了~");

    // 爱心面包钱不够对话
    inline const DialogNode _12000022(12000022, "女仆", "小哥哥，您身上的钱似乎不太够呢");

    // 瓦学弟蛋包饭购买成功对话序列
    inline const DialogNode _12000030(12000030, "女仆", "极柔极柔，浓香浓香", {}, 12000031);
    inline const DialogNode _12000031(12000031, "女仆", "惊奇的心动魔法", {}, 12000032);
    inline const DialogNode _12000032(12000032, "女仆", "蛋包饭", {}, 12000033);
    inline const DialogNode _12000033(12000033, "女仆", "变得美味吧", {}, 12000034);
    inline const DialogNode _12000034(12000034, "", "仿佛真的有什么东西飘进蛋包饭里了", {}, 12000035);
    inline const DialogNode _12000035(12000035, "<PLAYER_NAME>", "番茄酱恰到好处的酸味", {}, 12000036);
    inline const DialogNode _12000036(12000036, "<PLAYER_NAME>", "和酱汁的甘甜融合为一", {}, 12000037);
    inline const DialogNode _12000037(12000037, "<PLAYER_NAME>", "摇身一变，成为暖心的家常风味，滑润顺口！");

    // 瓦学弟蛋包饭钱不够对话
    inline const DialogNode _12000038(12000038, "女仆", "小哥哥，您身上的钱似乎不太够呢");

    // 退出对话
    inline const DialogNode _12000040(12000040, "女仆", "感谢光临！期待您下次品尝我们的精品！");

    // 主菜单对话节点
    inline const DialogNode _12000001(12000001, "女仆", "菜单来咯，等一下让我来施展魔法哦",{
        Choice("女仆咖啡 - 60元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 60) {
                game_logic_.getPlayer().addSavings(-60);
                Food coffee(Food::FoodType::COFFEE);
                coffee.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(coffee));
                game_logic_.getStoryController().processNodeByID(12000010);
            } else {
                game_logic_.getStoryController().processNodeByID(12000012);
            }
        }),
        Choice("爱心面包 - 70元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 70) {
                game_logic_.getPlayer().addSavings(-70);
                Food bread(Food::FoodType::BREAD);
                bread.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(bread));
                game_logic_.getStoryController().processNodeByID(12000020);
            } else {
                game_logic_.getStoryController().processNodeByID(12000022);
            }
        }),
        Choice("瓦学弟蛋包饭 - 150元", 0, [](const Game& game_logic_) {
            if (game_logic_.getPlayer().getSavings() >= 150) {
                game_logic_.getPlayer().addSavings(-150);
                Food waxuedi(Food::FoodType::WAXUEDI);
                waxuedi.setAmount(1);
                game_logic_.getPlayer().addItem(std::make_shared<Food>(waxuedi));
                game_logic_.getStoryController().processNodeByID(12000030);
            } else {
                game_logic_.getStoryController().processNodeByID(12000038);
            }
        }),
        Choice("退出", 0, [](const Game& game_logic_) {
            game_logic_.getStoryController().processNodeByID(12000040);
        })
    });

    /**
     * @brief 将咖啡馆场景的所有对话节点注册到数据库中。
     * @param db 对话数据库的引用。
     */
    inline void registerAllDialogs(std::map<unsigned int, const DialogNode*>& db) {
        db[12000001] = &_12000001;
        db[12000010] = &_12000010;
        db[12000011] = &_12000011;
        db[12000012] = &_12000012;
        db[12000020] = &_12000020;
        db[12000021] = &_12000021;
        db[12000022] = &_12000022;
        db[12000030] = &_12000030;
        db[12000031] = &_12000031;
        db[12000032] = &_12000032;
        db[12000033] = &_12000033;
        db[12000034] = &_12000034;
        db[12000035] = &_12000035;
        db[12000036] = &_12000036;
        db[12000037] = &_12000037;
        db[12000038] = &_12000038;
        db[12000040] = &_12000040;
    }

    /// @brief 创建一个静态的注册器实例，以实现自动注册。
    inline DialogRegistrar registrar(registerAllDialogs);

} // namespace CafeScene

#endif // CAFESCENE_H
