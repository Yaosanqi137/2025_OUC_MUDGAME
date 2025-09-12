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
};

#endif // ENEMY_H