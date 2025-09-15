#ifndef BATTLE_COMMAND_HANDLER_H
#define BATTLE_COMMAND_HANDLER_H

#include <string>

class Game;

class BattleCommandHandler {
public:
    // 显示敌人信息
    static void showEnemyInfo(Game& game, int enemyId);
    
    // 开始战斗
    static void startBattle(Game& game, int enemyId);
    
private:
};

#endif // BATTLE_COMMAND_HANDLER_H
