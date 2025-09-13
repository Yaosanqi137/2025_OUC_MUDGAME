#include "TrainingEvent.h"
#include "../class/entity/Player.h"
#include <cmath>
#include <algorithm>

TrainingEvent::TrainingEvent(std::shared_ptr<Player> player_)
    : player(player_), strengthExp(0), agilityExp(0), staminaExp(0),
      strengthLevel(1), agilityLevel(1), staminaLevel(1),
      strengthExpRate(1.0), agilityExpRate(1.0), staminaExpRate(1.0) {}

bool TrainingEvent::train(TrainingType type) {
    if (!canTrain(type)) {
        return false;
    }
    
    // 扣除金币
    player->addSavings(-getTrainingCost());
    
    // 扣除体力和饱食度
    player->addFatigue(-getFatigueCost());
    player->addHunger(-getHungerCost());
    
    // 增加经验值
    double expGain = 300.0;
    
    switch (type) {
        case TrainingType::STRENGTH:
            addStrengthExp(expGain);
            break;
        case TrainingType::AGILITY:
            addAgilityExp(expGain);
            break;
        case TrainingType::STAMINA:
            addStaminaExp(expGain);
            break;
    }
    
    return true;
}

bool TrainingEvent::canTrain(TrainingType type) const {
    // 检查金币是否足够
    if (player->getSavings() < getTrainingCost()) {
        return false;
    }
    
    // 检查体力是否足够
    if (player->getFatigue() < getFatigueCost()) {
        return false;
    }
    
    // 检查饱食度是否足够
    if (player->getHunger() < getHungerCost()) {
        return false;
    }
    
    return true;
}

int TrainingEvent::getTrainingCost() const {
    return 10; // 健身房门票10金币
}

double TrainingEvent::getFatigueCost() const {
    return 25.0; // 消耗25体力值
}

double TrainingEvent::getHungerCost() const {
    return 25.0; // 消耗25饱食度
}

void TrainingEvent::applyDailyExperienceDecay() {
    double decayRate = getExperienceDecayRate();
    
    strengthExp = std::max(0.0, strengthExp * (1.0 - decayRate));
    agilityExp = std::max(0.0, agilityExp * (1.0 - decayRate));
    staminaExp = std::max(0.0, staminaExp * (1.0 - decayRate));
}

double TrainingEvent::getStrengthExp() const {
    return strengthExp;
}

double TrainingEvent::getAgilityExp() const {
    return agilityExp;
}

double TrainingEvent::getStaminaExp() const {
    return staminaExp;
}

int TrainingEvent::getStrengthLevel() const {
    return strengthLevel;
}

int TrainingEvent::getAgilityLevel() const {
    return agilityLevel;
}

int TrainingEvent::getStaminaLevel() const {
    return staminaLevel;
}

void TrainingEvent::addStrengthExp(double exp) {
    strengthExp += exp * strengthExpRate;
    checkAndLevelUp(TrainingType::STRENGTH);
}

void TrainingEvent::addAgilityExp(double exp) {
    agilityExp += exp * agilityExpRate;
    checkAndLevelUp(TrainingType::AGILITY);
}

void TrainingEvent::addStaminaExp(double exp) {
    staminaExp += exp * staminaExpRate;
    checkAndLevelUp(TrainingType::STAMINA);
}

double TrainingEvent::calculateRequiredExp(int level) const {
    return 100.0 * std::pow(1.2, level - 1);
}

void TrainingEvent::checkAndLevelUp(TrainingType type) {
    double* currentExp = nullptr;
    int* currentLevel = nullptr;
    void (Player::*addAttributeFunc)(double) = nullptr;
    
    switch (type) {
        case TrainingType::STRENGTH:
            currentExp = &strengthExp;
            currentLevel = &strengthLevel;
            addAttributeFunc = &Player::addStrength;
            break;
        case TrainingType::AGILITY:
            currentExp = &agilityExp;
            currentLevel = &agilityLevel;
            addAttributeFunc = &Player::addAgility;
            break;
        case TrainingType::STAMINA:
            currentExp = &staminaExp;
            currentLevel = &staminaLevel;
            addAttributeFunc = &Player::addStamina;
            break;
    }
    
    double requiredExp = calculateRequiredExp(*currentLevel);
    
    while (*currentExp >= requiredExp) {
        // 升级
        *currentExp -= requiredExp;
        (*currentLevel)++;
        
        // 增加对应属性
        (player.get()->*addAttributeFunc)(1.0);
        
        // 计算下一级所需经验
        requiredExp = calculateRequiredExp(*currentLevel);
    }
}

double TrainingEvent::getExperienceDecayRate() const {
    int difficulty = player->getGameDifficulty();
    
    switch (difficulty) {
        case 1: return 0.0;    // 简单：无衰减
        case 2: return 0.1;    // 普通：10%衰减
        case 3: return 0.2;    // 困难：20%衰减
        default: return 0.1;   // 默认普通难度
    }
}

double TrainingEvent::getStrengthExpRate() const { return strengthExpRate;}
double TrainingEvent::getAgilityExpRate() const { return agilityExpRate;}
double TrainingEvent::getStaminaExpRate() const { return staminaExpRate;}

void TrainingEvent::setStaminaExpRate(double rate){staminaExpRate = rate;}
void TrainingEvent::setAgilityExpRate(double rate){agilityExpRate = rate;}
void TrainingEvent::setStrengthExpRate(double rate){strengthExpRate = rate;}