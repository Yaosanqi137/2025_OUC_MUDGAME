#include "Skill.h"
#include "../entity/Player.h"
#include "../entity/Enemy.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <map>


//

// excute函数未完工，已经趋近完成

//
// 技能ID映射表
static const std::map<std::string, int> skillNameToIdMap = {
    {"直拳", 1}, {"踢腿", 2}, {"高踢腿", 3}, {"重拳", 4}, {"上钩拳", 5},
    {"反手重拳", 6}, {"反手直拳", 7}, {"交叉拳", 8}, {"空手道踢腿", 9},
    {"空手道高踢腿", 10}, {"爪击", 11}, {"折背", 12}, {"拳术直拳", 13},
    {"蓄力上勾拳", 14}, {"空手道劈斩", 15}, {"迅击", 16}, {"近战缠斗", 17},
    {"长拳", 18}, {"屹立不倒", 19}, {"肉食跑者", 20}, {"肌肉记忆", 21},
    {"屹立不倒2", 22}, {"肉食跑者2", 23}, {"肌肉记忆2", 24}, {"屹立不倒3", 25},
    {"肉食跑者3", 26}, {"肌肉记忆3", 27}, {"迅速", 28}, {"意志", 29}, {"激励", 30},
    {"闪击", 31}, {"自杀式袭击", 32}, {"千手不破", 33}, {"人身重锤", 34},
    {"钝兵挫锐", 35}, {"无限能量", 36}, {"拳击手", 37}
};

// ========== 构造函数实现 ==========
Skill::Skill(int id, const std::string& name, const std::string& desc, 
             SkillEffectType type, TargetType targetType, int cost,
             double baseDmg, double dmgCoeff, double baseHit, double hitCoeff,
             double baseStamina, double staminaCoeff, double minValue,
             const std::string& attrType, double multiplier, double value)
    : id(id), skillName(name), description(desc), effectType(type),
      targetType(targetType), isLocked(true), unlockCost(cost),
      baseDamage(baseDmg), damageCoefficient(dmgCoeff),
      baseHitRate(baseHit), hitRateCoefficient(hitCoeff),
      baseStaminaCost(baseStamina), staminaCostCoefficient(staminaCoeff),
      minAttributeValue(minValue), attributeType(attrType),
      trainingMultiplier(multiplier), effectValue(value) {}

// ========== Getter 实现 ==========
int Skill::getId() const { return id; }
std::string Skill::getSkillName() const { return skillName; }
std::string Skill::getDescription() const { return description; }
SkillEffectType Skill::getEffectType() const { return effectType; }
TargetType Skill::getTargetType() const { return targetType; }
int Skill::getUnlockCost() const { return unlockCost; }
bool Skill::getIsLocked() const { return isLocked; }

double Skill::getBaseDamage() const { return baseDamage; }
double Skill::getDamageCoefficient() const { return damageCoefficient; }
double Skill::getBaseHitRate() const { return baseHitRate; }
double Skill::getHitRateCoefficient() const { return hitRateCoefficient; }
double Skill::getBaseStaminaCost() const { return baseStaminaCost; }
double Skill::getStaminaCostCoefficient() const { return staminaCostCoefficient; }

double Skill::getMinAttributeValue() const { return minAttributeValue; }
std::string Skill::getAttributeType() const { return attributeType; }
double Skill::getTrainingMultiplier() const { return trainingMultiplier; }
double Skill::getEffectValue() const { return effectValue; }

// ========== 计算公式实现 ==========
double Skill::calculateDamage(double strength) const {
    return baseDamage + strength * damageCoefficient;
}

double Skill::calculateHitRate(double agility, double strength, double stamina) const {
    double total = strength + agility + stamina;
    double Y = (total > 0) ? (agility / total) * 3.0 : 0.0;
    return baseHitRate + hitRateCoefficient * Y;
}

double Skill::calculateStaminaCost(double strength) const {
    return baseStaminaCost + strength * staminaCostCoefficient;
}

// ========== 解锁相关方法实现 ==========
void Skill::unlock() { isLocked = false; }
void Skill::lock() { isLocked = true; }

bool Skill::canUnlock(int playerSkillPoints) const {
    return playerSkillPoints >= unlockCost;
}

// ========== 使用条件检查（重载版本） ==========
bool Skill::canUse(const Player& user) const {
    if (isLocked) return false;
    
    double requiredStamina = calculateStaminaCost(user.getStrength());
    if (user.getFatigue() < requiredStamina) return false;

    if (effectType == SkillEffectType::ATTRIBUTE_BOOST) {
        if (attributeType == "strength" && user.getStrength() < minAttributeValue) return false;
        if (attributeType == "agility" && user.getAgility() < minAttributeValue) return false;
        if (attributeType == "stamina" && user.getStamina() < minAttributeValue) return false;
    }

    return true;
}

bool Skill::canUse(const Enemy& user) const {
    if (isLocked) return false;
    
    double requiredStamina = calculateStaminaCost(user.getStrength());
    if (user.getFatigue() < requiredStamina) return false;

    // 敌人没有增益类技能

    //
    return true;
}

// ========== 执行技能（重载版本） ==========
void Skill::execute(Player& user, Player& target) {
    if (!canUse(user)) return;
    
    double staminaCost = calculateStaminaCost(user.getStrength());
    user.addFatigue(-staminaCost);
    
    // 根据不同效果类型执行
    switch (effectType) {
        case SkillEffectType::ATTRIBUTE_BOOST: {
            if (attributeType == "strength") target.setMinStrength(std::max(target.getMinStrength(), minAttributeValue));
            else if (attributeType == "agility") target.setMinAgility(std::max(target.getMinAgility(), minAttributeValue));
            else if (attributeType == "stamina") target.setMinStamina(std::max(target.getMinStamina(), minAttributeValue));
            break;
        }
        case SkillEffectType::TRAINING_EFFICIENCY:{
            if(attributeType == "strength")
                target.getTrainingSystem() -> setStrengthExpRate(trainingMultiplier);
            else if(attributeType == "agility")
                target.getTrainingSystem() -> setAgilityExpRate(trainingMultiplier);
            else if(attributeType == "stamina")
                target.getTrainingSystem() -> setStaminaExpRate(trainingMultiplier);
            break;
        }

        default: break;
    }
}   

void Skill::execute(Player& user, Enemy& target) {
    if (!canUse(user)) return;
    
    double staminaCost = calculateStaminaCost(user.getStrength());
    user.addFatigue(-staminaCost);
    
    if (effectType == SkillEffectType::BASIC_ATTACK) {
        double damage = calculateDamage(user.getStrength());
        double hitRate = calculateHitRate(user.getAgility(), user.getStrength(), user.getStamina());
        hitRate += user.getExHitRate();     // 有特殊技能会增加命中率
        hitRate = std::min(hitRate, 1.0);
        if ((std::rand() % 10000) <= hitRate * 10000) {
            target.addHealth(-damage);
        }
    }
}

void Skill::execute(Enemy& user, Player& target) {
    if (!canUse(user)) return;
    
    double staminaCost = calculateStaminaCost(user.getStrength());
    user.addFatigue(-staminaCost);
    
    if (effectType == SkillEffectType::BASIC_ATTACK) {
        double damage = calculateDamage(user.getStrength());
        double hitRate = calculateHitRate(user.getAgility(), user.getStrength(), user.getStamina());
        
        if ((std::rand() % 100) <= hitRate) {
            target.addHealth(-damage);
        }
    }
}

void Skill::execute(Enemy& user, Enemy& target) {
    if (!canUse(user)) return;
    
    double staminaCost = calculateStaminaCost(user.getStrength());
    user.addFatigue(-staminaCost);
    
    if (effectType == SkillEffectType::BASIC_ATTACK) {
        double damage = calculateDamage(user.getStrength());
        double hitRate = calculateHitRate(user.getAgility(), user.getStrength(), user.getStamina());
        
        if ((std::rand() % 100) <= hitRate) {
            target.addHealth(-damage);
        }
    }
}

// ========== 类型判断 ==========
bool Skill::isAttackSkill() const { 
    return effectType == SkillEffectType::BASIC_ATTACK; 
}

bool Skill::isAttributeBoostSkill() const { 
    return effectType == SkillEffectType::ATTRIBUTE_BOOST; 
}

bool Skill::isTrainingSkill() const { 
    return effectType == SkillEffectType::TRAINING_EFFICIENCY; 
}

bool Skill::isSpecialEffectSkill() const {
    return effectType != SkillEffectType::BASIC_ATTACK &&
           effectType != SkillEffectType::ATTRIBUTE_BOOST &&
           effectType != SkillEffectType::TRAINING_EFFICIENCY;
}

// ========== 工具方法 ==========
int Skill::skillNameToID(const std::string& skillName) {
    auto it = skillNameToIdMap.find(skillName);
    return (it != skillNameToIdMap.end()) ? it->second : -1;
}

std::string Skill::idToSkillName(int id) {
    for (const auto& pair : skillNameToIdMap) {
        if (pair.second == id) return pair.first;
    }
    return "未知技能";
}