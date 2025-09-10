#include "Food.h"
#include <stdexcept>

// 私有构造函数
Food::Food(FoodType type, double price, double healthEffect, double hungerEffect, double energyEffect)
    : type_(type), price_(price), healthEffect_(healthEffect), 
      hungerEffect_(hungerEffect), energyEffect_(energyEffect) {}

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

const std::string& Food::getName() const {
    static const std::string names[] = {
        "肉",
        "苏打水", 
        "能量饮料",
        "冷冻披萨",
        "巧克力棒",
        "蛋白质棒"
    };
    return names[static_cast<int>(type_)];
}

int Food::getPrice() const {
    return price_;
}

void Food::use(Player& user) {

    if (healthEffect_ != 0) {
        user.addHealthiness(healthEffect_ / user.getHealth());  // 恢复的是血量槽百分比
    }

    // 该函数目前有争议，不确定饱食度是不是百分比形式
    if (hungerEffect_ != 0) {
        user.addHunger(hungerEffect_ / user.getHunger());       // 恢复的是饱食度百分比
    }
    
    if (energyEffect_ != 0) {
        user.addFatigue(energyEffect_ / user.getEnergy());     // 恢复的是体力槽百分比
    }
}

Food::FoodType Food::getFoodType() const {
    return type_;
}

Food Food::createStoreFood(FoodType type) {
    switch (type) {
        case FoodType::MEAT:
            return Food(type, 20.0, 10.0, 40.0, 0.0);
        case FoodType::SODA:
            return Food(type, 6.0, 0.0, 9.0, 0.0);
        case FoodType::ENERGY_DRINK:
            return Food(type, 14.0, 0.0, 5.0, 10.0);
        case FoodType::FROZEN_PIZZA:
            return Food(type, 9.0, 10.0, 17.0, 0.0);
        default:
            throw std::invalid_argument("Not a store food type");
    }
}

Food Food::createGymFood(FoodType type) {
    switch (type) {
        case FoodType::ENERGY_DRINK:
            return Food(type, 18.0, 0.0, 5.0, 10.0);
        case FoodType::CHOCOLATE_BAR:
            return Food(type, 12.0, 0.0, 5.0, 5.0);
        case FoodType::PROTEIN_BAR:
            return Food(type, 18.0, 0.0, 15.0, 0.0);
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