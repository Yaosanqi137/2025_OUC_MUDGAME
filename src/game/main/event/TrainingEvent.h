#ifndef TRAININGEVENT_H
#define TRAININGEVENT_H

#include <memory>

class Player;

class TrainingEvent
{
public:
    explicit TrainingEvent(std::shared_ptr<Player> player);

    // 训练类型枚举
    enum class TrainingType
    {
        STRENGTH, // 力量训练
        AGILITY,  // 敏捷训练
        STAMINA   // 耐力训练
    };

    // 执行训练
    bool train(TrainingType type);

    // 检查是否可以训练
    bool canTrain(TrainingType type) const;

    // 获取训练消耗
    int getTrainingCost() const;   // 金币消耗
    double getFatigueCost() const; // 体力值消耗
    double getHungerCost() const;  // 饱食度消耗

    // 每日经验衰减
    void applyDailyExperienceDecay();

    // 获取经验值
    double getStrengthExp() const; // 力量训练经验
    double getAgilityExp() const;  // 敏捷训练经验
    double getStaminaExp() const;  // 耐力训练经验

    // 获取等级
    int getStrengthLevel() const; // 力量训练等级
    int getAgilityLevel() const;  // 敏捷训练等级
    int getStaminaLevel() const;  // 耐力训练等级

    // 经验值操作
    void addStrengthExp(double exp);
    void addAgilityExp(double exp);
    void addStaminaExp(double exp);

private:
    std::shared_ptr<Player> player;

    // 训练经验值
    double strengthExp;
    double agilityExp;
    double staminaExp;

    // 训练等级
    int strengthLevel;
    int agilityLevel;
    int staminaLevel;

    // 计算升级所需经验
    double calculateRequiredExp(int level) const;

    // 检查并处理升级
    void checkAndLevelUp(TrainingType type);

    // 获取经验衰减比例
    double getExperienceDecayRate() const;
};

#endif // TRAININGEVENT_H