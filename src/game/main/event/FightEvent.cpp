#include "FightEvent.h"
#include "../basic/Dialog.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>

// 初始化敌人工厂
std::map<int, std::function<std::shared_ptr<Enemy>()>> FightEvent::enemyFactory;

FightEvent::FightEvent(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy, Game& game)
    : player_(player), enemy_(enemy), game_(game), battleOver_(false), playerWon_(false),
      currentRound_(0), battleState_(BATTLE_STATE_INITIALIZING) {
    
    // 检查 player 和 enemy 是否有效
    if (!player_) {
        throw std::invalid_argument("无效的玩家指针");
    }
    
    if (!enemy_) {
        throw std::invalid_argument("无效的敌人指针");
    }
    
    try {
        // 配置敌人技能
        configureEnemySkills();
        
        // 初始化随机数种子
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in FightEvent constructor: " << e.what() << std::endl;
        throw; // 重新抛出异常
    }
    catch (...) {
        std::cerr << "Unknown exception in FightEvent constructor" << std::endl;
        throw std::runtime_error("未知异常在 FightEvent 构造函数中");
    }
}

// 初始化敌人工厂
void FightEvent::initializeEnemyFactory() {
    if (!enemyFactory.empty()) return;
    
    enemyFactory[1] = []() {
        return std::make_shared<Enemy>(1, "业余拳手", 1.0, 2.0, 1.0);
    };
    
    enemyFactory[2] = []() {
        return std::make_shared<Enemy>(2, "健身房常客", 3.0, 4.0, 2.0);
    };
    
    enemyFactory[3] = []() {
        return std::make_shared<Enemy>(3, "街头拳手", 4.0, 3.0, 3.0);
    };
    
    enemyFactory[4] = []() {
        return std::make_shared<Enemy>(4, "健身房教练", 6.0, 5.0, 4.0);
    };
    
    enemyFactory[5] = []() {
        return std::make_shared<Enemy>(5, "职业新人", 7.0, 6.0, 5.0);
    };
    
    enemyFactory[6] = []() {
        return std::make_shared<Enemy>(6, "速度型选手", 2.0, 6.0, 8.0);
    };
    
    enemyFactory[7] = []() {
        return std::make_shared<Enemy>(7, "耐力型选手", 8.0, 10.0, 6.0);
    };
    
    enemyFactory[8] = []() {
        return std::make_shared<Enemy>(8, "技巧型拳手", 4.0, 8.0, 13.0);
    };
    
    enemyFactory[9] = []() {
        return std::make_shared<Enemy>(9, "力量型拳手", 15.0, 12.0, 7.0);
    };
    
    enemyFactory[10] = []() {
        return std::make_shared<Enemy>(10, "冠军挑战者", 18.0, 15.0, 14.0);
    };
    
    enemyFactory[11] = []() {
        return std::make_shared<Enemy>(11, "世界拳王", 25.0, 20.0, 25.0);
    };
}

std::shared_ptr<Enemy> FightEvent::createEnemyById(int enemyId) {
    try {
        if (enemyFactory.empty()) {
            initializeEnemyFactory();
        }
        
        auto it = enemyFactory.find(enemyId);
        if (it != enemyFactory.end()) {
            auto enemy = it->second();
            if (!enemy) {
                std::cerr << "Warning: Enemy factory returned null for ID: " << enemyId << std::endl;
                return std::make_shared<Enemy>(enemyId, "默认敌人", 5.0, 5.0, 5.0);
            }
            return enemy;
        }
        
        // 默认敌人
        return std::make_shared<Enemy>(enemyId, "未知对手", 5.0, 5.0, 5.0);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in createEnemyById: " << e.what() << std::endl;
        return std::make_shared<Enemy>(enemyId, "错误敌人", 5.0, 5.0, 5.0);
    }
    catch (...) {
        std::cerr << "Unknown exception in createEnemyById" << std::endl;
        return std::make_shared<Enemy>(enemyId, "错误敌人", 5.0, 5.0, 5.0);
    }
}

void FightEvent::configureEnemySkills() {
    // 清空现有技能
    auto& enemySkills = enemy_->getSkills();
    enemySkills.clear();
    
    // 根据敌人ID配置技能
    int enemyId = enemy_->getId();
    
    switch (enemyId) {
        case 1: // 业余拳手
            enemy_->addSkill(SkillFactory::createLongPunch());
            break;
            
        case 2: // 健身房常客
            enemy_->addSkill(SkillFactory::createPunch());
            break;
            
        case 3: // 街头拳手
            enemy_->addSkill(SkillFactory::createKick());
            break;
            
        case 4: // 健身房教练
            enemy_->addSkill(SkillFactory::createHeavyPunch());
            break;
            
        case 5: // 职业新人
            enemy_->addSkill(SkillFactory::createUppercut());
            break;
            
        case 6: // 速度型选手
            enemy_->addSkill(SkillFactory::createBackhandHeavyPunch());
            break;
            
        case 7: // 耐力型选手
            enemy_->addSkill(SkillFactory::createBackBreak());
            enemy_->addSkill(SkillFactory::createSuicideAttack());
            break;
            
        case 8: // 技巧型拳手
            enemy_->addSkill(SkillFactory::createKarateHighKick());
            enemy_->addSkill(SkillFactory::createFlashStrike());
            break;
            
        case 9: // 力量型拳手
            enemy_->addSkill(SkillFactory::createChargedUppercut());
            enemy_->addSkill(SkillFactory::createHumanHammer());
            break;
            
        case 10: // 冠军挑战者
            enemy_->addSkill(SkillFactory::createCloseCombat());
            enemy_->addSkill(SkillFactory::createInfiniteEnergy());
            enemy_->addSkill(SkillFactory::createHumanHammer());
            enemy_->addSkill(SkillFactory::createFlashStrike());
            break;
            
        case 11: // 世界拳王
            enemy_->addSkill(SkillFactory::createPunch());
            enemy_->addSkill(SkillFactory::createKick());
            enemy_->addSkill(SkillFactory::createHeavyPunch());
            enemy_->addSkill(SkillFactory::createUppercut());
            enemy_->addSkill(SkillFactory::createKarateHighKick());
            enemy_->addSkill(SkillFactory::createCloseCombat());
            enemy_->addSkill(SkillFactory::createFlashStrike());
            enemy_->addSkill(SkillFactory::createHumanHammer());
            enemy_->addSkill(SkillFactory::createInfiniteEnergy());
            break;
            
        default:
            enemy_->addSkill(SkillFactory::createPunch());
            break;
    }
}

void FightEvent::startBattle() {
    try {
        battleState_ = BATTLE_STATE_INITIALIZING;
        battleOver_ = false;
        playerWon_ = false;
        currentRound_ = 1;

        game_.getDialog().addMessage("<SYSTEM>", "战斗开始了，对手是" + enemy_->getName());
        game_.getDialog().addMessage("<SYSTEM>", "他的攻击技能是:");
        for (int i = 0; i < enemy_ -> getSkills().size(); i++) {
            if (enemy_->getSkills()[i]->isAttackSkill()) {
                game_.getDialog().addMessage("<SYSTEM>", "ID: " + std::to_string(enemy_->getSkills()[i]->getId()) + 
                    " - " + enemy_->getSkills()[i]->getSkillName() + ": " + enemy_->getSkills()[i]->getDescription());
            }
        }
        // 显示敌人信息
        game_.getDialog().addMessage("<SYSTEM>", "敌人属性: 力量 " + std::to_string((int)enemy_->getStrength()) +
                                    ", 耐力 " + std::to_string((int)enemy_->getStamina()) +
                                    ", 敏捷 " + std::to_string((int)enemy_->getAgility()));
        
        // 显示玩家可用攻击技能
        auto& playerSkills = player_->getSkills();
        game_.getDialog().addMessage("<SYSTEM>", "你的可用攻击技能:");
        for (int i = 0; i < playerSkills.size(); i++) {
            if (playerSkills[i]->isAttackSkill()) {
                game_.getDialog().addMessage("<SYSTEM>", "ID: " + std::to_string(playerSkills[i]->getId()) + 
                    " - " + playerSkills[i]->getSkillName() + ": " + playerSkills[i]->getDescription());
            }
        }
        
        game_.getDialog().addMessage("<SYSTEM>", "战斗命令: /enemy attack skill <id> 或 /enemy attack pass");

        // 决定先手：比较玩家和敌人的速度
        double playerSpeed = player_->getSpeed();
        double enemySpeed = enemy_->getSpeed();
        
        bool playerFirst = (playerSpeed > enemySpeed);
        if(playerSpeed == enemySpeed) {
            int roll = std::rand() % 2;
            playerFirst = (roll == 0);
        }
        
        if(playerFirst){
            game_.getDialog().addMessage("<SYSTEM>", player_->getName() + "先手行动");
            battleState_ = BATTLE_STATE_PLAYER_TURN;
            game_.getDialog().addMessage("<SYSTEM>", "轮到了你的回合，请选择行动");
        } else {
            game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "先手行动");
            battleState_ = BATTLE_STATE_ENEMY_TURN;
            // 延迟一下让玩家看到消息
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            processEnemyTurn();
        }
    }
    catch (const std::exception& e) {
        game_.getDialog().addMessage("<SYSTEM>", "战斗开始错误: " + std::string(e.what()));
        std::cerr << "Exception in FightEvent::startBattle: " << e.what() << std::endl;
        battleState_ = BATTLE_STATE_BATTLE_OVER;
        battleOver_ = true;
    }
    catch (...) {
        game_.getDialog().addMessage("<SYSTEM>", "战斗开始未知错误");
        std::cerr << "Unknown exception in FightEvent::startBattle" << std::endl;
        battleState_ = BATTLE_STATE_BATTLE_OVER;
        battleOver_ = true;
    }
}

void FightEvent::endBattle() {
    battleOver_ = true;
    battleState_ = BATTLE_STATE_BATTLE_OVER;
    
    // 清除当前战斗
    game_.clearCurrentBattle();
    
    if (player_->getHealth() <= 0) {
        playerWon_ = false;
        game_.getDialog().addMessage("<SYSTEM>", "你被击败了！");
        // 战败属性下降
        player_->addStrength(-1);
        player_->addStamina(-1);
        player_->addAgility(-1);
        game_.getDialog().addMessage("<SYSTEM>", "身体和信心都受到打击，感觉身体有些生疏了……");
    } else if (enemy_->getHealth() <= 0) {
        playerWon_ = true;
        game_.getDialog().addMessage("<SYSTEM>", "你获胜了！");
        
        // 解锁下一个敌人
        int enemyId = enemy_->getId();
        // std::cout << "DEBUG: endBattle - enemy ID: " << enemyId << std::endl;
        // game_.getDialog().addMessage("<SYSTEM>", "DEBUG: endBattle - enemy ID: " + std::to_string(enemyId));
        player_->unlockNextEnemy(enemyId);
        
        applyBattleRewards();
    }
}


bool FightEvent::isBattleOver() const {
    return battleOver_ || player_->getHealth() <= 0 || enemy_->getHealth() <= 0;
}

bool FightEvent::isPlayerWinner() const {
    return playerWon_;
}

bool FightEvent::isEnemyWinner() const {
    return !playerWon_ && battleOver_;
}

bool FightEvent::isPlayerTurn() const {
    return battleState_ == BATTLE_STATE_PLAYER_TURN;
}

int FightEvent::getCurrentRound() const {
    return currentRound_;
}

int FightEvent::getBattleState() const {
    return battleState_;
}

void FightEvent::playerChooseAction(int action, int skillIndex) {
    if (battleOver_ || battleState_ != BATTLE_STATE_PLAYER_TURN) {
        game_.getDialog().addMessage("<SYSTEM>", "现在不是你的回合或战斗已结束");
        return;
    }
    
    battleState_ = BATTLE_STATE_PROCESSING;
    
    try {
        switch (action) {
            case 0: // 技能攻击
                if (skillIndex >= 0 && skillIndex < static_cast<int>(player_->getSkills().size())) {
                    processPlayerSkill(skillIndex);
                } else {
                    game_.getDialog().addMessage("<SYSTEM>", "无效的技能选择");
                    battleState_ = BATTLE_STATE_PLAYER_TURN; // 回到玩家回合
                }
                break;
                
            case 1: // 跳过回合
                processPlayerSkip();
                break;
                
            default:
                game_.getDialog().addMessage("<SYSTEM>", "无效的战斗行动");
                battleState_ = BATTLE_STATE_PLAYER_TURN; // 回到玩家回合
                break;
        }
        
        // 只有在成功处理行动后才推进战斗状态
        if (battleState_ == BATTLE_STATE_PROCESSING && !battleOver_) {
            advanceBattleState();
        }
    }
    catch (const std::exception& e) {
        game_.getDialog().addMessage("<SYSTEM>", "处理玩家行动时出错: " + std::string(e.what()));
        std::cerr << "Exception in playerChooseAction: " << e.what() << std::endl;
        battleState_ = BATTLE_STATE_PLAYER_TURN; // 回到玩家回合
    }
    catch (...) {
        game_.getDialog().addMessage("<SYSTEM>", "处理玩家行动时发生未知错误");
        std::cerr << "Unknown exception in playerChooseAction" << std::endl;
        battleState_ = BATTLE_STATE_PLAYER_TURN; // 回到玩家回合
    }
}



void FightEvent::processEnemyTurn() {
    if (battleOver_ || battleState_ != BATTLE_STATE_ENEMY_TURN) {
        return;
    }
    
    try {
        // 敌人AI：可以选择使用技能或跳过
        auto& enemySkills = enemy_->getSkills();
        if (!enemySkills.empty() && (std::rand() % 100) < 70) {
            // 随机选择一个技能
            int skillIndex = std::rand() % enemySkills.size();
            auto skill = enemySkills[skillIndex];
            
            if(skill->canUse(*enemy_)) {
                game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "使用了" + skill->getSkillName());
                
                // 计算伤害
                double damage = skill->calculateDamage(enemy_->getStrength());
                double hitRate = skill->calculateHitRate(enemy_->getAgility(), 
                                                       enemy_->getStrength(), 
                                                       enemy_->getStamina());
                
                if (checkHit(hitRate)) {
                    applyDamageToPlayer(damage);
                    game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "造成了" + std::to_string((int)damage) + "点伤害");
                    
                    // 检查玩家是否被击倒（耐力透支）
                    double staminaCost = skill->calculateStaminaCost(enemy_->getStrength());
                    if (player_->getFatigue() < staminaCost) {
                        processKnockdown(player_, 20.0); // 玩家被击倒，恢复20%体力
                    }
                } else {
                    game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "的攻击未命中!");
                }
                
                // 消耗敌人体力
                double staminaCost = skill->calculateStaminaCost(enemy_->getStrength());
                enemy_->addFatigue(-staminaCost);
            } else {
                processEnemySkip();
            }
        } else {
            // 敌人跳过回合，恢复20%体力
            processEnemySkip();
        }
        
        // 显示状态更新
        game_.getDialog().addMessage("<SYSTEM>", "你的状态: 生命值 " + std::to_string((int)player_->getHealth()) + 
                                    "/" + std::to_string((int)player_->getMaxHealth()) +
                                    ", 体力 " + std::to_string((int)player_->getFatigue()) + 
                                    "/" + std::to_string((int)player_->getMaxFatigue()));
        game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "的状态: 生命值 " + 
                                    std::to_string((int)enemy_->getHealth()) + "/" + 
                                    std::to_string((int)enemy_->getMaxHealth()) +
                                    ", 体力 " + std::to_string((int)enemy_->getFatigue()) + 
                                    "/" + std::to_string((int)enemy_->getMaxFatigue()));
        
        // 推进战斗状态
        if (!battleOver_) {
            advanceBattleState();
        }
    }
    catch (const std::exception& e) {
        game_.getDialog().addMessage("<SYSTEM>", "处理敌人行动时出错: " + std::string(e.what()));
        std::cerr << "Exception in processEnemyTurn: " << e.what() << std::endl;
        battleState_ = BATTLE_STATE_PLAYER_TURN; // 回到玩家回合
    }
    catch (...) {
        game_.getDialog().addMessage("<SYSTEM>", "处理敌人行动时发生未知错误");
        std::cerr << "Unknown exception in processEnemyTurn" << std::endl;
        battleState_ = BATTLE_STATE_PLAYER_TURN; // 回到玩家回合
    }
}


bool FightEvent::checkHit(double hitRate) const {
    double roll = (std::rand() % 10000) / 10000.0;
    return roll <= hitRate;
}

void FightEvent::applyDamageToEnemy(double damage) {
    // 确保伤害不为负
    if (damage < 0) {
        damage = 0;
    }
    
    // 保存当前血量
    double currentHealth = enemy_->getHealth();
    
    // 应用伤害
    enemy_->addHealth(-damage);
    
    // 确保生命值不低于0
    if (enemy_->getHealth() < 0) {
        enemy_->addHealth(-enemy_->getHealth()); // 设置为0
    }
    
    // 检查敌人是否被击败
    if (currentHealth > 0 && enemy_->getHealth() <= 0) {
        // 敌人被击败，结束战斗
        battleOver_ = true;
        playerWon_ = true;
        endBattle();
    }
}

void FightEvent::applyDamageToPlayer(double damage) {
    // 确保伤害不为负
    if (damage < 0) {
        damage = 0;
    }
    
    // 保存当前血量
    double currentHealth = player_->getHealth();
    
    // 应用伤害
    player_->addHealth(-damage);
    
    // 确保生命值不低于0
    if (player_->getHealth() < 0) {
        player_->addHealth(-player_->getHealth()); // 设置为0
    }
    
    // 检查玩家是否被击败
    if (currentHealth > 0 && player_->getHealth() <= 0) {
        // 玩家被击败，结束战斗
        battleOver_ = true;
        playerWon_ = false;
        endBattle();
    }
}

void FightEvent::processPlayerSkill(int skillIndex) {
    auto& playerSkills = player_->getSkills();
    if (skillIndex < 0 || skillIndex >= static_cast<int>(playerSkills.size())) {
        game_.getDialog().addMessage("<SYSTEM>", "无效的技能选择");
        return;
    }
    
    auto skill = playerSkills[skillIndex];
    
    // 检查技能是否可用
    if (!skill->canUse(*player_)) {
        game_.getDialog().addMessage("<SYSTEM>", "技能不可用，可能体力不足");
        return;
    }
    
    game_.getDialog().addMessage("<SYSTEM>", "你使用了" + skill->getSkillName());
    
    // 计算伤害
    double damage = skill->calculateDamage(player_->getStrength());
    double hitRate = skill->calculateHitRate(player_->getAgility(), 
                                           player_->getStrength(), 
                                           player_->getStamina());
    
    // 特殊技能效果
    for(auto& skill__ : player_->getSkills()){
        if(skill__ -> getSkillName() == "千手不破"){
            std::set<int> upperSkillIndex = {1,4,5,6,7,8,11,12,13,14,15};
            std::set<int> lowerSkillIndex = {2,3,9,10};
            if(upperSkillIndex.find(skillIndex) != upperSkillIndex.end())
                damage *= enemy_ -> getUpperBodySustainDamageRate();
            else if(lowerSkillIndex.find(skillIndex) != lowerSkillIndex.end())
                damage *= enemy_ -> getLowerBodySustainDamageRate();
        }
    }
    
    hitRate += player_ -> getExHitRate();
    hitRate = std::max(0.0, hitRate);
    hitRate = std::min(1.0, hitRate);
    
    if (checkHit(hitRate)) {
        applyDamageToEnemy(damage);
        game_.getDialog().addMessage("<SYSTEM>", "你造成了" + std::to_string((int)damage) + "点伤害");
        
        // 特殊技能 31号技能"闪击"
        for(auto& skill__ : enemy_->getSkills()){
            if(skill__ -> getSkillName() == "闪击"){
                double roll = std::rand() % 100;
                if(roll < 25){
                    double reflectDamage = damage * 0.25;
                    applyDamageToPlayer(reflectDamage);
                    game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "的闪击效果反弹了" + 
                                                std::to_string((int)reflectDamage) + "点伤害");
                }
            }
        }
        
        // 检查敌人是否被击倒（耐力透支）
        double staminaCost = skill->calculateStaminaCost(player_->getStrength());
        if (enemy_->getFatigue() < staminaCost) {
            processKnockdown(enemy_, 20.0);
        }
    } else {
        game_.getDialog().addMessage("<SYSTEM>", "你的攻击未命中!");
    }
    
    // 计算玩家体力消耗
    double staminaCost = skill->calculateStaminaCost(player_->getStrength());
    
    // 特殊技能效果
    for(auto& skill__ : player_->getSkills()){
        if(skill__ -> getSkillName() == "无限能量"){
            if(player_ -> getHealth() < (player_ -> getMaxHealth() + player_ -> getExMaxHealth()))
                staminaCost *= (1 - 0.15);
        }
    }
    
    // 消耗玩家体力
    player_->addFatigue(-staminaCost);
    
    // 执行技能效果
    if((19 <= skillIndex && skillIndex <= 30) || skillIndex == 36){
        skill->execute(*player_, *player_);
    } else {
        skill->execute(*player_, *enemy_);
    }
    
    // 显示状态更新
    game_.getDialog().addMessage("<SYSTEM>", "你的状态: 生命值 " + std::to_string((int)player_->getHealth()) + 
                                "/" + std::to_string((int)player_->getMaxHealth()) +
                                ", 体力 " + std::to_string((int)player_->getFatigue()) + 
                                "/" + std::to_string((int)player_->getMaxFatigue()));
    game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "的状态: 生命值 " + 
                                std::to_string((int)enemy_->getHealth()) + "/" + 
                                std::to_string((int)enemy_->getMaxHealth()) +
                                ", 体力 " + std::to_string((int)enemy_->getFatigue()) + 
                                "/" + std::to_string((int)enemy_->getMaxFatigue()));
}

void FightEvent::processPlayerSkip() {
    // 跳过回合恢复20%体力
    double recoveryAmount = player_->getMaxFatigue() * 0.2;
    player_->addFatigue(recoveryAmount);
    player_->getGameLogic().getDialog().addMessage("<SYSTEM>", "你选择跳过回合，恢复了" + std::to_string((int)recoveryAmount) + "点体力");
}

void FightEvent::processEnemySkip() {
    // 敌人跳过回合恢复20%体力
    double recoveryAmount = enemy_->getMaxFatigue() * 0.2;
    enemy_->addFatigue(recoveryAmount);
    game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "选择跳过回合，恢复了" + 
                                std::to_string((int)recoveryAmount) + "点体力");
}

void FightEvent::processKnockdown(std::shared_ptr<Player> target, double recoveryPercent) {
    // 玩家被击倒：损失20%血量，恢复一定百分比体力
    double healthLoss = target->getHealth() * 0.2;
    double fatigueRecovery = target->getMaxFatigue() * (recoveryPercent / 100.0);
    
    target->addHealth(-healthLoss);
    target->addFatigue(fatigueRecovery);
    
    game_.getDialog().addMessage("<SYSTEM>", "你被击倒了! 损失" + std::to_string((int)healthLoss) + 
                                "点生命值，恢复" + std::to_string((int)fatigueRecovery) + "点体力");
}

void FightEvent::processKnockdown(std::shared_ptr<Enemy> target, double recoveryPercent) {
    // 敌人被击倒：损失20%血量，恢复一定百分比体力
    double healthLoss = target->getHealth() * 0.2;
    double fatigueRecovery = target->getMaxFatigue() * (recoveryPercent / 100.0);
    
    target->addHealth(-healthLoss);
    target->addFatigue(fatigueRecovery);
    
    game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "被击倒了! 损失" + 
                                std::to_string((int)healthLoss) + "点生命值，恢复" + 
                                std::to_string((int)fatigueRecovery) + "点体力");
}

void FightEvent::applyBattleRewards() {
    if (!playerWon_) {
        return;
    }
    
    int enemyId = enemy_->getId();
    std::string enemyName = enemy_ -> getName();
    // 基础奖励
    switch (enemyId) {
        case 1: // 业余拳手
            player_->addSavings(0); // 教学战无奖励
            game_.getDialog().addMessage(enemyName, "看来你还有很多要学呢。");
            break;
            
        case 2: // 健身房常客
            player_->addSavings(20);
            player_->addSkillPoints(1);
            break;
            
        case 3: // 街头拳手
            player_->addSavings(30);
            player_->addSkillPoints(1);
            break;
            
        case 4: // 健身房教练
            player_->addSavings(50);
            player_->addSkillPoints(2);
            player_->getTrainingSystem()->setMoneyCostRate(0.5);
            game_.getDialog().addMessage(enemyName, "呼…呼…不错，你真的变强了。拿着，这是我的VIP卡，以后来训练给你打折。");
            game_.getDialog().addMessage("<SYSTEM>", "获得健身房VIP卡！训练费用减半！");
            break;
            
        case 5: // 职业新人
            player_->addSavings(50);
            player_->addSkillPoints(1);
            break;
            
        case 6: // 速度型选手
            player_->addSavings(50);
            player_->addSkillPoints(1);
            break;
            
        case 7: // 耐力型选手
            player_->addSavings(100);
            player_->addSkillPoints(2);
            player_->getTrainingSystem()->setStaminaExpRate(player_->getTrainingSystem()->getStaminaExpRate() + 0.2);
            game_.getDialog().addMessage(enemyName, "怎么可能…我的耐力竟然…输了…");
            game_.getDialog().addMessage("<SYSTEM>", "获得耐力训练器！耐力训练效率+20%！");
            break;
            
        case 8: // 技巧型拳手
            player_->addSavings(200);
            player_->addSkillPoints(2);
            break;
            
        case 9: // 力量型拳手
            player_->addSavings(200);
            player_->addSkillPoints(2);
            break;
            
        case 10: // 冠军挑战者
            player_->addSavings(500);
            player_->addSkillPoints(3);
            player_->addStrength(2);
            player_->addStamina(2);
            player_->addAgility(2);
            game_.getDialog().addMessage(enemyName, "呃啊……世界级的舞台……果然……深不可测……");
            game_.getDialog().addMessage("<SYSTEM>", "获得冠军腰带！全属性+2！");
            break;
            
        case 11: // 世界拳王
            player_->addSavings(1000);
            player_->addSkillPoints(5);
            game_.getDialog().addMessage(enemyName, "不可思议……新的王者……诞生了……这座王冠……是你的了……");
            game_.getDialog().addMessage("<SYSTEM>", "女士们先生们！让我们欢呼吧！一位新的、无可争议的世界拳王！");
            break;
            
        default:
            player_->addSavings(50);
            player_->addSkillPoints(1);
            break;
    }
    
    // 解锁下一个敌人
    player_->unlockNextEnemy(enemyId);
    
    // 显示奖励信息
    game_.getDialog().addMessage("<SYSTEM>", "当前金钱: $" + std::to_string((int)player_->getSavings()));
    game_.getDialog().addMessage("<SYSTEM>", "当前技能点: " + std::to_string((int)player_->getSkillPoints()));
}

void FightEvent::applySpecialRewards(int enemyId) {
    // 特殊奖励处理
    switch (enemyId) {
        case 4: // 健身房教练 - 解锁"健身房VIP卡"
            player_->addItemByType("拳击馆通行证", 1);
            break;
            
        case 7: // 耐力型选手 - 解锁"耐力训练器"
            player_->addItemByType("蛋白质棒", 3);
            break;
            
        case 10: // 冠军挑战者 - 解锁"冠军腰带"
            player_->addStrength(2);
            player_->addStamina(2);
            player_->addAgility(2);
            break;
            
        default:
            break;
    }
}

void FightEvent::advanceBattleState() {
    if (isBattleOver()) {
        endBattle();
        return;
    }
    
    // 切换回合
    if (battleState_ == BATTLE_STATE_PROCESSING) {
        // 根据当前状态确定下一步
        // 如果刚刚处理完玩家行动，切换到敌人回合
        battleState_ = BATTLE_STATE_ENEMY_TURN;
        
        // 延迟一下让玩家看到消息
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        processEnemyTurn();
    } else if (battleState_ == BATTLE_STATE_ENEMY_TURN) {
        // 敌人回合结束后，切换到玩家回合
        currentRound_++;
        battleState_ = BATTLE_STATE_PLAYER_TURN;
        game_.getDialog().addMessage("<SYSTEM>", "轮到了你的回合，请选择行动");
    }
}


void FightEvent::setEnemyHealthToLow() {
    // 设置敌人血量为1点
    enemy_->addHealth(-(enemy_->getHealth() - 1));
    
    // 显示状态更新
    game_.getDialog().addMessage("<SYSTEM>", "你的状态: 生命值 " + std::to_string((int)player_->getHealth()) + 
                                "/" + std::to_string((int)player_->getMaxHealth()) +
                                ", 体力 " + std::to_string((int)player_->getFatigue()) + 
                                "/" + std::to_string((int)player_->getMaxFatigue()));
    game_.getDialog().addMessage("<SYSTEM>", enemy_->getName() + "的状态: 生命值 " + 
                                std::to_string((int)enemy_->getHealth()) + "/" + 
                                std::to_string((int)enemy_->getMaxHealth()) +
                                ", 体力 " + std::to_string((int)enemy_->getFatigue()) + 
                                "/" + std::to_string((int)enemy_->getMaxFatigue()));
}
