#include "Medicine.h"
#include "../entity/Player.h"
#include <stdexcept>

// 私有构造函数
Medicine::Medicine(MedicineType type, double price, const std::string& name, 
                  const std::string& intro, double healthEffect, double strengthEffect,
                  double agilityEffect, double staminaEffect, int skillPointEffect,
                  bool hasSideEffect)
    : AbstractItem(name, intro), type(type), price(static_cast<int>(price)), 
      healthEffect(healthEffect), strengthEffect(strengthEffect),
      agilityEffect(agilityEffect), staminaEffect(staminaEffect),
      skillPointEffect(skillPointEffect), hasSideEffect(hasSideEffect),
      amount_(1) {}  // 默认数量为1

// 公共构造函数
Medicine::Medicine(MedicineType type) {
    *this = createMedicine(type);
}

// 实现抽象接口：获取物品名称
const std::string& Medicine::getName() const {
    return AbstractItem::getName();
}

// 实现抽象接口：获取物品价格
int Medicine::getPrice() const {
    return price;
}

// 实现抽象接口：使用物品
void Medicine::use(Player& user) {
    // 应用主要效果
    if (healthEffect > 0) {
        user.recoverAllWounds();      // 恢复所有伤病
        user.setHealthToMax();        // 生命值回满
        user.clearNegativeStatus();   // 清除负面状态
    }
    
    if (strengthEffect != 0) {
        user.increaseStrength(static_cast<int>(strengthEffect));
    }
    
    if (agilityEffect != 0) {
        user.increaseAgility(static_cast<int>(agilityEffect));
    }
    
    if (staminaEffect != 0) {
        user.increaseEndurance(static_cast<int>(staminaEffect));
    }
    
    if (skillPointEffect != 0) {
        user.addSkillPoints(skillPointEffect);
    }
    
    // 副作用处理
    if (hasSideEffect) {
        user.setDecayRateMultiplier(2.0);
        user.getTrainingSystem()->setHasSideEffect(true);
    }
    
    // 减少物品数量
    if (amount_ > 0) {
        amount_--;
    }
}

// 获取药物类型
Medicine::MedicineType Medicine::getType() const {
    return type;
}

// 创建药物的工厂方法
Medicine Medicine::createMedicine(MedicineType type) {
    switch (type) {
        case MedicineType::WOUND_RECOVERY:
            return {type, 50.0, "创伤愈合剂", "战败受伤直接恢复伤病状态（0血恢复到满血）", 
                    1.0, 0.0, 0.0, 0.0, 0, false};
                    
        case MedicineType::STRENGTH_BOOST:
            return {type, 200.0, "力量强化剂", "提升力量值+1（副作用：能力下降速度提升一倍）", 
                    0.0, 1.0, 0.0, 0.0, 0, true};
                    
        case MedicineType::AGILITY_BOOST:
            return {type, 200.0, "敏捷强化剂", "提升敏捷值+1（副作用：能力下降速度提升一倍）", 
                    0.0, 0.0, 1.0, 0.0, 0, true};
                    
        case MedicineType::ENDURANCE_BOOST:
            return {type, 200.0, "耐力强化剂", "提升耐力值+1（副作用：能力下降速度提升一倍）", 
                    0.0, 0.0, 0.0, 1.0, 0, true};
                    
        case MedicineType::SKILL_POINT:
            return {type, 100.0, "技能点药剂", "获得1个技能点", 
                    0.0, 0.0, 0.0, 0.0, 1, false};
                    
        default:
            throw std::invalid_argument("Unknown medicine type");
    }
}

// 各类效果的getter方法
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

// 物品数量管理
int Medicine::getAmount() const {
    return amount_;
}

void Medicine::setAmount(int amount) {
    if (amount >= 0) {
        amount_ = amount;
    }
}
