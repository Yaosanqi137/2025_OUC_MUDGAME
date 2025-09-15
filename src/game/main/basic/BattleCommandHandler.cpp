#include "BattleCommandHandler.h"
#include "Game.h"
#include "../event/FightEvent.h"
#include <iostream>

// 实现 showEnemyInfo
void BattleCommandHandler::showEnemyInfo(Game& game, int enemyId) {
    try {
        // 检查 enemyId 的有效性
        if (enemyId <= 0 || enemyId > 11) {
            game.getDialog().addMessage("<SYSTEM>", "无效的敌人ID: " + std::to_string(enemyId));
            return;
        }
        
        // 使用 FightEvent 的静态方法创建敌人
        auto enemy = FightEvent::createEnemyById(enemyId);
        
        // 检查 enemy 是否有效
        if (!enemy) {
            game.getDialog().addMessage("<SYSTEM>", "错误: 无法创建敌人");
            return;
        }
        
        // 安全地获取敌人名称
        std::string enemyName;
        try {
            enemyName = enemy->getName();
            if (enemyName.empty()) {
                enemyName = "未知敌人";
            }
        } catch (...) {
            enemyName = "未知敌人";
        }
        
        game.getDialog().addMessage("<SYSTEM>", "下一个敌人: " + enemyName);
        
        // 安全地获取属性值
        double strength = 0, stamina = 0, agility = 0;
        try {
            strength = enemy->getStrength();
            stamina = enemy->getStamina();
            agility = enemy->getAgility();
        } catch (...) {
            // 忽略异常，使用默认值
        }
        
        game.getDialog().addMessage("<SYSTEM>", "属性: 力量 " + std::to_string((int)strength) +
                                  ", 耐力 " + std::to_string((int)stamina) +
                                  ", 敏捷 " + std::to_string((int)agility));
        
        // 安全地获取技能列表
        try {
            auto& skills = enemy->getSkills();
            if (!skills.empty()) {
                game.getDialog().addMessage("<SYSTEM>", "技能:");
                for (const auto& skill : skills) {
                    if (skill) {
                        std::string skillName, skillDesc;
                        try {
                            skillName = skill->getSkillName();
                            skillDesc = skill->getDescription();
                        } catch (...) {
                            skillName = "未知技能";
                            skillDesc = "无描述";
                        }
                        
                        game.getDialog().addMessage("<SYSTEM>", "  - " + skillName + ": " + skillDesc);
                    }
                }
            }
        } catch (...) {
            game.getDialog().addMessage("<SYSTEM>", "无法获取技能列表");
        }
    }
    catch (const std::exception& e) {
        game.getDialog().addMessage("<SYSTEM>", "错误: " + std::string(e.what()));
        std::cerr << "Exception in showEnemyInfo: " << e.what() << std::endl;
    }
    catch (...) {
        game.getDialog().addMessage("<SYSTEM>", "未知错误");
        std::cerr << "Unknown exception in showEnemyInfo" << std::endl;
    }
}

void BattleCommandHandler::startBattle(Game& game, int enemyId) {
    if (enemyId > 11) {
        game.getDialog().addMessage("<SYSTEM>", "没有更多敌人了");
        return;
    }
    
    // 创建敌人实例
    auto enemy = FightEvent::createEnemyById(enemyId);
    
    // 获取玩家的共享指针
    auto playerPtr = game.getPlayerSharedPtr();
    if (!playerPtr) {
        game.getDialog().addMessage("<SYSTEM>", "错误: 无法获取玩家引用");
        return;
    }
    
    // 创建战斗事件
    auto fightEvent = std::make_shared<FightEvent>(playerPtr, enemy, game);
    
    // 设置当前战斗
    game.setCurrentBattle(fightEvent);
    
    // 开始战斗
    fightEvent->startBattle();
}
