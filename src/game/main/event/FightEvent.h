#ifndef FIGHTEVENT_H
#define FIGHTEVENT_H

#include "../class/entity/Player.h"
#include "../class/entity/Enemy.h"
#include "../class/skill/Skill.h"
#include "../class/skill/SkillFactory.h"
#include "../basic/BattleCommandHandler.h"
#include "../basic/Game.h"

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <set>

// 战斗状态定义
#define BATTLE_STATE_INITIALIZING 0    // 战斗初始化
#define BATTLE_STATE_PLAYER_TURN 1     // 玩家回合
#define BATTLE_STATE_ENEMY_TURN 2      // 敌人回合
#define BATTLE_STATE_PROCESSING 3      // 处理行动结果
#define BATTLE_STATE_BATTLE_OVER 4     // 战斗结束

// 战斗动作常量
const int FIGHT_ACTION_SKILL = 0;
const int FIGHT_ACTION_SKIP = 1;
const int FIGHT_ACTION_NONE = 2;

class FightEvent {
public:
    FightEvent(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy, Game& game);
    
    // 战斗流程控制
    void startBattle();
    void endBattle();
    bool isBattleOver() const;
    
    // 回合操作
    void playerChooseAction(int action, int skillIndex = -1);
    void processEnemyTurn();
    
    // 战斗状态查询
    bool isPlayerWinner() const;
    bool isEnemyWinner() const;
    bool isPlayerTurn() const;
    int getCurrentRound() const;
    int getBattleState() const;
    
    // 战斗奖励和应用
    void applyBattleRewards();
    
    // 敌人创建工厂方法
    static std::shared_ptr<Enemy> createEnemyById(int enemyId);

private:
    std::shared_ptr<Player> player_;
    std::shared_ptr<Enemy> enemy_;
    Game& game_;
    
    bool battleOver_;
    bool playerWon_;
    int currentRound_;
    int battleState_; // 战斗状态

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
    
    // 敌人属性配置
    static std::map<int, std::function<std::shared_ptr<Enemy>()>> enemyFactory;
    
    // 初始化敌人工厂
    static void initializeEnemyFactory();
    
    // 战斗流程控制
    void advanceBattleState();
};

#endif // FIGHTEVENT_H
