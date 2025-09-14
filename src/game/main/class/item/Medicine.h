#include "Medicine.h"
#include "Player.h"
#include <stdexcept>

// 私有构造函数实现
Medicine::Medicine(MedicineType type, double price, const std::string& name, 
                 const std::string& intro, double healthEffect, double strengthEffect,
                 double agilityEffect, double staminaEffect, int skillPointEffect,
                 bool hasSideEffect)
    : AbstractItem(name, intro),  // 调用父类构造函数
      type(type),
      price(price),
      healthEffect(healthEffect),
      strengthEffect(strengthEffect),
      agilityEffect(agilityEffect),
      staminaEffect(staminaEffect),
      skillPointEffect(skillPointEffect),
      hasSideEffect(hasSideEffect) {}

// 公共构造函数实现
Medicine::Medicine(MedicineType type) : AbstractItem("", "") {
    // 这里可以设置默认值，实际应该通过createMedicine创建
    this->type = type;
    this->price = 0;
    this->healthEffect = 0;
    this->strengthEffect = 0;
    this->agilityEffect = 0;
    this->staminaEffect = 0;
    this->skillPointEffect = 0;
    this->hasSideEffect = false;
}

// 获取价格
int Medicine::getPrice() const {
    return static_cast<int>(price);
}

// 使用药物
void Medicine::use(Player& user) {
    // 应用主要效果
    if (healthEffect > 0) {
        user.increaseHealth(healthEffect);
    }
    if (strengthEffect > 0) {
        user.increaseStrength(strengthEffect);
    }
    if (agilityEffect > 0) {
        user.increaseAgility(agilityEffect);
    }
    if (staminaEffect > 0) {
        user.increaseStamina(staminaEffect);
    }
    if (skillPointEffect > 0) {
        user.addSkillPoints(skillPointEffect);
    }

    // 处理副作用
    if (hasSideEffect) {
        // 根据药物类型添加相应的副作用
        switch (type) {
            case MedicineType::STRENGTH_POTION:
            case MedicineType::AGILITY_POTION:
            case MedicineType::STAMINA_POTION:
                user.setDecayRateMultiplier(2.0);  // 能力下降速度加倍
                break;
            default:
                break;
        }
    }
}

// 获取药物类型
Medicine::MedicineType Medicine::getMedicineType() const {
    return type;
}

// 创建药物的静态工厂方法
Medicine Medicine::createMedicine(MedicineType type) {
    switch (type) {
        case MedicineType::REVIVAL_PILL:
            return Medicine(
                type, 
                50.0, 
                "回生丹", 
                "恢复所有伤病并回满生命值，清除负面状态",
                100.0,  // 生命值效果
                0.0,    // 力量效果
                0.0,    // 敏捷效果
                0.0,    // 耐力效果
                0,      // 技能点效果
                false   // 无副作用
            );
        case MedicineType::STRENGTH_POTION:
            return Medicine(
                type, 
                200.0, 
                "力量药水", 
                "永久提升1点力量，但会使能力下降速度加倍",
                0.0,    // 生命值效果
                1.0,    // 力量效果
                0.0,    // 敏捷效果
                0.0,    // 耐力效果
                0,      // 技能点效果
                true    // 有副作用
            );
        case MedicineType::AGILITY_POTION:
            return Medicine(
                type, 
                200.0, 
                "敏捷药水", 
                "永久提升1点敏捷，但会使能力下降速度加倍",
                0.0,    // 生命值效果
                0.0,    // 力量效果
                1.0,    // 敏捷效果
                0.0,    // 耐力效果
                0,      // 技能点效果
                true    // 有副作用
            );
        case MedicineType::STAMINA_POTION:
            return Medicine(
                type, 
                200.0, 
                "耐力药水", 
                "永久提升1点耐力，但会使能力下降速度加倍",
                0.0,    // 生命值效果
                0.0,    // 力量效果
                0.0,    // 敏捷效果
                1.0,    // 耐力效果
                0,      // 技能点效果
                true    // 有副作用
            );
        case MedicineType::SKILL_POINT_POTION:
            return Medicine(
                type, 
                100.0, 
                "技能点药水", 
                "增加1个技能点",
                0.0,    // 生命值效果
                0.0,    // 力量效果
                0.0,    // 敏捷效果
                0.0,    // 耐力效果
                1,      // 技能点效果
                false   // 无副作用
            );
        default:
            throw std::invalid_argument("未知的药物类型");
    }
}

// 各种效果的getter方法
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
