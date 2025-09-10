/*

Last changed on 9-11 1:00 by Anyeling
Bug to fix: removeItemFromInventory模板函数无法链接 不能显式声明
Bug to fix: not link to BagLayout(ui) yet

*/


#include "Player.h"

Player::Player(Game& game_logic) : game_logic_(game_logic), name("NOT_SET"), healthiness(80), strength(1),
                   stamina(1), agility(1), hunger(80), fatigue(80), money(1000), location("???"),
                   minStrength(0), minStamina(0), minAgility(0), skillPoints(0) {
}

// TODO: 保存数据
Player::~Player() = default;

const std::string &Player::getName() {
    return name;
}

void Player::setName(const std::string& name) {
    this->name = name;
}

const std::string& Player::getLocation() const {
    return location;
}

void Player::setLocation(const std::string &location) {
    this->location = location;
}

double Player::getHealthiness() const {
    return healthiness;
}

double Player::getStrength() const {
    return strength;
}

double Player::getStamina() const {
    return stamina;
}

double Player::getAgility() const {
    return agility;
}

double Player::getHunger() const {
    return hunger;
}

double Player::getFatigue() const {
    return fatigue;
}

double Player::getSavings() const {
    return money;
}

double Player::getSpeed() const {
    return 1.1 * agility;
}

double Player::getHealth() const {
    return 100 + 15 * (stamina - 1);
}

double Player::getEnergy() const {
    return 100 + 5 * (stamina - 1);
}

void Player::addHealthiness(const double value) {
    healthiness += value;
    healthiness = std::max(0.0,healthiness);    // 确保不低于0
    healthiness = std::min(1.0,healthiness);    // 确保不高于1
}

void Player::addStrength(const double value) {
    strength += value;
    strength = std::max(strength, minStrength); // 确保不低于最低力量值
}

void Player::addStamina(const double value) {
    stamina += value;
    stamina = std::max(stamina, minStamina);    // 确保不低于最低耐力值
}

void Player::addAgility(const double value) {
    agility += value;
    agility = std::max(agility, minAgility);    // 确保不低于最低敏捷值
}

void Player::addHunger(const double value) {
    hunger += value;
    hunger = std::max(0.0, hunger);     // 确保不低于0
    hunger = std::min(1.0, hunger);     // 确保不高于1
}

void Player::addFatigue(const double value) {
    fatigue += value;
    fatigue = std::max(0.0, fatigue);   // 确保不低于0
    fatigue = std::min(1.0, fatigue);   // 确保不高于1
}

void Player::addSavings(const double value) {
    money += value;
}

// 设置最低属性值(力量，耐力，敏捷)
void Player::setMinStrength(double value) {
    minStrength = value;
}
void Player::setMinStamina(double value) {
    minStamina = value;
}
void Player::setMinAgility(double value) {
    minAgility = value;
}

// 获取最低属性值(力量，耐力，敏捷)
double Player::getMinStrength() const {
    return minStrength;
}

double Player::getMinStamina() const {
    return minStamina;
}

double Player::getMinAgility() const {
    return minAgility;
}


// 销毁Item(Food; Medicine)函数
/*
template<typename T>
bool Player::removeItemFromInventory(const std::shared_ptr<T>& item) {
    // 这里需要根据你的背包实现来编写具体的移除逻辑
    // 示例：遍历背包找到并移除对应物品
    auto& inventory = getInventory(); // 假设有 getInventory() 方法
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (*it == item) {
            inventory.erase(it);
            return true;
        }
    }
    return false;
}
    
*/

// 显式实例化模板 编译错误
/*
template bool Player::removeItemFromInventory<Food>(const std::shared_ptr<Food>& item);
template bool Player::removeItemFromInventory<Medicine>(const std::shared_ptr<Medicine>& item);
*/