#ifndef BATTLESCENE_H
#define BATTLESCENE_H

#include "../DialogRegistry.h"
#include "../Story.h"
#include "../../basic/Game.h"
#include "../../class/entity/Player.h"
#include "../../event/FightEvent.h"
namespace BattleScene {
    // 战斗场景对话节点ID范围: 20000000-20999999
    
    /**
     * @brief 动态创建战斗对话节点
     * @param enemyId 敌人ID
     * @param game 游戏引用
     * @return 动态创建的对话节点
     */
    inline DialogNode* createBattleNode(int enemyId, Game& game) {
        unsigned int nodeId = 20000000 + enemyId;
        std::string enemyName = "未知敌人";
        
        // 获取敌人信息
        auto enemy = FightEvent::createEnemyById(enemyId);
        if (enemy) {
            enemyName = enemy->getName();
        }
        
        std::vector<Choice> choices;
        
        // 获取玩家技能
        auto& playerSkills = game.getPlayer().getSkills();
        for (int i = 0; i < playerSkills.size(); i++) {
            if (playerSkills[i]->isAttackSkill()) {
                // 为每个攻击技能创建选项
                choices.push_back(Choice(
                    playerSkills[i]->getSkillName() + " (消耗体力: " + 
                    std::to_string((int)playerSkills[i]->calculateStaminaCost(game.getPlayer().getStrength())) + ")",
                    0,
                    [i, &game](const Game& game_logic_) {
                        // 处理技能选择
                        auto battle = game_logic_.getCurrentBattle();
                        if (battle) {
                            battle->playerChooseAction(0, i); // 0 表示技能攻击
                        }
                    }
                ));
            }
        }
        
        // 添加跳过回合选项
        choices.push_back(Choice(
            "跳过回合 (恢复20%体力)",
            0,
            [&game](const Game& game_logic_) {
                auto battle = game_logic_.getCurrentBattle();
                if (battle) {
                    battle->playerChooseAction(1, 0); // 1 表示跳过回合
                }
            }
        ));
        
        return new DialogNode(
            nodeId,
            enemyName,
            "选择你的行动:",
            choices
        );
    }
    
    /**
     * @brief 将战斗场景的所有对话节点注册到数据库中。
     * @param db 对话数据库的引用。
     */
    inline void registerAllDialogs(std::map<unsigned int, const DialogNode*>& db) {
        // 战斗节点的注册将在运行时动态进行
        // 这里只注册一个占位符节点
        static DialogNode placeholder(20000000, "战斗", "战斗场景", {});
        db[20000000] = &placeholder;
    }
    
    /// @brief 创建一个静态的注册器实例，以实现自动注册。
    inline DialogRegistrar registrar(registerAllDialogs);
}

#endif // BATTLESCENE_H
