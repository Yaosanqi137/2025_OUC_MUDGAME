#ifndef FIGHTEVENT_H
#define FIGHTEVENT_H

/*

Changed on 9-14 2:23 by Anyeling
回合制战斗机制未完成
特殊奖励未处理
系统提示未完成
时间流逝未完成

*/


#include "../class/entity/Player.h"
#include "../class/entity/Enemy.h"
#include "../class/skill/Skill.h"
#include "../class/skill/SkillFactory.h"

#include <memory>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <set>
#include <string>
class Player;
class Enemy;
class Skill;
class SkillFactory;
class Game;
class Dialog;

class FightEvent {
public:
    // 回合操作类型（现在只有两个选项）
    enum class TurnAction {
        SKILL,      // 使用技能
        SKIP,       // 跳过回合
        NONE        // 未选择
    };

    FightEvent(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy);
    
    // 战斗流程控制
    void startBattle(Game& game);
    void endBattle();
    bool isBattleOver() const;
    
    // 回合操作
    void playerChooseAction(TurnAction action, int skillIndex = -1);
    void processEnemyTurn(); // 敌人回合处理
    
    // 战斗状态查询
    bool isPlayerWinner() const;
    bool isEnemyWinner() const;
    bool isPlayerTurn() const;
    int getCurrentRound() const;
    
    // 战斗奖励和应用
    void applyBattleRewards();

private:
    std::shared_ptr<Player> player_;
    std::shared_ptr<Enemy> enemy_;
    
    bool battleOver_;
    bool playerWon_;
    bool playerTurn_;
    int currentRound_;
    
    // 战斗辅助方法
    void configureEnemySkills();
    bool checkHit(double hitRate) const;
    
    // 伤害和应用方法
    void applyDamageToEnemy(double damage);
    void applyDamageToPlayer(double damage);
    
    // 回合处理
    void processPlayerSkill(int skillIndex);
    void processPlayerSkip();
    void processEnemySkip();
    
    // 击倒处理
    void processKnockdown(std::shared_ptr<Player> target, double recoveryPercent);
    void processKnockdown(std::shared_ptr<Enemy> target, double recoveryPercent);
    
    // 特殊奖励处理
    void applySpecialRewards(int enemyId);
};

#endif // FIGHTEVENT_H