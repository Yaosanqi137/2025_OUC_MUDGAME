#include "SaveClass.h"
#include "Game.h"
#include "GameTime.h"
#include "../class/entity/Player.h"
#include "../class/item/UsefulItem.h"
#include "../event/TrainingEvent.h"
#include <fstream>
#include <iostream>

SaveClass* SaveClass::getInstance() {
    static SaveClass instance;
    return &instance;
}

bool SaveClass::doesSaveExist(const std::string& filePath) const {
    return std::filesystem::exists(filePath);
}

void SaveClass::saveGame(Game& game, const std::string& filePath) {
    // 临时文件
    const std::string tempFilePath = filePath + ".tmp";

    toml::table saveData;

    // --- 1. 保存 Game 全局数据 ---
    saveData["game"]["is_first_new_game"] = game.isFirstNewGame;

    // 保存 GameState (带安全过滤)
    GameState stateToSave = game.getCurrentState();
    if (stateToSave == GameState::AwaitingChoice ||
        stateToSave == GameState::AwaitingTextInput) {
        // 将危险状态降级为 InGame，以确保加载后可以正常交互
        stateToSave = GameState::InGame;
        }
    // 将枚举转换为整数进行存储
    saveData["game"]["game_state"] = static_cast<int>(stateToSave);

    // --- 2. 保存 GameTime ---
    auto& timeTable = saveData["gametime"];
    timeTable["year"] = GameTime::getYear();
    timeTable["month"] = GameTime::getMonth();
    timeTable["day"] = GameTime::getDay();
    timeTable["hour"] = GameTime::getHour();
    timeTable["minute"] = GameTime::getMinute();

    // --- 3. 保存 Player 数据 ---
    Player& player = game.getPlayer();
    auto& playerTable = saveData["player"];
    playerTable["name"] = player.getName();
    playerTable["location"] = player.getLocation();
    playerTable["money"] = player.getSavings();
    playerTable["skill_points"] = player.getSkillPoints();
    playerTable["visited_locations"] = player.getVisitedLocations();

    // 3.1 保存核心属性
    auto& attributesTable = playerTable["attributes"];
    attributesTable["strength"] = player.getStrength();
    attributesTable["stamina"] = player.getStamina();
    attributesTable["agility"] = player.getAgility();
    attributesTable["hunger"] = player.getHunger();
    attributesTable["fatigue"] = player.getFatigue();
    attributesTable["health"] = player.getHealth();
    attributesTable["min_strength"] = player.getMinStrength();
    attributesTable["min_stamina"] = player.getMinStamina();
    attributesTable["min_agility"] = player.getMinAgility();
    attributesTable["max_hunger"] = player.getMaxHunger();
    attributesTable["max_fatigue"] = player.getMaxFatigue();
    attributesTable["max_health"] = player.getMaxHealth();
    attributesTable["ex_max_hunger"] = player.getExMaxHunger();
    attributesTable["ex_max_fatigue"] = player.getExMaxFatigue();
    attributesTable["ex_max_health"] = player.getExMaxHealth();
    // 被动属性
    auto& combatPassivesTable = playerTable["combat_passives"];
    combatPassivesTable["sustain_damage_rate"] = player.getSustainDamageRate();
    combatPassivesTable["upper_body_sustain_damage_rate"] = player.getUpperBodySustainDamageRate();
    combatPassivesTable["lower_body_sustain_damage_rate"] = player.getLowerBodySustainDamageRate();
    combatPassivesTable["fatigue_consume_rate"] = player.getFatigueConsumeRate();
    combatPassivesTable["ex_hit_rate"] = player.getExHitRate();

    // 3.2 保存已学技能 (只保存名称)
    playerTable["learned_skills"] = player.getLearnedSkillNames();

    // 3.3 保存背包物品
    toml::array inventoryArr;
    for (const auto& itemPtr : player.getInventory()) {
        if (itemPtr->getAmount() > 0) {
            toml::table itemData{
                {"name", itemPtr->getName()},
                {"amount", itemPtr->getAmount()}
            };

            // ++ 新增：检查并保存特殊状态 ++
            // 尝试将其动态转换为 UsefulItem
            if (auto usefulItem = std::dynamic_pointer_cast<UsefulItem>(itemPtr)) {
                // 如果是，则保存其耐久度
                itemData["durability"] = usefulItem->getDurability();
            }

            inventoryArr.push_back(itemData);
        }
    }
    playerTable["inventory"] = inventoryArr;

    // 3.4 保存已解锁的敌人
    toml::array unlockedEnemiesArr;
    for (int i = 0; i < 12; ++i) { // 假设总共有12个敌人
        unlockedEnemiesArr.push_back(player.isEnemyUnlocked(i));
    }
    playerTable["unlocked_enemies"] = unlockedEnemiesArr;
    
    // 3.5 保存训练系统数据
    auto trainingSystem = player.getTrainingSystem();
    auto& trainingTable = playerTable["training"];
    trainingTable["strength_exp"] = trainingSystem->getStrengthExp();
    trainingTable["agility_exp"] = trainingSystem->getAgilityExp();
    trainingTable["stamina_exp"] = trainingSystem->getStaminaExp();
    trainingTable["strength_level"] = trainingSystem->getStrengthLevel();
    trainingTable["agility_level"] = trainingSystem->getAgilityLevel();
    trainingTable["stamina_level"] = trainingSystem->getStaminaLevel();
    trainingTable["strength_exp_rate"] = trainingSystem->getStrengthExpRate();
    trainingTable["agility_exp_rate"] = trainingSystem->getAgilityExpRate();
    trainingTable["stamina_exp_rate"] = trainingSystem->getStaminaExpRate();
    trainingTable["money_cost_rate"] = trainingSystem->getMoneyCostRate();
    trainingTable["has_side_effect"] = trainingSystem->getHasSideEffect();

    // 保存每日衰减的时间戳
    auto& decayTable = playerTable["decay_timestamp"];
    // 需要在 Player 类中添加 getter
    decayTable["year"] = player.getLastDecayYear();
    decayTable["month"] = player.getLastDecayMonth();
    decayTable["day"] = player.getLastDecayDay();

    // --- 写入文件 ---
    try {
        // 步骤 1: 写入临时文件
        std::ofstream tempFile(tempFilePath);
        if (!tempFile.is_open()) {
            game.getDialog().addMessage("<SYSTEM>", "错误: 无法创建临时存档文件。");
            return;
        }
        tempFile << toml::format(toml::value(saveData));
        tempFile.close(); // 确保写入完成

        // 步骤 2: 写入成功后，用临时文件替换正式文件
        std::filesystem::rename(tempFilePath, filePath);

        game.getDialog().addMessage("<SYSTEM>", "游戏已成功保存。");

    } catch (const std::exception& e) {
        game.getDialog().addMessage("<SYSTEM>", "保存失败: " + std::string(e.what()));
        std::cerr << "Error saving game: " << e.what() << std::endl;
        // 清理失败的临时文件
        std::filesystem::remove(tempFilePath);
    }
}

bool SaveClass::loadGame(Game& game, const std::string& filePath) {
    if (!doesSaveExist(filePath)) {
        game.getDialog().addMessage("<SYSTEM>", "错误: 存档文件不存在。");
        return false;
    }

    try {
        const auto data = toml::parse(filePath);

        // --- 1. 加载 Game 全局数据 ---
        game.isFirstNewGame = toml::find_or<bool>(data, "game", "is_first_new_game", true);

        // 加载 GameState
        // 从整数恢复枚举。如果存档中没有这个字段，则默认为 InGame (0)
        int loadedStateInt = toml::find_or<int>(data, "game", "game_state", static_cast<int>(GameState::InGame));
        game.setGameState(static_cast<GameState>(loadedStateInt));

        // --- 2. 加载 GameTime ---
        const auto& timeTable = toml::find(data, "gametime");
        GameTime::setTime(
            toml::find<unsigned int>(timeTable, "year"),
            toml::find<unsigned int>(timeTable, "month"),
            toml::find<unsigned int>(timeTable, "day"),
            toml::find<unsigned int>(timeTable, "hour"),
            toml::find<unsigned int>(timeTable, "minute")
        );

        // --- 3. 加载 Player 数据 ---
        Player& player = game.getPlayer();
        const auto& playerTable = toml::find(data, "player");

        player.setName(toml::find<std::string>(playerTable, "name"));
        player.setLocation(toml::find<std::string>(playerTable, "location"));
        player.setSavings(toml::find_or<double>(playerTable, "money", 100.0));
        player.setSkillPoints(toml::find_or<double>(playerTable, "skill_points", 5.0));
        // 已访问地点
        auto& visitedSet = player.getVisitedLocations();
        visitedSet.clear();
        const auto visitedArr = toml::find_or<toml::array>(playerTable, "visited_locations", toml::array{});
        for (const auto& locVal : visitedArr) {
            visitedSet.insert(locVal.as_string());
        }

        // 3.1 加载核心属性
        const auto& attr = toml::find(playerTable, "attributes");
        player.setStrength(toml::find_or<double>(attr, "strength", 1.0));
        player.setStamina(toml::find_or<double>(attr, "stamina", 1.0));
        player.setAgility(toml::find_or<double>(attr, "agility", 1.0));
        player.setHunger(toml::find_or<double>(attr, "hunger", 80.0));
        player.setFatigue(toml::find_or<double>(attr, "fatigue", 80.0));
        player.setHealth(toml::find_or<double>(attr, "health", 100.0));
        player.setMinStrength(toml::find<double>(attr, "min_strength"));
        player.setMinStamina(toml::find<double>(attr, "min_stamina"));
        player.setMinAgility(toml::find<double>(attr, "min_agility"));
        player.setMaxHunger(toml::find<double>(attr, "max_hunger"));
        player.setMaxFatigue(toml::find<double>(attr, "max_fatigue"));
        player.setMaxHealth(toml::find<double>(attr, "max_health"));
        player.setExMaxHunger(toml::find<double>(attr, "ex_max_hunger"));
        player.setExMaxFatigue(toml::find<double>(attr, "ex_max_fatigue"));
        player.setExMaxHealth(toml::find<double>(attr, "ex_max_health"));
        // 被动属性
        if (playerTable.contains("combat_passives")) {
            const auto& passives = toml::find(playerTable, "combat_passives");
            player.setSustainDamageRate(toml::find_or<double>(passives, "sustain_damage_rate", 1.0));
            player.setUpperBodySustainDamageRate(toml::find_or<double>(passives, "upper_body_sustain_damage_rate", 1.0));
            player.setLowerBodySustainDamageRate(toml::find_or<double>(passives, "lower_body_sustain_damage_rate", 1.0));
            player.setFatigueConsumeRate(toml::find_or<double>(passives, "fatigue_consume_rate", 1.0));
            player.setExHitRate(toml::find_or<double>(passives, "ex_hit_rate", 0.0));
        }

        // 3.2 加载已学技能
        const auto learnedSkillsArr = toml::find<toml::array>(playerTable, "learned_skills");
        player.getSkills().clear();
        for (const auto& skillNameVal : learnedSkillsArr) {
            auto skill = SkillFactory::createSkillByName(skillNameVal.as_string());
            if (skill) {
                player.addSkill(skill);
            }
        }

        // 在所有技能都加载完毕后，同步技能树管理器
        player.resyncSkillTreeManager();
        
        // 3.3 加载背包 (先重置，再加载)
        player.initializeInventory(); // 这会创建所有物品的实例，数量为0
        const auto inventoryArr = toml::find<toml::array>(playerTable, "inventory");
        for (const auto& item_val : inventoryArr) {
            const std::string item_name = toml::find<std::string>(item_val, "name");
            const int item_amount = toml::find<int>(item_val, "amount");

            auto itemPtr = player.findItemByName(item_name);
            if (itemPtr) {
                itemPtr->setAmount(item_amount);

                // 检查并加载特殊状态
                if (auto usefulItem = std::dynamic_pointer_cast<UsefulItem>(itemPtr)) {
                    // 如果存档中有耐久度数据，则加载它
                    int durability = toml::find_or<int>(item_val, "durability", -1);
                    if (durability != -1) {
                        usefulItem->setDurability(durability);
                    }
                }
            }
        }

        // 3.4 加载解锁的敌人
        const auto unlockedEnemiesArr = toml::find<toml::array>(playerTable, "unlocked_enemies");
        for (size_t i = 0; i < unlockedEnemiesArr.size(); ++i) {
            if (unlockedEnemiesArr.at(i).as_boolean()) {
                player.unlockEnemy(i);
            }
        }
        
        // 3.5 加载训练数据
        auto trainingSystem = player.getTrainingSystem();
        const auto& train_table = toml::find(playerTable, "training");
        trainingSystem->addStrengthExp(toml::find<double>(train_table, "strength_exp"));
        trainingSystem->addAgilityExp(toml::find<double>(train_table, "agility_exp"));
        trainingSystem->addStaminaExp(toml::find<double>(train_table, "stamina_exp"));
        trainingSystem->setStrengthExpRate(toml::find_or<double>(train_table, "strength_exp_rate", 1.0));
        trainingSystem->setAgilityExpRate(toml::find_or<double>(train_table, "agility_exp_rate", 1.0));
        trainingSystem->setStaminaExpRate(toml::find_or<double>(train_table, "stamina_exp_rate", 1.0));
        trainingSystem->setMoneyCostRate(toml::find<double>(train_table, "money_cost_rate"));
        trainingSystem->setHasSideEffect(toml::find<bool>(train_table, "has_side_effect"));
        if (playerTable.contains("decay_timestamp")) {
            const auto& decayTable = toml::find(playerTable, "decay_timestamp");
            // 需要在 Player 类中添加 setter
            player.setLastDecayTimestamp(
                toml::find<unsigned int>(decayTable, "year"),
                toml::find<unsigned int>(decayTable, "month"),
                toml::find<unsigned int>(decayTable, "day")
            );
        }
        game.getDialog().addMessage("<SYSTEM>", "存档已成功加载。");
        return true;

    } catch (const std::exception& e) {
        game.getDialog().addMessage("<SYSTEM>", "加载失败: " + std::string(e.what()));
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}