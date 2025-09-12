#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(int id_, const std::string& name_, 
             double strength_, double stamina_, double agility_)
    : id(id), name(name), 
      strength(strength_), stamina(stamina_), agility(agility_),
      fatigue(100 + 5 * (stamina_ - 1)), 
      health(100 + 15 * (stamina_ - 1)),
      maxHealth(100 + 15 * (stamina_ - 1)), maxFatigue(100 + 5 * (stamina_ - 1)) {
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
}