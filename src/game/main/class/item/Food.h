#ifndef FOOD_H
#define FOOD_H

#include "AbstractItem.h"
#include <string>

// 前向声明 Player 类，避免循环引用
class Player;

// Food 类继承自 AbstractItem，实现食物专属功能
class Food : public AbstractItem {
public:
    // 枚举食物类型：覆盖所有场景的食物种类
    enum class FoodType {
        MEAT,           // 肉
        SODA,           // 苏打水
        ENERGY_DRINK,   // 能量饮料
        FROZEN_PIZZA,   // 冷冻披萨
        CHOCOLATE_BAR,  // 巧克力棒
        PROTEIN_BAR,    // 蛋白质棒
        COFFEE,         // 女仆咖啡
        BREAD,          // 爱心面包
        WAXUE_DI_BURGER // 瓦学弟蛋包饭（修正命名）
    };

    // 公共构造函数：通过食物类型创建实例
    explicit Food(FoodType type);
    // 虚析构函数
    ~Food() override = default;

    // 实现 AbstractItem 抽象接口
    [[nodiscard]] const std::string& getName() const override;  // 恢复getName()声明
    [[nodiscard]] int getPrice() const override;
    bool use(Player& user) override;  // 保持与基类一致的返回类型

    // 食物类型相关接口
    [[nodiscard]] FoodType getFoodType() const;

    // 静态工厂方法
    static Food createStoreFood(FoodType type);
    static Food createGymFood(FoodType type);
    static Food createCafeFood(FoodType type);  // 修正拼写错误 Cofe->Cafe

    // 检查专属场景
    static bool isStoreExclusive(FoodType type);
    static bool isGymExclusive(FoodType type);
    static bool isCafeExclusive(FoodType type);  // 修正拼写错误

    // get方法实现, 主要用于EatEvent的效果显示
    [[nodiscard]] double getHealthEffect() const;   // 健康值
    [[nodiscard]] double getHungerEffect() const;   // 饱食度
    [[nodiscard]] double getEnergyEffect() const;   // 体力值

private:
    // 食物核心属性
    FoodType type_;          
    double price_;           
    double healthEffect_;    
    double hungerEffect_;    
    double energyEffect_;    

    // 私有构造函数：仅用于静态工厂方法
    Food(FoodType type, double price, const std::string& name, const std::string& intro,
         double healthEffect, double hungerEffect, double energyEffect);
};

#endif // FOOD_H
