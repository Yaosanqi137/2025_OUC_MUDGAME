#ifndef MEDICINE_H
#define MEDICINE_H

#include "AbstractItem.h"
#include <string>

class Player;

class Medicine : public AbstractItem {
public:
    // 枚举药物类型
    enum class MedicineType {
        REVIVAL_PILL,       // 回生丹
        STRENGTH_POTION,    // 力量药水
        AGILITY_POTION,     // 敏捷药水  
        STAMINA_POTION,     // 耐力药水
        SKILL_POINT_POTION  // 技能点药水
    };

    explicit Medicine(MedicineType type);
    ~Medicine() override = default;

    // 实现抽象接口
    [[nodiscard]] int getPrice() const override;
    void use(Player& user) override;

    // 获取药物类型
    [[nodiscard]] MedicineType getMedicineType() const;

    // 静态方法：创建药物
    static Medicine createMedicine(MedicineType type);

    // get方法实现，用于UI显示
    [[nodiscard]] double getHealthEffect() const;   // 生命值效果
    [[nodiscard]] double getStrengthEffect() const; // 力量效果
    [[nodiscard]] double getAgilityEffect() const;  // 敏捷效果
    [[nodiscard]] double getStaminaEffect() const;  // 耐力效果
    [[nodiscard]] int getSkillPointEffect() const;  // 技能点效果

private:
    MedicineType type;
    double price;

    // 药物效果
    double healthEffect;     // 生命值影响
    double strengthEffect;   // 力量影响
    double agilityEffect;    // 敏捷影响
    double staminaEffect;    // 耐力影响
    int skillPointEffect;    // 技能点影响

    // 副作用标志
    bool hasSideEffect;      // 是否有副作用

    // 私有构造函数，用于静态工厂方法
    Medicine(MedicineType type, double price, const std::string& name, 
             const std::string& intro, double healthEffect, double strengthEffect,
             double agilityEffect, double staminaEffect, int skillPointEffect,
             bool hasSideEffect);
};

#endif // MEDICINE_H