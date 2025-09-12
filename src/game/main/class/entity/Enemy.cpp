#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(int id, const std::string& name, 
             double strength, double stamina, double agility)
    : id_(id), name_(name), strength_(strength), 
      stamina_(stamina), agility_(agility),
      current_health_(100 + 15 * (stamina - 1)),
      current_energy_(100 + 5 * (stamina - 1)),
      current_fatigue_(80.0) {
}

Enemy::~Enemy() = default;

int Enemy::getId() const { return id_; }
std::string Enemy::getName() const { return name_; }
double Enemy::getStrength() const { return strength_; }
double Enemy::getStamina() const { return stamina_; }
double Enemy::getAgility() const { return agility_; }
double Enemy::getHealth() const { return 100 + 15 * (stamina_ - 1); }
double Enemy::getEnergy() const { return 100 + 5 * (stamina_ - 1); }
double Enemy::getSpeed() const { return agility_; }
double Enemy::getCurrentHealth() const { return current_health_; }
double Enemy::getCurrentEnergy() const { return current_energy_; }
double Enemy::getCurrentFatigue() const { return current_fatigue_; }
bool Enemy::isDefeated() const { return current_health_ <= 0; }

void Enemy::addHealth(double value) {
    current_health_ += value;
    current_health_ = std::max(0.0, current_health_);
    current_health_ = std::min(getHealth(), current_health_);
}

void Enemy::addEnergy(double value) {
    current_energy_ += value;
    current_energy_ = std::max(0.0, current_energy_);
    current_energy_ = std::min(getEnergy(), current_energy_);
}

void Enemy::addFatigue(double value) {
    current_fatigue_ += value;
    current_fatigue_ = std::max(0.0, current_fatigue_);
    current_fatigue_ = std::min(100.0, current_fatigue_);
}

std::vector<std::shared_ptr<Skill>>& Enemy::getSkills() {
    return skills_;
}

void Enemy::addSkill(std::shared_ptr<Skill> skill) {
    skills_.push_back(skill);
}