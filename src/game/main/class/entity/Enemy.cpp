// Enemy.cpp
#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(int id_, const std::string& name_, 
             double strength_, double stamina_, double agility_)
    : id(id_), name(name_), 
      strength(strength_), stamina(stamina_), agility(agility_),
      fatigue(100 + 5 * (stamina_ - 1)), 
      health(100 + 15 * (stamina_ - 1)),
      maxHealth(100 + 15 * (stamina_ - 1)), maxFatigue(100 + 5 * (stamina_ - 1)),
      sustainDamageRate(1.0), upperBodySustainDamageRate(1.0), lowerBodySustainDamageRate(1.0),
      fatigueConsumeRate(1.0), exHitRate(0.0) {
    // 确保 name 不为空
    if (name.empty()) {
        name = "未知敌人";
    }
}

Enemy::~Enemy() = default;

int Enemy::getId() const { return id; }
std::string Enemy::getName() const { return name; }
double Enemy::getStrength() const { return strength; }
double Enemy::getStamina() const { return stamina; }
double Enemy::getAgility() const { return agility; }
double Enemy::getFatigue() const { return fatigue; }
double Enemy::getHealth() const { return health; }
double Enemy::getMaxHealth() const { return maxHealth; }
double Enemy::getMaxFatigue() const { return maxFatigue; }
double Enemy::getSpeed() const { return agility; }
bool Enemy::isDefeated() const { return health <= 0; }

void Enemy::addStrength(double value) {
    strength += value;
}

void Enemy::addStamina(double value) {
    stamina += value;
}

void Enemy::addAgility(double value) {
    agility += value;
}

void Enemy::addFatigue(double value) {
    fatigue += value;
    fatigue = std::max(0.0, fatigue);
    fatigue = std::min(getMaxFatigue(), fatigue);
}

void Enemy::addHealth(double value) {
    health += value;
    health = std::max(0.0, health);
    health = std::min(getMaxHealth(), health);
}

std::vector<std::shared_ptr<Skill>>& Enemy::getSkills() {
    return skills_;
}

void Enemy::addSkill(std::shared_ptr<Skill> skill) {
    skills_.push_back(skill);
    std::string skillName_ = skill -> getSkillName();
    if(skillName_ == "自杀式袭击"){
        exHitRate -= 0.1;
        fatigueConsumeRate - 0.2;
    }
    else if(skillName_ == "钝兵挫锐"){
        sustainDamageRate -= 0.2;
    }
    else if(skillName_ == "拳击手"){
        exHitRate += 0.1;
    }
    else if(skillName_ == "千手不破"){
        upperBodySustainDamageRate -= 0.3;
        lowerBodySustainDamageRate += 0.2;
    }
}

// 下面是特殊技能效果函数
double Enemy::getSustainDamageRate() const{return sustainDamageRate;}
double Enemy::getUpperBodySustainDamageRate() const{return upperBodySustainDamageRate;}
double Enemy::getLowerBodySustainDamageRate() const{return lowerBodySustainDamageRate;}
double Enemy::getFatigueConsumeRate() const{return fatigueConsumeRate;}
double Enemy::getExHitRate() const{return exHitRate;}

void Enemy::setSustainDamageRate(double rate){sustainDamageRate = rate;}
void Enemy::setUpperBodySustainDamageRate(double rate){upperBodySustainDamageRate = rate;}
void Enemy::setLowerBodySustainDamageRate(double rate){lowerBodySustainDamageRate = rate;}
void Enemy::setFatigueConsumeRate(double rate){fatigueConsumeRate = rate;}
void Enemy::setExHitRate(double rate){exHitRate = rate;}
// 上面是特殊技能效果函数