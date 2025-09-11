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

    // 战斗属性计算
    double getHealth() const;        // 血量: 100 + 15 * (stamina - 1)
    double getEnergy() const;        // 体力槽: 100 + 5 * (stamina - 1)
    double getSpeed() const;         // 速度: agility

    // 当前状态（可修改）
    double getCurrentHealth() const;
    double getCurrentEnergy() const;
    double getCurrentFatigue() const;
    bool isDefeated() const;

    // 修改当前状态
    void addHealth(double value);
    void addEnergy(double value);
    void addFatigue(double value);

    // 技能相关
    std::vector<std::shared_ptr<Skill>>& getSkills();
    void addSkill(std::shared_ptr<Skill> skill);

private:
    int id_;
    std::string name_;

    // 基础属性
    double strength_;
    double stamina_;
    double agility_;

    // 战斗状态
    double current_health_;
    double current_energy_;
    double current_fatigue_;

    std::vector<std::shared_ptr<Skill>> skills_;
};

#endif // ENEMY_H