/*

Last changed on 9-11 1:00 by Anyeling
Bug to fix: removeItemFromInventory模板函数无法链接 不能显式声明
Bug to fix: not link to BagLayout(ui) yet

*/


#include "Player.h"
#include "../item/Food.h"
#include "../item/UsefulItem.h"
#include "../../event/TrainingEvent.h"
#include "../item/Medicine.h"

Player::Player(Game& game_logic) : game_logic_(game_logic), name("NOT_SET") , strength(1),
                   stamina(1), agility(1), hunger(80), fatigue(80), money(100), location("???"), health(100),
                   minStrength(0), minStamina(0), minAgility(0), skillPoints(5),
                   maxHunger(80), maxFatigue(80), maxHealth(100),
                   exMaxHunger(0), exMaxFatigue(0), exMaxHealth(0),
                   sustainDamageRate(1.0),lowerBodySustainDamageRate(1.0),upperBodySustainDamageRate(1.0),
                   fatigueConsumeRate(1.0),exHitRate(0.0),
                   gameDifficulty(2),
                   lastDecayYear(GameTime::getYear()),   // 初始化上次衰减年份
                   lastDecayMonth(GameTime::getMonth()), // 初始化上次衰减月份
                   lastDecayDay(GameTime::getDay())      // 初始化上次衰减日期
                   {
    // 在构造函数中初始化背包
    initializeInventory();

    // 初始化技能树（可以初始化已学技能，暂时留个端口）
    // skillTreeManager_.addLearnedSkill("直拳");
    unlockedEnemies_.resize(12);
    unlockedEnemies_[0] = true;
    unlockedEnemies_[1] = true;
    // 初始化训练系统
    trainingSystem = std::make_shared<TrainingEvent>(std::shared_ptr<Player>(this, [](Player*){}));
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
    return agility;
}

double Player::getHealth() const {
    return health;
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
    hunger = std::min(maxHunger + exMaxHunger, hunger);

    // 急救
    if (hunger <= 0.0) {
        triggerEmergencyRescue();
    }
}

void Player::addFatigue(const double value) {
    fatigue += value;
    fatigue = std::max(0.0, fatigue);
    fatigue = std::min(maxFatigue + exMaxFatigue, fatigue);

    // 急救
    if (fatigue <= 0.0) {
        triggerEmergencyRescue();
    }
}

void Player::addHealth(double value) {
    health += value;
    health = std::max(0.0, health);
    health = std::min(health, maxHealth + exMaxHealth);

    // 急救
    if (health <= 0.0) {
        triggerEmergencyRescue();
    }
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
    } else if (itemType == "弗兰克的名片") {
        auto item = std::make_shared<UsefulItem>(UsefulItem::ItemType::CARD);
        item->setAmount(amount);
        addItem(item);
    } else if (itemType == "禁药") {
        auto item = std::make_shared<UsefulItem>(UsefulItem::ItemType::BANNED_DRUG);
        item->setAmount(amount);
        addItem(item);
    } else if (itemType == "回生丹") {
        auto medicine = std::make_shared<Medicine>(Medicine::MedicineType::REVIVAL_PILL);
        medicine->setAmount(amount);
        addItem(medicine);
    } else if (itemType == "力量药水") {
        auto medicine = std::make_shared<Medicine>(Medicine::MedicineType::STRENGTH_POTION);
        medicine->setAmount(amount);
        addItem(medicine);
    } else if (itemType == "敏捷药水") {
        auto medicine = std::make_shared<Medicine>(Medicine::MedicineType::AGILITY_POTION);
        medicine->setAmount(amount);
        addItem(medicine);
    } else if (itemType == "耐力药水") {
        auto medicine = std::make_shared<Medicine>(Medicine::MedicineType::STAMINA_POTION);
        medicine->setAmount(amount);
        addItem(medicine);
    } else if (itemType == "技能点药水") {
        auto medicine = std::make_shared<Medicine>(Medicine::MedicineType::SKILL_POINT_POTION);
        medicine->setAmount(amount);
        addItem(medicine);
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
    meat->setAmount(0);
    inventory_.push_back(meat);

    auto soda = std::make_shared<Food>(Food::FoodType::SODA);
    soda->setAmount(0);
    inventory_.push_back(soda);

    auto energyDrink = std::make_shared<Food>(Food::FoodType::ENERGY_DRINK);
    energyDrink->setAmount(0);
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
    boxingGloves->setAmount(0);
    inventory_.push_back(boxingGloves);

    auto gymPass = std::make_shared<UsefulItem>(UsefulItem::ItemType::BOXING_GYM_PASS);
    gymPass->setAmount(1);
    inventory_.push_back(gymPass);

    auto card = std::make_shared<UsefulItem>(UsefulItem::ItemType::CARD);
    card->setAmount(1);
    inventory_.push_back(card);

    auto bannedDrug = std::make_shared<UsefulItem>(UsefulItem::ItemType::BANNED_DRUG);
    bannedDrug->setAmount(0);
    inventory_.push_back(bannedDrug);

    auto revivalPill = std::make_shared<Medicine>(Medicine::MedicineType::REVIVAL_PILL);
    revivalPill->setAmount(0);
    inventory_.push_back(revivalPill);

    auto strengthPotion = std::make_shared<Medicine>(Medicine::MedicineType::STRENGTH_POTION);
    strengthPotion->setAmount(0);
    inventory_.push_back(strengthPotion);

    auto agilityPotion = std::make_shared<Medicine>(Medicine::MedicineType::AGILITY_POTION);
    agilityPotion->setAmount(0);
    inventory_.push_back(agilityPotion);

    auto staminaPotion = std::make_shared<Medicine>(Medicine::MedicineType::STAMINA_POTION);
    staminaPotion->setAmount(0);
    inventory_.push_back(staminaPotion);

    auto skillPointPotion = std::make_shared<Medicine>(Medicine::MedicineType::SKILL_POINT_POTION);
    skillPointPotion->setAmount(0);
    inventory_.push_back(skillPointPotion);
}

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


// ========= 技能树管理器 ==========
bool Player::learnSkill(const std::string& skillName) {
    auto skill = SkillFactory::createSkillByName(skillName);
    if (!skill) {
        return false;
    }
    
    // 使用技能树管理器检查
    if (!skillTreeManager.tryLearnSkill(skillName, skillPoints)) {
        return false;
    }
    
    // 解锁技能并消耗技能点
    skill->unlock();
    skills_.push_back(skill);
    skillPoints -= skill->getUnlockCost();
    
    if(skillName == "自杀式袭击"){
        exHitRate -= 0.1;
        fatigueConsumeRate - 0.2;
    }
    else if(skillName == "钝兵挫锐"){
        sustainDamageRate -= 0.2;
    }
    else if(skillName == "拳击手"){
        exHitRate += 0.1;
    }
    else if(skillName == "千手不破"){
        upperBodySustainDamageRate -= 0.3;
        lowerBodySustainDamageRate += 0.2;
    }

    // 自身增益
    skill -> execute(*this,*this);

    return true;
}

bool Player::canLearnSkill(const std::string& skillName) const {
    auto skill = SkillFactory::createSkillByName(skillName);
    if (!skill) {
        return false;
    }
    
    return skillTreeManager.canLearnSkill(skillName) && (skillPoints >= skill->getUnlockCost());
}

std::vector<std::string> Player::getAvailableSkills() const {
    return skillTreeManager.getAvailableSkills();
}

std::vector<std::string> Player::getLearnedSkillNames() const {
    return skillTreeManager.getLearnedSkills();
}

// 获取和设置游戏难度
int Player::getGameDifficulty() const{return gameDifficulty;}
void Player::setGameDifficulty(int level) {gameDifficulty = level;}

// 训练系统相关
std::shared_ptr<TrainingEvent> Player::getTrainingSystem() {
    return trainingSystem;
}

bool Player::train(TrainingType type) {
    bool result = trainingSystem->train(type, this->game_logic_);
    checkAndApplyDailyDecay(getGameLogic()); // 训练后检查日期变化并应用衰减
    return result;
}

bool Player::canTrain(TrainingType type) const {
    return trainingSystem->canTrain(type);
}

// 下面是特殊技能效果函数
double Player::getSustainDamageRate() const{return sustainDamageRate;}
double Player::getUpperBodySustainDamageRate() const{return upperBodySustainDamageRate;}
double Player::getLowerBodySustainDamageRate() const{return lowerBodySustainDamageRate;}
double Player::getFatigueConsumeRate() const{return fatigueConsumeRate;}
double Player::getExHitRate() const{return exHitRate;}

void Player::setSustainDamageRate(double rate){sustainDamageRate = rate;}
void Player::setUpperBodySustainDamageRate(double rate){upperBodySustainDamageRate = rate;}
void Player::setLowerBodySustainDamageRate(double rate){lowerBodySustainDamageRate = rate;}
void Player::setFatigueConsumeRate(double rate){fatigueConsumeRate = rate;}
void Player::setExHitRate(double rate){exHitRate = rate;}
// 上面是特殊技能效果函数


// 获取和设置额外最大属性值(吃东西或使用物品改变)
double Player::getExMaxHealth() const{return exMaxHealth;}
double Player::getExMaxFatigue() const{return exMaxFatigue;}
double Player::getExMaxHunger() const{return exMaxHunger;}
void Player::setExMaxHealth(double value) {exMaxHealth = value;}
void Player::setExMaxFatigue(double value) {exMaxFatigue = value;}
void Player::setExMaxHunger(double value) {exMaxHunger = value;}

// 指令集( Skill )展示
std::vector<std::string> Player::getAllSkillsInfo() const {
    std::vector<std::string> result;
    auto allSkillNames = SkillFactory::getAllSkillNames();
    
    for (const auto& skillName : allSkillNames) {
        auto skill = SkillFactory::createSkillByName(skillName);
        if (skill) {
            std::string info = "[" + std::to_string(skill->getId()) + "] " + 
                              skillName + " - " + skill->getDescription() + 
                              " (消耗:" + std::to_string(skill->getUnlockCost()) + "技能点)";
            result.push_back(info);
        }
    }
    
    return result;
}

std::vector<std::string> Player::getLearnableSkillsInfo() const {
    std::vector<std::string> result;
    auto availableSkills = skillTreeManager.getAvailableSkills();
    
    for (const auto& skillName : availableSkills) {
        auto skill = SkillFactory::createSkillByName(skillName);
        if (skill) {
            std::string status = "可学习";
            if (skillPoints < skill->getUnlockCost()) {
                status = "技能点不足 (需要:" + std::to_string(skill->getUnlockCost()) + 
                        ", 当前:" + std::to_string((int)skillPoints) + ")";
            }
            
            std::string info = "[" + std::to_string(skill->getId()) + "] " + 
                              skillName + " - " + skill->getDescription() + 
                              " (" + status + ")";
            result.push_back(info);
        }
    }
    
    return result;
}

bool Player::learnSkillById(int skillId) {
    // 通过ID找到技能名称
    std::string skillName;
    for (const auto& skill : SkillFactory::getAllSkillNames()) {
        auto skillObj = SkillFactory::createSkillByName(skill);
        if (skillObj && skillObj->getId() == skillId) {
            skillName = skill;
            break;
        }
    }
    
    if (skillName.empty()) {
        return false;
    }
    
    return learnSkill(skillName);
}

// 返回游戏引用
Game& Player::getGameLogic() {return game_logic_;}

int Player::getHighestUnlockedEnemy() const {
    // 从最高ID开始向下查找，找到第一个已解锁的敌人
    for (int i = unlockedEnemies_.size() - 1; i >= 0; --i) {
        if (unlockedEnemies_[i]) {
            return i;
        }
    }
    return 0; // 如果没有解锁任何敌人，返回0
}

void Player::checkAndApplyDailyDecay(Game& game) {
    unsigned int currentYear = GameTime::getYear();
    unsigned int currentMonth = GameTime::getMonth();
    unsigned int currentDay = GameTime::getDay();

    // 检查日期是否变化（年、月、日任意一个变化）
    if (currentYear != lastDecayYear || currentMonth != lastDecayMonth || currentDay != lastDecayDay) {
        // 调用训练系统的每日经验衰减
        trainingSystem->applyDailyExperienceDecay(game);
        
        // 更新记录日期
        lastDecayYear = currentYear;
        lastDecayMonth = currentMonth;
        lastDecayDay = currentDay;
        
        // 可选：添加日志消息
        game_logic_.getDialog().addMessage("<SYSTEM>", "已经经过了一天，训练经验有所衰减。");
    }
}

// 急救系统实现
void Player::triggerEmergencyRescue() {
    // 输出急救信息
    game_logic_.getDialog().addMessage("", "你晕了过去，有人拨打了急救电话，将你送去医院了...");

    game_logic_.getDialog().addMessage("", "...");
    game_logic_.getDialog().addMessage("", "...");
    game_logic_.getDialog().addMessage("", "...");
    game_logic_.getDialog().addMessage("", "...");

    // 输出恢复信息
    game_logic_.getDialog().addMessage("", "当你醒来时，医生已经将你治好，但是你还没有完全恢复，身体还很虚弱");

    // 设置玩家状态
    health = 10.0;    // 生命值设置为10
    hunger = 50.0;    // 饥饿值设置为50
    fatigue = 30.0;   // 体力设置为30

    if (getSavings() < 180) {
        game_logic_.getDialog().addMessage("医生", "小伙子，你并没有足够的钱医疗");
        game_logic_.getDialog().addMessage("医生", "但是你的保险替你还清了剩下的医疗费");
        game_logic_.getDialog().addMessage("医生", "下次可要注意身体啊");
        addSavings(-getSavings()); // 钱包清零
    } else {
        game_logic_.getDialog().addMessage("医生", "小伙子，你醒啦？");
        game_logic_.getDialog().addMessage("医生", "这次没受什么伤，但下次可要注意身体啊");
        game_logic_.getDialog().addMessage("医生", "对了，医疗费 180 元，已经缴纳了");
        addSavings(-180);
    }
    game_logic_.getDialog().addMessage("<PLAYER_NAME>", "我去，180块钱这就没了？");
    game_logic_.getDialog().addMessage("", "你看你身上没什么事，于是又回去做刚刚做的事情了");
}
