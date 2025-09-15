#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <memory>
#include <vector>
#include "../skill/Skill.h"

class Enemy {
public:
    Enemy(int id, const std::string& name, double strength, double stamina, double agility);
    ~Enemy();

    // 基本信息
    int getId() const;
    std::string getName() const;

    // 核心属性获取
    double getStrength() const;
    double getStamina() const;
    double getAgility() const;
    double getFatigue() const;            // 体力(数值)
    double getHealth() const;             // 生命值(数值)

    // 战斗属性计算
    double getMaxHealth() const;          
    double getMaxFatigue() const;         
    double getSpeed() const;              

    // 当前状态（可修改）
    bool isDefeated() const;

    // 修改当前状态
    void addStrength(double value);
    void addStamina(double value);
    void addAgility(double value);
    void addFatigue(double value);
    void addHealth(double value);

    // 技能相关
    std::vector<std::shared_ptr<Skill>>& getSkills();
    void addSkill(std::shared_ptr<Skill> skill);

    // 下面是特殊技能效果函数
    double getSustainDamageRate() const;
    double getUpperBodySustainDamageRate() const;
    double getLowerBodySustainDamageRate() const;
    double getFatigueConsumeRate() const;
    double getExHitRate() const;

    void setSustainDamageRate(double rate);
    void setUpperBodySustainDamageRate(double rate);
    void setLowerBodySustainDamageRate(double rate);
    void setFatigueConsumeRate(double rate);
    void setExHitRate(double rate);
    // 上面是特殊技能效果函数
private:
    int id;
    std::string name;

    // 基础属性
    double strength;        // 力量
    double stamina;         // 耐力
    double agility;         // 敏捷
    double fatigue;         // 体力值(数值)
    double health;          // 生命值(数值)

    //上限值
    double maxHealth;      // 生命值上限
    double maxFatigue;     // 体力值上限

    // 技能列表
    std::vector<std::shared_ptr<Skill>> skills_;

    // 下面这块是特殊技能专用属性
    double sustainDamageRate;           // 普遍受到伤害倍数
    double upperBodySustainDamageRate;  // 上肢受到伤害倍数
    double lowerBodySustainDamageRate;     // 下肢施加伤害倍数
    double fatigueConsumeRate;          // 体力消耗倍数
    double exHitRate;                   // 额外命中率
    // 上面这块是特殊技能专用属性

};

#endif // ENEMY_H