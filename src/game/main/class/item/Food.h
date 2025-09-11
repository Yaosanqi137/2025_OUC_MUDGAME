#ifndef FOOD_H
#define FOOD_H

#include "AbstractItem.h"
#include <string>

class Player;

class Food : public AbstractItem {
public:
    // 枚举食物类型
    enum class FoodType {
        MEAT,           // 肉
        SODA,           // 苏打水
        ENERGY_DRINK,   // 能量饮料
        FROZEN_PIZZA,   // 冷冻披萨
        CHOCOLATE_BAR,  // 巧克力棒
        PROTEIN_BAR     // 蛋白质棒
    };

    explicit Food(FoodType type);
    ~Food() override = default;

    // 实现抽象接口
    [[nodiscard]] int getPrice() const override;
    void use(Player& user) override;

    // 获取食物类型
    [[nodiscard]] FoodType getFoodType() const;

    // 静态方法：创建商店食物
    static Food createStoreFood(FoodType type);
    
    // 静态方法：创建健身房食物
    static Food createGymFood(FoodType type);

    // 检查是否是商店专属食物
    static bool isStoreExclusive(FoodType type);
    
    // 检查是否是健身房专属食物
    static bool isGymExclusive(FoodType type);

    // get方法实现, 主要用于EatEvent的效果显示
    [[nodiscard]] double getHealthEffect() const;   // 健康值
    [[nodiscard]] double getHungerEffect() const;   // 饱食度
    [[nodiscard]] double getEnergyEffect() const;   // 体力值

private:
    FoodType type_;
    double price_;

    // 这三个属性均不是百分比形式，use时注意除以对应的属性上限，如健康值恢复是 healthEffect_ / user.getHealth()
    double healthEffect_;    // 生命值影响
    double hungerEffect_;    // 饱食度影响
    double energyEffect_;    // 体力影响

    // 私有构造函数，用于静态工厂方法
    Food(FoodType type, double price, const std::string& name, const std::string& intro, double healthEffect, double hungerEffect, double energyEffect);
};

#endif // FOOD_H