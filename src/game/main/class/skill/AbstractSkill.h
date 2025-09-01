// Changed on 2025-09-01
#ifndef ABSTRACTSKILL_H
#define ABSTRACTSKILL_H

#include <string>

// AbstractSkill类中使用Player类作为参数类型
class Player;

class AbstractSkill {
protected:
    // 技能名称
    std::string skillName;
    
    // 技能描述
    std::string description;
    
    // 基础伤害值（固定值部分）
    int baseDamage;
    
    // 伤害系数（力量乘以的系数）
    float damageCoefficient;
    
    // 基础命中率（固定值部分）
    int baseHitRate;
    
    // 命中率系数
    float hitRateCoefficient;
    
    // 基础体力消耗（固定值部分）
    int baseStaminaCost;
    
    // 体力消耗系数
    float staminaCostCoefficient;
    
    // 技能所需的最低玩家等级
    int requiredLevel;
    
    // 技能类型：0-攻击技能, 1-防御技能, 2-辅助技能等
    int skillType;

public:
    // 构造函数：初始化技能的所有基本属性
    AbstractSkill(const std::string& name, const std::string& desc, 
                  int baseDmg, float dmgCoeff, 
                  int baseHit, float hitCoeff, 
                  int baseStamina, float staminaCoeff,
                  int reqLevel = 1, int type = 0);
    
    // 虚析构函数：确保派生类对象能被正确销毁
    virtual ~AbstractSkill() = default;

    // 获取技能名称
    std::string getSkillName() const;
    
    // 获取技能描述
    std::string getDescription() const;
    
    // 获取基础伤害值
    int getBaseDamage() const;
    
    // 获取伤害系数
    float getDamageCoefficient() const;
    
    // 获取基础命中率
    int getBaseHitRate() const;
    
    // 获取命中率系数
    float getHitRateCoefficient() const;
    
    // 获取基础体力消耗
    int getBaseStaminaCost() const;
    
    // 获取体力消耗系数
    float getStaminaCostCoefficient() const;
    
    // 获取技能所需等级
    int getRequiredLevel() const;
    
    // 获取技能类型
    int getSkillType() const;

    // 设置技能描述
    void setDescription(const std::string& newDescription);
    
    // 设置基础伤害值
    void setBaseDamage(int newBaseDamage);
    
    // 设置伤害系数
    void setDamageCoefficient(float newCoefficient);
    
    // 设置基础命中率
    void setBaseHitRate(int newBaseHitRate);
    
    // 设置命中率系数
    void setHitRateCoefficient(float newCoefficient);
    
    // 设置基础体力消耗
    void setBaseStaminaCost(int newBaseStaminaCost);
    
    // 设置体力消耗系数
    void setStaminaCostCoefficient(float newCoefficient);
    
    // 设置技能所需等级
    void setRequiredLevel(int level);
    
    // 设置技能类型
    void setSkillType(int type);

    // 计算技能总伤害：基础伤害 + 玩家力量 × 伤害系数
    virtual int calculateDamage(int playerStrength) const;
    
    // 计算技能命中率：基础命中率 + (敏捷/(力量+敏捷+耐力)×3) × 命中系数
    virtual float calculateHitRate(int playerAgility, int playerStrength, int playerStamina) const;
    
    // 计算技能体力消耗：基础体力消耗 + 玩家力量 × 体力消耗系数
    virtual int calculateStaminaCost(int playerStrength) const;

    // 检查玩家是否可以使用该技能（检查等级、体力等条件）
    virtual bool canUse(const Player& user) const;
    
    // 技能使用时的回调函数（在使用前调用）
    virtual void onUse(Player& user);
    
    // 技能命中目标时的回调函数
    virtual void onHit(Player& user, Player& target);
    
    // 技能未命中目标时的回调函数
    virtual void onMiss(Player& user, Player& target);

    // 执行技能的主要逻辑（纯虚函数，必须由子类实现）
    virtual void execute(Player& user, Player& target) = 0;

    // 升级技能（提高技能效果）
    virtual void upgrade();
    
    // 获取升级所需的代价（金币、技能点等）
    virtual int getUpgradeCost() const;

    // 将技能数据序列化为字符串（用于存档）
    virtual std::string serialize() const;
    
    // 从字符串反序列化技能数据（用于读档）
    virtual void deserialize(const std::string& data);
};

#endif
