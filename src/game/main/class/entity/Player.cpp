/*

Last changed on 9-11 1:00 by Anyeling
Bug to fix: removeItemFromInventory模板函数无法链接 不能显式声明
Bug to fix: not link to BagLayout(ui) yet

*/


#include "Player.h"
#include "../item/Food.h"
#include "../item/UsefulItem.h"

Player::Player(Game& game_logic) : game_logic_(game_logic), name("NOT_SET"), healthiness(80), strength(1),
                   stamina(1), agility(1), hunger(80), fatigue(80), money(1000), location("???"), health(100),
                   minStrength(0), minStamina(0), minAgility(0), skillPoints(0),
                   maxHunger(80), maxFatigue(80), maxHealth(100) {
    // 在构造函数中初始化背包
    initializeInventory();
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
    return health;
}

double Player::getEnergy() const {
    return 100 + 5 * (stamina - 1);
}

void Player::addHealthiness(const double value) {
    healthiness += value;
    healthiness = std::max(0.0,healthiness);    // 确保不低于0
    healthiness = std::min(100.0,healthiness);    // 确保不高于100
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
    hunger = std::max(0.0, hunger);
    hunger = std::min(maxHunger, hunger);
}

void Player::addFatigue(const double value) {
    fatigue += value;
    fatigue = std::max(0.0, fatigue);
    fatigue = std::min(maxFatigue, fatigue);
}

void Player::addHealth(double value) {
    health += value;
    health = std::max(0.0, health);
    health = std::max(health, maxHealth);
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

// 获取最高属性值(饱食度，体力，生命值)
double Player::getMaxHunger() const {
    return maxHunger;
}

double Player::getMaxFatigue() const {
    return maxFatigue;
}

double Player::getMaxHealth() const {
    return maxHealth;
}

// 设置最高属性值(饱食度，体力，生命值)
void Player::setMaxHunger(double value) {
    maxHunger = value;
}

void Player::setMaxFatigue(double value) {
    maxFatigue = value;
}

void Player::setMaxHealth(double value) {
    maxHealth = value;
}


std::vector<std::shared_ptr<AbstractItem>>& Player::getInventory() {
    return inventory_;
}

bool Player::removeItemFromInventory(const std::shared_ptr<AbstractItem>& item) {
    for (auto it = inventory_.begin(); it != inventory_.end(); ++it) {
        if (*it == item) {
            inventory_.erase(it);
            return true;
        }
    }
    return false;
}

std::shared_ptr<AbstractItem> Player::findItemByName(const std::string& name) {
    for (auto& item : inventory_) {
        if (item->getName() == name) {
            return item;
        }
    }
    return nullptr;
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

std::vector<std::shared_ptr<AbstractItem>> Player::getDisplayableItems() const {
    std::vector<std::shared_ptr<AbstractItem>> displayableItems;
    for (const auto& item : inventory_) {
        if (item->getAmount() > 0) {
            displayableItems.push_back(item);
        }
    }
    return displayableItems;
}

void Player::addItem(std::shared_ptr<AbstractItem> item) {
    // 检查背包中是否已经有同名物品
    auto existingItem = findItemByName(item->getName());
    if (existingItem) {
        // 如果已存在，增加数量
        existingItem->addAmount(item->getAmount());
    } else {
        // 如果不存在，添加新物品
        inventory_.push_back(item);
    }
}

void Player::addItemByType(const std::string& itemType, int amount) {
    // 根据物品类型创建物品实例并添加到背包
    if (itemType == "烤肉") {
        auto food = std::make_shared<Food>(Food::FoodType::MEAT);
        food->setAmount(amount);
        addItem(food);
    } else if (itemType == "苏打水") {
        auto food = std::make_shared<Food>(Food::FoodType::SODA);
        food->setAmount(amount);
        addItem(food);
    } else if (itemType == "能量饮料") {
        auto food = std::make_shared<Food>(Food::FoodType::ENERGY_DRINK);
        food->setAmount(amount);
        addItem(food);
    } else if (itemType == "冷冻披萨") {
        auto food = std::make_shared<Food>(Food::FoodType::FROZEN_PIZZA);
        food->setAmount(amount);
        addItem(food);
    } else if (itemType == "巧克力棒") {
        auto food = std::make_shared<Food>(Food::FoodType::CHOCOLATE_BAR);
        food->setAmount(amount);
        addItem(food);
    } else if (itemType == "蛋白质棒") {
        auto food = std::make_shared<Food>(Food::FoodType::PROTEIN_BAR);
        food->setAmount(amount);
        addItem(food);
    } else if (itemType == "拳套") {
        auto item = std::make_shared<UsefulItem>(UsefulItem::ItemType::BOXING_GLOVES);
        item->setAmount(amount);
        addItem(item);
    } else if (itemType == "拳击馆通行证") {
        auto item = std::make_shared<UsefulItem>(UsefulItem::ItemType::BOXING_GYM_PASS);
        item->setAmount(amount);
        addItem(item);
    }
}

bool Player::setItemAmount(const std::string& itemName, int amount) {
    auto item = findItemByName(itemName);
    if (item) {
        item->setAmount(amount);
        return true;
    }
    return false;
}

bool Player::addItemAmount(const std::string& itemName, int amount) {
    auto item = findItemByName(itemName);
    if (item) {
        item->addAmount(amount);
        return true;
    }
    return false;
}

bool Player::removeItemAmount(const std::string& itemName, int amount) {
    auto item = findItemByName(itemName);
    if (item && item->getAmount() >= amount) {
        item->addAmount(-amount); // 减少数量
        return true;
    }
    return false;
}

bool Player::useItem(const std::string& itemName) {
    auto item = findItemByName(itemName);
    if (item && item->getAmount() > 0) {
        item->use(*this); // 使用物品，传递玩家对象
        return true;
    }
    return false;
}

void Player::initializeInventory() {
    // 清空现有背包
    inventory_.clear();

    // 添加所有食物类型，数量为0
    auto meat = std::make_shared<Food>(Food::FoodType::MEAT);
    meat->setAmount(2); // 测试用：添加2个肉
    inventory_.push_back(meat);

    auto soda = std::make_shared<Food>(Food::FoodType::SODA);
    soda->setAmount(3); // 测试用：添加3个苏打水
    inventory_.push_back(soda);

    auto energyDrink = std::make_shared<Food>(Food::FoodType::ENERGY_DRINK);
    energyDrink->setAmount(1); // 测试用：添加1个能量饮料
    inventory_.push_back(energyDrink);

    auto frozenPizza = std::make_shared<Food>(Food::FoodType::FROZEN_PIZZA);
    frozenPizza->setAmount(0);
    inventory_.push_back(frozenPizza);

    auto chocolateBar = std::make_shared<Food>(Food::FoodType::CHOCOLATE_BAR);
    chocolateBar->setAmount(0);
    inventory_.push_back(chocolateBar);

    auto proteinBar = std::make_shared<Food>(Food::FoodType::PROTEIN_BAR);
    proteinBar->setAmount(0);
    inventory_.push_back(proteinBar);

    // 添加所有有用物品类型，数量为0
    auto boxingGloves = std::make_shared<UsefulItem>(UsefulItem::ItemType::BOXING_GLOVES);
    boxingGloves->setAmount(1); // 测试用：添加1个拳套
    inventory_.push_back(boxingGloves);

    auto gymPass = std::make_shared<UsefulItem>(UsefulItem::ItemType::BOXING_GYM_PASS);
    gymPass->setAmount(0);
    inventory_.push_back(gymPass);
}


// ==================== 敌人解锁进度管理 ====================
bool Player::isEnemyUnlocked(int enemyId) const {
    if (enemyId < 1 || enemyId >= unlockedEnemies_.size()) {
        return false;
    }
    return unlockedEnemies_[enemyId];
}

void Player::unlockEnemy(int enemyId) {
    if (enemyId > 0 && enemyId < unlockedEnemies_.size()) {
        unlockedEnemies_[enemyId] = true;
    }
}

void Player::unlockNextEnemy(int currentEnemyId) {
    int nextEnemyId = currentEnemyId + 1;
    if (nextEnemyId > 0 && nextEnemyId < unlockedEnemies_.size()) {
        unlockedEnemies_[nextEnemyId] = true;
    }
}


double Player::getSkillPoints() const {
    return skillPoints;
}

void Player::addSkillPoints(double value) {
    skillPoints += value;
}

std::vector<std::shared_ptr<Skill>>& Player::getSkills() {
    return skills_;
}

void Player::addSkill(std::shared_ptr<Skill> skill) {
    skills_.push_back(skill);
}

bool Player::learnSkill(const std::string& skillName) {
    auto skill = SkillFactory::createSkillByName(skillName);
    if (skill && skillPoints >= skill->getUnlockCost()) {
        skill->unlock();
        skills_.push_back(skill);
        skillPoints -= skill->getUnlockCost();
        return true;
    }
    return false;
}