#ifndef SKILL_H
#define SKILL_H

#include <string>
#include <functional>
#include <memory>

// 前向声明
class Player;
class Enemy;

// 技能效果类型枚举
enum class SkillEffectType {
    NONE,                   // 无效果
    BASIC_ATTACK,           // 基础攻击
    ATTRIBUTE_BOOST,        // 属性增益
    TRAINING_EFFICIENCY,    // 训练效率
    DAMAGE_REFLECT,         // 伤害反射
    DAMAGE_REDUCTION,       // 伤害减免
    HIT_RATE_MODIFIER,      // 命中率修改
    STAMINA_MODIFIER,       // 体力消耗修改
    ATTACK_TYPE_DEFENSE,    // 攻击类型防御
    ENEMY_ATTRIBUTE_MOD,    // 敌人属性修改
    GLOBAL_HIT_RATE_BOOST   // 全局命中率提升
};

// 目标类型枚举
enum class TargetType {
    SELF,           // 对自己使用
    ENEMY,          // 对敌人使用
    PLAYER,         // 对玩家使用（敌人对玩家）
    ANY             // 任意目标
};

class Skill {
private:
    int id;                    // 技能ID
    std::string skillName;     // 技能名称
    std::string description;   // 技能描述
    SkillEffectType effectType;// 技能效果类型
    TargetType targetType;     // 目标类型
    bool isLocked;             // 是否锁定
    int unlockCost;            // 解锁所需技能点数
    
    // 攻击相关属性
    double baseDamage;              // 基础伤害值
    double damageCoefficient;       // 伤害系数
    double baseHitRate;             // 基础命中率
    double hitRateCoefficient;      // 命中率系数
    double baseStaminaCost;         // 基础体力消耗
    double staminaCostCoefficient;  // 体力消耗系数
    
    // 属性增益相关
    double minAttributeValue;       // 最低属性值
    std::string attributeType;      // 属性类型
    
    // 训练效率相关
    double trainingMultiplier;      // 训练效率倍率
    
    // 特殊效果相关
    double effectValue;             // 效果数值

public:
    // 构造函数
    Skill(int id, const std::string& name, const std::string& desc, 
          SkillEffectType type, TargetType targetType, int cost,
          double baseDmg = 0, double dmgCoeff = 0, double baseHit = 0, 
          double hitCoeff = 0, double baseStamina = 0, double staminaCoeff = 0,
          double minValue = 0, const std::string& attrType = "", 
          double multiplier = 0, double value = 0);

    // 获取基本信息
    int getId() const;
    std::string getSkillName() const;
    std::string getDescription() const;
    SkillEffectType getEffectType() const;
    TargetType getTargetType() const;
    int getUnlockCost() const;
    bool getIsLocked() const;
    
    // 攻击相关属性获取
    double getBaseDamage() const;
    double getDamageCoefficient() const;
    double getBaseHitRate() const;
    double getHitRateCoefficient() const;
    double getBaseStaminaCost() const;
    double getStaminaCostCoefficient() const;
    
    // 属性增益相关获取
    double getMinAttributeValue() const;
    std::string getAttributeType() const;
    
    // 训练效率相关获取
    double getTrainingMultiplier() const;
    
    // 特殊效果相关获取
    double getEffectValue() const;

    // 技能效果计算方法
    double calculateDamage(double strength) const;
    double calculateHitRate(double agility, double strength, double stamina) const;
    double calculateStaminaCost(double strength) const;
    
    // 解锁相关方法
    void unlock();
    void lock();
    bool canUnlock(int playerSkillPoints) const;
    
    // 使用条件检查（重载版本）
    bool canUse(const Player& user) const;
    bool canUse(const Enemy& user) const;
    
    // 执行技能（重载版本）
    void execute(Player& user, Player& target);
    void execute(Player& user, Enemy& target);
    void execute(Enemy& user, Player& target);
    void execute(Enemy& user, Enemy& target);
    
    // 技能类型判断
    bool isAttackSkill() const;
    bool isAttributeBoostSkill() const;
    bool isTrainingSkill() const;
    bool isSpecialEffectSkill() const;

    // 工具方法
    static int skillNameToID(const std::string& skillName);
    static std::string idToSkillName(int id);
};

#endif // SKILL_H