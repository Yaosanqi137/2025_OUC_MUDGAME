#include "Medicine.h"
#include "../entity/Player.h"
#include <stdexcept>

// 私有构造函数
Medicine::Medicine(MedicineType type, double price, const std::string& name, 
                  const std::string& intro, double healthEffect, double strengthEffect,
                  double agilityEffect, double staminaEffect, int skillPointEffect,
                  bool hasSideEffect)
    : AbstractItem(name, intro), type(type), price(price), 
      healthEffect(healthEffect), strengthEffect(strengthEffect),
      agilityEffect(agilityEffect), staminaEffect(staminaEffect),
      skillPointEffect(skillPointEffect), hasSideEffect(hasSideEffect) {}

// 公共构造函数
Medicine::Medicine(MedicineType type) {
    *this = createMedicine(type);
}

int Medicine::getPrice() const {
    return price;
}

bool Medicine::use(Player& user) {
    // 检查是否有可用的物品
    if (amount_ <= 0) {
        return false;
    }

    // 应用主要效果
    if (healthEffect != 0) {
        user.addHealth(user.getMaxHealth() + user.getExMaxHealth());
    }
    
    if (strengthEffect != 0) {
        user.addStrength(strengthEffect);
    }
    
    if (agilityEffect != 0) {
        user.addAgility(agilityEffect);
    }
    
    if (staminaEffect != 0) {
        user.addStamina(staminaEffect);
    }
    
    if (skillPointEffect != 0) {
        user.addSkillPoints(skillPointEffect);
    }
    
    // 副作用
    if (hasSideEffect) {
        user.getTrainingSystem() -> setHasSideEffect(true);
    }
    
    // 减少物品数量
    amount_--;

    // 返回true表示成功使用
    return true;
}

Medicine::MedicineType Medicine::getMedicineType() const {
    return type;
}

Medicine Medicine::createMedicine(MedicineType type) {
    switch (type) {
        case MedicineType::REVIVAL_PILL:
            return {type, 50.0, "回生丹", "战败受伤直接恢复伤病状态（0血恢复到满血）", 
                    100000.0, 0.0, 0.0, 0.0, 0, false}; // 这里的 100000其实可以改成任何数值，弃用了
                    
        case MedicineType::STRENGTH_POTION:
            return {type, 200.0, "力量药水", "提升力量值+1（副作用：能力下降速度提升一倍）", 
                    0.0, 1.0, 0.0, 0.0, 0, true};
                    
        case MedicineType::AGILITY_POTION:
            return {type, 200.0, "敏捷药水", "提升敏捷值+1（副作用：能力下降速度提升一倍）", 
                    0.0, 0.0, 1.0, 0.0, 0, true};
                    
        case MedicineType::STAMINA_POTION:
            return {type, 200.0, "耐力药水", "提升耐力值+1（副作用：能力下降速度提升一倍）", 
                    0.0, 0.0, 0.0, 1.0, 0, true};
                    
        case MedicineType::SKILL_POINT_POTION:
            return {type, 100.0, "技能点药水", "获得1个技能点", 
                    0.0, 0.0, 0.0, 0.0, 1, false};
                    
        default:
            throw std::invalid_argument("Unknown medicine type");
    }
}

double Medicine::getHealthEffect() const {
    return healthEffect;
}

double Medicine::getStrengthEffect() const {
    return strengthEffect;
}

double Medicine::getAgilityEffect() const {
    return agilityEffect;
}

double Medicine::getStaminaEffect() const {
    return staminaEffect;
}

int Medicine::getSkillPointEffect() const {
    return skillPointEffect;
}