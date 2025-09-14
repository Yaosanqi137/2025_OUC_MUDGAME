#include "Food.h"
#include "../entity/Player.h"
#include <stdexcept>

// 私有构造函数
Food::Food(FoodType type, double price, const std::string& name, const std::string& intro, double healthEffect, double hungerEffect, double energyEffect)
    : AbstractItem(name, intro), type_(type), price_(price), healthEffect_(healthEffect),
      hungerEffect_(hungerEffect), energyEffect_(energyEffect) {
    intro_ += "使用后恢复饱食度 " + std::to_string(hungerEffect_) + " 点" +
            "恢复体力 " + std::to_string(energyEffect_) + " 点" +
            "恢复生命值 " + std::to_string(healthEffect_) + " 点";
}

// 公共构造函数
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
            throw std::invalid_argument("Unknown food type");
    }
}

int Food::getPrice() const {
    return price_;
}

void Food::use(Player& user) {

    if (healthEffect_ != 0) {
        user.addHealth(healthEffect_);  // 恢复血量
    }

    if (hungerEffect_ != 0) {
        user.addHunger(hungerEffect_);       // 恢复饱食度
    }
    
    if (energyEffect_ != 0) {
        user.addFatigue(energyEffect_);     // 恢复的体力
    }

    if (amount_ > 0) {
        amount_--;
    }
}

Food::FoodType Food::getFoodType() const {
    return type_;
}

Food Food::createStoreFood(FoodType type) {
    switch (type) {
        case FoodType::MEAT:
            return {type, 20.0, "烤肉", "一块多汁的烤肉，能有效恢复饱食度。", 10.0, 40.0, 0.0};
        case FoodType::SODA:
            return {type, 6.0, "苏打水", "一罐甜得发腻的苏打水。", 0.0, 9.0, 0.0};
        case FoodType::ENERGY_DRINK:
            return {type, 14.0, "能量饮料", "功能性饮料，能快速补充体力。", 0.0, 5.0, 10.0};
        case FoodType::FROZEN_PIZZA:
            return {type, 9.0, "冷冻披萨", "方便快捷的冷冻披萨，稍微恢复健康和饱食度。", 10.0, 17.0, 0.0};
        default:
            throw std::invalid_argument("Not a store food type");
    }
}

Food Food::createGymFood(FoodType type) {
    switch (type) {
        case FoodType::ENERGY_DRINK:
            // 注意：健身房的能量饮料可以有不同的价格或描述
            return {type, 18.0, "能量饮料", "健身房特供能量饮料，效果更佳。", 0.0, 5.0, 10.0};
        case FoodType::CHOCOLATE_BAR:
            return {type, 12.0, "巧克力棒", "高热量巧克力棒，迅速补充能量。", 0.0, 5.0, 5.0};
        case FoodType::PROTEIN_BAR:
            return {type, 18.0, "蛋白质棒", "富含蛋白质的能量棒，健身人士首选。", 0.0, 15.0, 0.0};
        default:
            throw std::invalid_argument("Not a gym food type");
    }
}

bool Food::isStoreExclusive(FoodType type) {
    return type == FoodType::MEAT || type == FoodType::SODA || type == FoodType::FROZEN_PIZZA;
}

bool Food::isGymExclusive(FoodType type) {
    return type == FoodType::CHOCOLATE_BAR || type == FoodType::PROTEIN_BAR;
}


// get方法实现, 主要用于EatEvent的效果显示
double Food::getHealthEffect() const {
    return healthEffect_;
}

double Food::getHungerEffect() const {
    return hungerEffect_;
}

double Food::getEnergyEffect() const {
    return energyEffect_;
}