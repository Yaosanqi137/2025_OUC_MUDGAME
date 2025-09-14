#include "Food.h"
#include "../entity/Player.h"
#include <stdexcept>
#include <string>

// 私有构造函数：整合两个版本的参数，初始化基类和成员变量
Food::Food(FoodType type, double price, const std::string& name, const std::string& intro, 
           double healthEffect, double hungerEffect, double energyEffect)
    : AbstractItem(name, intro), type_(type), price_(price), 
      healthEffect_(healthEffect), hungerEffect_(hungerEffect), energyEffect_(energyEffect) {
    // 完善物品描述，补充效果信息
    intro_ += " 使用后恢复饱食度 " + std::to_string(static_cast<int>(hungerEffect_)) + " 点，" +
              "恢复体力 " + std::to_string(static_cast<int>(energyEffect_)) + " 点，" +
              "恢复生命值 " + std::to_string(static_cast<int>(healthEffect_)) + " 点";
}

// 公共构造函数：根据食物类型创建对应实例
Food::Food(FoodType type) {
    switch (type) {
        case FoodType::MEAT:
            *this = createStoreFood(FoodType::MEAT);
            break;
        case FoodType::SODA:
            *this = createStoreFood(FoodType::SODA);
            break;
        case FoodType::ENERGY_DRINK:
            *this = createStoreFood(FoodType::ENERGY_DRINK);
            break;
        case FoodType::FROZEN_PIZZA:
            *this = createStoreFood(FoodType::FROZEN_PIZZA);
            break;
        case FoodType::CHOCOLATE_BAR:
            *this = createGymFood(FoodType::CHOCOLATE_BAR);
            break;
        case FoodType::PROTEIN_BAR:
            *this = createGymFood(FoodType::PROTEIN_BAR);
            break;
        default:
            throw std::invalid_argument("未知食物类型");
    }
}

// 从基类继承并实现getName()，直接使用基类的name_成员
const std::string& Food::getName() const {
    return AbstractItem::getName();
}

// 实现价格接口
int Food::getPrice() const {
    return static_cast<int>(price_);
}

// 使用物品：整合两种效果计算方式，采用绝对值修改而非百分比
void Food::use(Player& user) {
    if (healthEffect_ != 0) {
        user.addHealth(healthEffect_);  // 恢复绝对值血量
    }

    if (hungerEffect_ != 0) {
        user.addHunger(hungerEffect_);  // 恢复绝对值饱食度
    }
    
    if (energyEffect_ != 0) {
        user.addFatigue(energyEffect_);  // 恢复绝对值体力
    }

    // 使用后减少物品数量
    if (getAmount() > 0) {
        addAmount(-1);
    }
}

Food::FoodType Food::getFoodType() const {
    return type_;
}

// 创建商店食物：使用详细构造函数，补充名称和描述
Food Food::createStoreFood(FoodType type) {
    switch (type) {
        case FoodType::MEAT:
            return {type, 20.0, "烤肉", "一块多汁的烤肉，", 10.0, 40.0, 0.0};
        case FoodType::SODA:
            return {type, 6.0, "苏打水", "一罐甜得发腻的苏打水，", 0.0, 9.0, 0.0};
        case FoodType::ENERGY_DRINK:
            return {type, 14.0, "能量饮料", "功能性饮料，", 0.0, 5.0, 10.0};
        case FoodType::FROZEN_PIZZA:
            return {type, 9.0, "冷冻披萨", "方便快捷的冷冻披萨，", 10.0, 17.0, 0.0};
        default:
            throw std::invalid_argument("不是商店出售的食物类型");
    }
}

// 创建健身房食物：区分价格和描述
Food Food::createGymFood(FoodType type) {
    switch (type) {
        case FoodType::ENERGY_DRINK:
            return {type, 18.0, "能量饮料", "健身房特供能量饮料，", 0.0, 5.0, 10.0};
        case FoodType::CHOCOLATE_BAR:
            return {type, 12.0, "巧克力棒", "高热量巧克力棒，", 0.0, 5.0, 5.0};
        case FoodType::PROTEIN_BAR:
            return {type, 18.0, "蛋白质棒", "富含蛋白质的能量棒，", 0.0, 15.0, 0.0};
        default:
            throw std::invalid_argument("不是健身房出售的食物类型");
    }
}

// 判断是否为商店专属食物
bool Food::isStoreExclusive(FoodType type) {
    return type == FoodType::MEAT || type == FoodType::SODA || type == FoodType::FROZEN_PIZZA;
}

// 判断是否为健身房专属食物
bool Food::isGymExclusive(FoodType type) {
    return type == FoodType::CHOCOLATE_BAR || type == FoodType::PROTEIN_BAR;
}

// 获取效果值的getter方法，用于外部显示效果
double Food::getHealthEffect() const {
    return healthEffect_;
}

double Food::getHungerEffect() const {
    return hungerEffect_;
}

double Food::getEnergyEffect() const {
    return energyEffect_;
}
    