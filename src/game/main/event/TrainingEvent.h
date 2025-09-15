#ifndef TRAININGEVENT_H
#define TRAININGEVENT_H

#include <memory>
#include <string>

class Player;
class Game;
class Dialog;

enum class TrainingType
{
    STRENGTH, // 力量训练
    AGILITY,  // 敏捷训练
    STAMINA   // 耐力训练
};

class TrainingEvent
{
public:
    explicit TrainingEvent(std::shared_ptr<Player> player);

    // 执行训练
    bool train(TrainingType type, Game& game);

    // 检查是否可以训练
    bool canTrain(TrainingType type) const;

    // 获取训练状态信息
    std::string getTrainingStatus() const;
    
    // 获取训练帮助信息
    static std::string getTrainingHelp();

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

    // 获取经验值倍数
    double getStrengthExpRate() const;  // 力量训练经验获取系数
    double getAgilityExpRate() const;   // 敏捷训练经验获取系数
    double getStaminaExpRate() const;   // 耐力训练经验获取系数

    // 设置经验获取倍数
    void setStrengthExpRate(double rate);   // 设置力量训练经验获取系数
    void setAgilityExpRate(double rate);   // 设置敏捷训练经验获取系数
    void setStaminaExpRate(double rate);   // 设置耐力训练经验获取系数

    // 获取等级
    int getStrengthLevel() const; // 力量训练等级
    int getAgilityLevel() const;  // 敏捷训练等级
    int getStaminaLevel() const;  // 耐力训练等级

    // 经验值操作
    bool addStrengthExp(double exp);
    bool addAgilityExp(double exp);
    bool addStaminaExp(double exp);

    bool getHasSideEffect() const;
    void setHasSideEffect(bool flag);

    // 训练费用倍数( 击败敌人4的特殊效果 )
    double getMoneyCostRate() const;
    void setMoneyCostRate(double rate);

private:
    std::shared_ptr<Player> player;

    // 训练经验值
    double strengthExp;
    double agilityExp;
    double staminaExp;

    // 训练经验值倍数
    double strengthExpRate;
    double agilityExpRate;
    double staminaExpRate;

    // 训练消耗的金币倍数
    double moneyCostRate;

    // 训练等级
    int strengthLevel;
    int agilityLevel;
    int staminaLevel;

    // 嗑药副作用
    bool hasSideEffect;

    // 计算升级所需经验
    double calculateRequiredExp(int level) const;

    // 检查并处理升级
    bool checkAndLevelUp(TrainingType type);

    // 获取经验衰减比例
    double getExperienceDecayRate() const;

    // 训练成功消息
    void sendTrainingSuccessMessage(Game& game, TrainingType type) const;
    
    // 训练失败消息
    void sendTrainingFailureMessage(Game& game, TrainingType type) const;

    // 升级消息
    void sendLevelUpMessage(Game& game, TrainingType type, int newLevel) const;
    
    // 获取下一级所需经验信息
    std::string getNextLevelExpInfo(TrainingType type) const;
};

#endif // TRAININGEVENT_H