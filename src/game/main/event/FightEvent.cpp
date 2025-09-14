/*

Changed on 9-14 2:23 by Anyeling
回合制战斗机制未完成
系统提示未完成
时间流逝未完成

*/


#include "FightEvent.h"
#include "../basic/Dialog.h"
#include "../basic/Game.h"
#include <iostream>
#include <random>
#include <algorithm>

bool humanHummerRecover = false;
FightEvent::FightEvent(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy)
    : player_(player), enemy_(enemy), battleOver_(false), playerWon_(false),
      playerTurn_(false), currentRound_(0) {

    humanHummerRecover = false;

    // 配置敌人技能
    configureEnemySkills();
    
    // 初始化随机数种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
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

void FightEvent::startBattle(Game& game) {
    game.getDialog().addMessage("<SYSTEM>", "战斗开始了，对手是" + enemy_ -> getName());
    battleOver_ = false;
    playerWon_ = false;
    currentRound_ = 0;
    
    // 特殊技能34 人身重锤
    for (auto& skill : player_->getSkills()) {
        if (skill->getSkillName() == "人身重锤") {
            int roll_ = std::rand() % 100; 
            if(roll_ < 25)
                enemy_ -> addFatigue(-10.0);
            }
    }
    for (auto& skill : enemy_->getSkills()) {
        if (skill->getSkillName() == "人身重锤") {
            int roll_ = std::rand() % 100; 
            if(roll_ < 25)
                player_ -> addFatigue(-10.0);
                humanHummerRecover = true;
            }
    }

    // 决定先手：比较玩家和敌人的速度
    double playerSpeed = player_->getSpeed();
    double enemySpeed = enemy_->getSpeed();
    
    playerTurn_ = (playerSpeed > enemySpeed);
    if(playerSpeed == enemySpeed) {
        int roll = std::rand() % 2;
        playerTurn_ = (roll == 0);
    }
    
    // 开始第一回合
    currentRound_ = 1;

    
    if(playerTurn_){
        game.getDialog().addMessage("<SYSTEM>", "轮到了你的回合，请选择你要做出的选择");
        std::string input ;
        playerChooseAction(TurnAction::SKILL, 0);
    }
    else{

    }
}

void FightEvent::endBattle() {
    battleOver_ = true;
    
    // 确定胜利者
    if (player_->getHealth() <= 0) {
        playerWon_ = false;
    } else if (enemy_->getHealth() <= 0) {
        playerWon_ = true;
        applyBattleRewards();
    }
}

bool FightEvent::isBattleOver() const {
    if(battleOver_ || player_->getHealth() <= 0 || enemy_->getHealth() <= 0){
        if(humanHummerRecover)
            player_ -> addFatigue(10);
        return true;
    }
    return false;
}

bool FightEvent::isPlayerWinner() const {
    return playerWon_;
}

bool FightEvent::isEnemyWinner() const {
    return !playerWon_ && battleOver_;
}

bool FightEvent::isPlayerTurn() const {
    return playerTurn_;
}

int FightEvent::getCurrentRound() const {
    return currentRound_;
}

void FightEvent::playerChooseAction(TurnAction action, int skillIndex) {
    if (battleOver_ || !playerTurn_) {
        return;
    }
    
    switch (action) {
        case TurnAction::SKILL:
            if (skillIndex >= 0 && skillIndex < static_cast<int>(player_->getSkills().size())) {
                processPlayerSkill(skillIndex);
            }
            break;
            
        case TurnAction::SKIP:
            processPlayerSkip();
            break;
            
        case TurnAction::NONE:
            // 不做任何操作
            break;
    }
    
    // 检查战斗是否结束
    if (isBattleOver()) {
        endBattle();
        return;
    }
    
    // 切换到敌人回合
    playerTurn_ = false;
    processEnemyTurn();
    
    // 检查战斗是否结束
    if (isBattleOver()) {
        endBattle();
        return;
    }
    
    // 切换到下一回合玩家回合
    playerTurn_ = true;
    currentRound_++;
}

void FightEvent::processEnemyTurn() {
    if (battleOver_ || playerTurn_) {
        return;
    }
    
    // 敌人AI：可以选择使用技能或跳过
    // 简单策略：70%概率使用技能，30%概率跳过
    auto& enemySkills = enemy_->getSkills();
    if (!enemySkills.empty() && (std::rand() % 100) < 70) {
        // 随机选择一个技能
        int skillIndex = std::rand() % enemySkills.size();
        auto skill = enemySkills[skillIndex];
        if(skill -> canUse(*enemy_)) {
            skill -> execute(*enemy_, *player_);
            return;
        }
        else 
        {
            processEnemySkip();
            return;
        }
        // 计算伤害
        double damage = skill->calculateDamage(enemy_->getStrength());
        double hitRate = skill->calculateHitRate(enemy_->getAgility(), 
                                               enemy_->getStrength(), 
                                               enemy_->getStamina());
        
        if (checkHit(hitRate)) {
            applyDamageToPlayer(damage);
            
            // 检查玩家是否被击倒（耐力透支）
            double staminaCost = skill->calculateStaminaCost(enemy_->getStrength());
            if (player_->getFatigue() < staminaCost) {
                processKnockdown(player_, 20.0); // 玩家被击倒，恢复20%体力
            }
        }
        
        // 消耗敌人体力
        double staminaCost = skill->calculateStaminaCost(enemy_->getStrength());
        enemy_->addFatigue(-staminaCost);
    } else {
        // 敌人跳过回合，恢复20%体力
        processEnemySkip();
    }
    
    // 检查敌人是否被击倒（在玩家回合中处理）
}

bool FightEvent::checkHit(double hitRate) const {
    double roll = (std::rand() % 10000) / 10000.0; // 0.0 到 100.0
    return roll <= hitRate;
}

void FightEvent::applyDamageToEnemy(double damage) {
    enemy_->addHealth(-damage);
}

void FightEvent::applyDamageToPlayer(double damage) {
    player_->addHealth(-damage);
}

void FightEvent::processPlayerSkill(int skillIndex) {
    auto& playerSkills = player_->getSkills();
    if (skillIndex < 0 || skillIndex >= static_cast<int>(playerSkills.size())) {
        return;
    }
    
    auto skill = playerSkills[skillIndex];
    
    // 检查技能是否可用
    if (!skill->canUse(*player_)) {
        /*
        
        提示技能不可用
        
        */
        return;
    }
    


    // 计算伤害
    double damage = skill->calculateDamage(player_->getStrength());
    for(auto& skill__ : player_->getSkills()){
        // 查找玩家是否有这个特殊技能
        if(skill__ -> getSkillName() == "千手不破"){
            std::set<int> upperSkillIndex = {1,4,5,6,7,8,11,12,13,14,15};
            std::set<int> lowerSkillIndex = {2,3,9,10};
            // 上肢伤害乘受伤倍数
            if(upperSkillIndex.find(skillIndex) != upperSkillIndex.end())
                damage *= enemy_ -> getUpperBodySustainDamageRate();
            // 下肢伤害乘受伤倍数
            else if(lowerSkillIndex.find(skillIndex) != lowerSkillIndex.end())
                damage *= enemy_ -> getLowerBodySustainDamageRate();
        }
    }
    double hitRate = skill->calculateHitRate(player_->getAgility(), 
                                           player_->getStrength(), 
                                           player_->getStamina());
    // ExHitRate 由特殊技能而得
    hitRate += player_ -> getExHitRate();
    hitRate = std::max(0.0,hitRate);
    hitRate = std::min(1.0,hitRate); 
    if (checkHit(hitRate)) {
        applyDamageToEnemy(damage);
        
        // 特殊技能 31号技能"闪击"
        for(auto& skill__ : enemy_->getSkills()){
            if(skill__ -> getSkillName() == "闪击"){
                double roll = std::rand() % 100;
                if(roll < 25){
                    applyDamageToPlayer(damage * 0.25);
                }
            }
        }

        
        double staminaCost = skill->calculateStaminaCost(player_->getStrength());
        if (enemy_-> getFatigue() < staminaCost) {
            processKnockdown(enemy_, 20.0); // 敌人被击倒，恢复20%体力(后面这个参数其实没用)
        }
    }
    
    // 计算玩家体力
    double staminaCost = skill->calculateStaminaCost(player_->getStrength());
    

    // 36 是 "无限能量"
    if((19 <= skillIndex && skillIndex <= 30) || skillIndex == 36){
        if(skillIndex == 36 && player_ -> getHealth() < (player_ -> getMaxHealth() + player_ -> getExMaxHealth()))
            staminaCost *= (1 - 0.15);
        // 消耗玩家体力
        player_->addFatigue(-staminaCost);
        skill->execute(*player_, *player_); 
    }
    else {
        // 消耗玩家体力
        player_->addFatigue(-staminaCost);
        skill->execute(*player_, *enemy_); 
    }
}

void FightEvent::processPlayerSkip() {
    // 跳过回合恢复20%体力
    double recoveryAmount = player_->getMaxFatigue() * 0.2;
    player_->addFatigue(recoveryAmount);
}

void FightEvent::processEnemySkip() {
    // 敌人跳过回合恢复20%体力
    double recoveryAmount = enemy_->getMaxFatigue() * 0.2; 
    enemy_->addFatigue(recoveryAmount);
}

void FightEvent::processKnockdown(std::shared_ptr<Player> target, double recoveryPercent) {
    // 玩家被击倒：损失20%血量，恢复一定百分比体力
    target->addHealth(-target->getHealth() * 0.2);
    target->addFatigue(target->getFatigue() * 0.2);
}

void FightEvent::processKnockdown(std::shared_ptr<Enemy> target, double recoveryPercent) {
    // 敌人被击倒：损失20%血量，恢复一定百分比体力
    target->addHealth(-target->getHealth() * 0.2);
    target->addFatigue(target->getFatigue() * 0.2); 
}

void FightEvent::applyBattleRewards() {
    if (!playerWon_) {
        return;
    }
    
    int enemyId = enemy_->getId();
    
    // 基础奖励
    switch (enemyId) {
        case 1: // 业余拳手
            player_->addSavings(0); // 教学战无奖励
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
            player_ -> getTrainingSystem() -> setMoneyCostRate(0.5);
            player_ -> getGameLogic().getDialog().addMessage("<SYSTEM>","获得健身房VIP卡！训练费用减半！");
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
            // applySpecialRewards(enemyId);
            player_ -> getTrainingSystem() -> setStaminaExpRate(player_ -> getTrainingSystem() -> getStaminaExpRate() + 0.2);
            player_ -> getGameLogic().getDialog().addMessage("<SYSTEM>","获得耐力训练器！耐力训练效率+20%！");
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
            // applySpecialRewards(enemyId);
            player_->addStrength(2);
            player_->addStamina(2);
            player_->addAgility(2);
            player_ -> getGameLogic().getDialog().addMessage("<SYSTEM>","获得冠军腰带！全属性+2！");
            break;
            
        case 11: // 世界拳王
            player_->addSavings(1000);
            player_->addSkillPoints(5);
            break;
            
        default:
            player_->addSavings(50);
            player_->addSkillPoints(1);
            break;
    }
    
    // 解锁下一个敌人
    player_->unlockNextEnemy(enemyId);
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