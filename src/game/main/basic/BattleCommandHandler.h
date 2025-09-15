// BattleCommandHandler.h
#ifndef BATTLE_COMMAND_HANDLER_H
#define BATTLE_COMMAND_HANDLER_H

#include <string>

class Game;

class BattleCommandHandler {
public:
    // 处理战斗命令
    static bool handleCommand(Game& game, const std::string& input);
    
    // 显示敌人信息
    static void showEnemyInfo(Game& game, int enemyId);
    
    // 开始战斗
    static void startBattle(Game& game, int enemyId);
    
private:
    // 处理攻击命令
    static bool handleAttackCommand(Game& game, const std::string& input);
    
    // 处理跳过命令
    static bool handleSkipCommand(Game& game);
};

#endif // BATTLE_COMMAND_HANDLER_H
