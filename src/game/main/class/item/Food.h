#ifndef FOOD_H
#define FOOD_H

#include "AbstractItem.h"
#include <string>

// 前向声明 Player 类，避免循环引用（use 方法仅需类声明即可）
class Player;

// Food 类继承自 AbstractItem，实现食物专属功能
class Food : public AbstractItem {
public:
    // 枚举食物类型：覆盖所有场景的食物种类
    enum class FoodType {
        MEAT,           // 肉（商店专属）
        SODA,           // 苏打水（商店专属）
        ENERGY_DRINK,   // 能量饮料（商店/健身房通用）
        FROZEN_PIZZA,   // 冷冻披萨（商店专属）
        CHOCOLATE_BAR,  // 巧克力棒（健身房专属）
        PROTEIN_BAR     // 蛋白质棒（健身房专属）
    };

    // 公共构造函数：通过食物类型创建实例（explicit 防止隐式类型转换）
    explicit Food(FoodType type);
    // 虚析构函数：确保派生类析构时能正确调用基类析构
    ~Food() override = default;

    // 实现 AbstractItem 抽象接口
    [[nodiscard]] const std::string& getName() const override;  // 获取食物名称（复用基类 name_ 成员）
    [[nodiscard]] int getPrice() const override;                // 获取食物价格（返回整数，符合货币单位）
    void use(Player& user) override;                            // 使用食物，作用于玩家

    // 食物类型相关接口
    [[nodiscard]] FoodType getFoodType() const;  // 获取当前食物类型

    // 静态工厂方法：创建对应场景的食物（隐藏创建细节）
    static Food createStoreFood(FoodType type);   // 创建商店出售的食物
    static Food createGymFood(FoodType type);     // 创建健身房出售的食物

    // 静态判断方法：区分食物的专属场景
    static bool isStoreExclusive(FoodType type);  // 判断是否为商店专属食物
    static bool isGymExclusive(FoodType type);    // 判断是否为健身房专属食物

    // 效果获取接口：用于外部显示食物效果（如 UI 展示、事件日志）
    [[nodiscard]] double getHealthEffect() const;  // 获取生命值影响值
    [[nodiscard]] double getHungerEffect() const;  // 获取饱食度影响值
    [[nodiscard]] double getEnergyEffect() const;  // 获取体力影响值

private:
    // 食物核心属性
    FoodType type_;          // 食物类型（枚举值）
    double price_;           // 食物价格（double 支持未来小数价格需求，返回时转整数）
    double healthEffect_;    // 生命值影响（绝对值，非百分比）
    double hungerEffect_;    // 饱食度影响（绝对值，非百分比）
    double energyEffect_;    // 体力影响（绝对值，非百分比）

    // 私有构造函数：仅用于静态工厂方法（封装复杂初始化逻辑）
    Food(FoodType type, double price, const std::string& name, const std::string& intro,
         double healthEffect, double hungerEffect, double energyEffect);
};

#endif // FOOD_H