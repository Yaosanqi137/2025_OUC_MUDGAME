#include "TrainingEvent.h"
#include "../class/entity/Player.h"
#include <cmath>
#include <algorithm>
#include "../basic/Game.h"
#include "../basic/Dialog.h"
TrainingEvent::TrainingEvent(std::shared_ptr<Player> player_)
    : player(player_), strengthExp(0), agilityExp(0), staminaExp(0),
      strengthLevel(1), agilityLevel(1), staminaLevel(1),
      strengthExpRate(1.0), agilityExpRate(1.0), staminaExpRate(1.0),
      hasSideEffect(false) {}


std::string TrainingEvent::getTrainingStatus() const {
    std::string status = "=== 训练状态 ===\n";
    status += "力量训练: 等级 " + std::to_string(getStrengthLevel()) + 
             ", 经验: " + std::to_string((int)getStrengthExp()) + "\n";
    status += "敏捷训练: 等级 " + std::to_string(getAgilityLevel()) + 
             ", 经验: " + std::to_string((int)getAgilityExp()) + "\n";
    status += "耐力训练: 等级 " + std::to_string(getStaminaLevel()) + 
             ", 经验: " + std::to_string((int)getStaminaExp());
    return status;
}

std::string TrainingEvent::getTrainingHelp() {
    return "=== 训练指令帮助 ===\n"
           "/train status - 查看训练状态\n"
           "/train strength - 进行力量训练\n"
           "/train agility - 进行敏捷训练\n"
           "/train stamina - 进行耐力训练\n"
           "训练消耗: 10金币, 25体力, 25饱食度\n"
           "训练收益: 300对应属性经验";
}


bool TrainingEvent::train(TrainingType type, Game& game) {
    if (!canTrain(type)) {
        sendTrainingFailureMessage(game, type);
        return false;
    }
    
    // 记录当前等级，用于检测升级
    int oldStrengthLevel = strengthLevel;
    int oldAgilityLevel = agilityLevel;
    int oldStaminaLevel = staminaLevel;
    
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
    
    sendTrainingSuccessMessage(game, type);
    
    // 检查并发送升级消息
    switch (type) {
        case TrainingType::STRENGTH:
            if (strengthLevel > oldStrengthLevel) {
                sendLevelUpMessage(game, type, strengthLevel);
                game.getDialog().addMessage("<SYESTEM>", "力量提升了" + std::to_string(strengthLevel - oldStrengthLevel) + "点");
            }
            break;
        case TrainingType::AGILITY:
            if (agilityLevel > oldAgilityLevel) {
                sendLevelUpMessage(game, type, agilityLevel);
                game.getDialog().addMessage("<SYESTEM>", "敏捷提升了" + std::to_string(agilityLevel - oldStrengthLevel) + "点");
            }
            break;
        case TrainingType::STAMINA:
            if (staminaLevel > oldStaminaLevel) {
                sendLevelUpMessage(game, type, staminaLevel);
                game.getDialog().addMessage("<SYESTEM>", "耐力提升了" + std::to_string(staminaLevel - oldStrengthLevel) + "点");
            }
            break;
    }
    
    // 显示当前经验和下一级所需经验
    game.getDialog().addMessage("<SYSTEM>", getNextLevelExpInfo(type));
    
    return true;
}

void TrainingEvent::sendTrainingSuccessMessage(Game& game, TrainingType type) const {
    std::string message;
    
    switch (type) {
        case TrainingType::STRENGTH:
            message = "力量训练完成！获得300力量经验";
            break;
        case TrainingType::AGILITY:
            message = "敏捷训练完成！获得300敏捷经验";
            break;
        case TrainingType::STAMINA:
            message = "耐力训练完成！获得300耐力经验";
            break;
    }
    
    game.getDialog().addMessage("<SYSTEM>", message);
}

void TrainingEvent::sendTrainingFailureMessage(Game& game, TrainingType type) const {
    std::string message;
    
    switch (type) {
        case TrainingType::STRENGTH:
            message = "力量训练";
            break;
        case TrainingType::AGILITY:
            message = "敏捷训练";
            break;
        case TrainingType::STAMINA:
            message = "耐力训练";
            break;
    }
    
    message += "失败！可能的原因: ";
    
    if (player->getSavings() < getTrainingCost()) {
        message += "金币不足";
    } else if (player->getFatigue() < getFatigueCost()) {
        message += "体力不足";
    } else if (player->getHunger() < getHungerCost()) {
        message += "饱食度不足";
    }
    
    game.getDialog().addMessage("<SYSTEM>", message);
}

void TrainingEvent::sendLevelUpMessage(Game& game, TrainingType type, int newLevel) const {
    std::string message;
    std::string attributeName;
    
    switch (type) {
        case TrainingType::STRENGTH:
            attributeName = "力量";
            break;
        case TrainingType::AGILITY:
            attributeName = "敏捷";
            break;
        case TrainingType::STAMINA:
            attributeName = "耐力";
            break;
    }
    
    message = "恭喜！" + attributeName + "提升到 " + std::to_string(newLevel) + " 级！";
    game.getDialog().addMessage("<SYSTEM>", message);
}

std::string TrainingEvent::getNextLevelExpInfo(TrainingType type) const {
    std::stringstream ss;
    double currentExp = 0;
    double requiredExp = 0;
    std::string attributeName;
    
    switch (type) {
        case TrainingType::STRENGTH:
            currentExp = strengthExp;
            requiredExp = calculateRequiredExp(strengthLevel);
            attributeName = "力量";
            break;
        case TrainingType::AGILITY:
            currentExp = agilityExp;
            requiredExp = calculateRequiredExp(agilityLevel);
            attributeName = "敏捷";
            break;
        case TrainingType::STAMINA:
            currentExp = staminaExp;
            requiredExp = calculateRequiredExp(staminaLevel);
            attributeName = "耐力";
            break;
    }
    
    ss << attributeName << "经验: " << (int)currentExp << "/" << (int)requiredExp;
    return ss.str();
}

// 修改 checkAndLevelUp 方法，使其返回是否升级
bool TrainingEvent::checkAndLevelUp(TrainingType type) {
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
    bool leveledUp = false;
    
    while (*currentExp >= requiredExp) {
        // 升级
        *currentExp -= requiredExp;
        (*currentLevel)++;
        leveledUp = true;
        
        // 增加对应属性
        (player.get()->*addAttributeFunc)(1.0);
        
        // 计算下一级所需经验
        requiredExp = calculateRequiredExp(*currentLevel);
    }
    
    return leveledUp;
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

bool TrainingEvent::addStrengthExp(double exp) {
    strengthExp += exp * strengthExpRate;
    return checkAndLevelUp(TrainingType::STRENGTH);
}

bool TrainingEvent::addAgilityExp(double exp) {
    agilityExp += exp * agilityExpRate;
    return checkAndLevelUp(TrainingType::AGILITY);
}

bool TrainingEvent::addStaminaExp(double exp) {
    staminaExp += exp * staminaExpRate;
    return checkAndLevelUp(TrainingType::STAMINA);
}

double TrainingEvent::calculateRequiredExp(int level) const {
    return 100.0 * std::pow(1.2, level - 1);
}


double TrainingEvent::getExperienceDecayRate() const {
    int difficulty = player->getGameDifficulty();
    double exDecay = 0.5;
    if(hasSideEffect){
        switch (difficulty) {
        case 1: return 0.0 + exDecay;    // 简单：无衰减 + 嗑药 50% 衰减
        case 2: return 0.1 + exDecay;    // 普通：10% 衰减 + 嗑药 50% 衰减
        case 3: return 0.2 + exDecay;    // 困难：20% 衰减 + 嗑药 50% 衰减
        default: return 0.1 + exDecay;   // 默认普通难度 + 嗑药 50% 衰减
        }
    }
    else{
        switch (difficulty) {
            case 1: return 0.0;    // 简单：无衰减
            case 2: return 0.1;    // 普通：10% 衰减
            case 3: return 0.2;    // 困难：20% 衰减
            default: return 0.1;   // 默认普通难度
        }
    }
}

double TrainingEvent::getStrengthExpRate() const { return strengthExpRate;}
double TrainingEvent::getAgilityExpRate() const { return agilityExpRate;}
double TrainingEvent::getStaminaExpRate() const { return staminaExpRate;}

void TrainingEvent::setStaminaExpRate(double rate){staminaExpRate = rate;}
void TrainingEvent::setAgilityExpRate(double rate){agilityExpRate = rate;}
void TrainingEvent::setStrengthExpRate(double rate){strengthExpRate = rate;}

bool TrainingEvent::getHasSideEffect() const {return hasSideEffect;}
void TrainingEvent::setHasSideEffect(bool flag) {hasSideEffect = flag;}