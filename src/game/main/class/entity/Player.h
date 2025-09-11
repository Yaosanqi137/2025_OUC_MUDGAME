#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "../../basic/Game.h"
#include "../item/AbstractItem.h"
class Game;

class Player {
public:
    explicit Player(Game& game_logic);
    ~Player();

    const std::string& getName();
    void setName(const std::string& name);

    // 位置相关
    const std::string& getLocation() const;
    void setLocation(const std::string& location);

    // 获取核心属性
    [[nodiscard]] double getHealthiness() const;        // 健康度
    [[nodiscard]] double getStrength() const;           // 力量
    [[nodiscard]] double getStamina() const;            // 耐力
    [[nodiscard]] double getAgility() const;            // 敏捷
    [[nodiscard]] double getHunger() const;             // 饱食度
    [[nodiscard]] double getFatigue() const;            // 体力

    [[nodiscard]] double getSavings() const;            // 积蓄

    // 获取本身属性
    [[nodiscard]] double getSpeed() const;              // 速度
    [[nodiscard]] double getHealth() const;             // 血量
    [[nodiscard]] double getEnergy() const;             // 体力槽


    // 更改属性
    void addHealthiness(double value);                  // 健康度
    void addStrength(double value);                     // 力量
    void addStamina(double value);                      // 耐力
    void addAgility(double value);                      // 敏捷
    void addHunger(double value);                       // 饱食度
    void addFatigue(double value);                      // 体力
    void addSavings(double value);                      // 积蓄
    void addHealth(double value);                       // 生命值

    // 获取最低属性值
    [[nodiscard]] double getMinStrength() const;        // 最低力量
    [[nodiscard]] double getMinStamina() const;         // 最低耐力
    [[nodiscard]] double getMinAgility() const;         // 最低敏捷
    
    // 设置最低属性值
    void setMinStrength(double value);                  // 最低力量
    void setMinStamina(double value);                   // 最低耐力 
    void setMinAgility(double value);                   // 最低敏捷

    // 获取最高属性值
    [[nodiscard]] double getMaxHunger() const;          // 最高饱食度
    [[nodiscard]] double getMaxFatigue() const;         // 最高体力
    [[nodiscard]] double getMaxHealth() const;          // 最高生命值

    // 设置最高属性值
    void setMaxHunger(double value);                    // 最高饱食度
    void setMaxFatigue(double value);                   // 最高体力
    void setMaxHealth(double value);                    // 最高生命值

    // 背包相关操作
    std::vector<std::shared_ptr<AbstractItem>>& getInventory();

    // 获取数量大于0的物品（用于UI显示）
    std::vector<std::shared_ptr<AbstractItem>> getDisplayableItems() const;

    // 添加物品到背包
    void addItem(std::shared_ptr<AbstractItem> item);

    // 根据物品类型添加物品
    void addItemByType(const std::string& itemType, int amount = 1);

    // 修改物品数量
    bool setItemAmount(const std::string& itemName, int amount);
    bool addItemAmount(const std::string& itemName, int amount);
    bool removeItemAmount(const std::string& itemName, int amount);

    // 移除物品
    bool removeItemFromInventory(const std::shared_ptr<AbstractItem>& item);
    
    // 基于名称查找物品
    std::shared_ptr<AbstractItem> findItemByName(const std::string& name);

    // 使用物品
    bool useItem(const std::string& itemName);

    // 初始化背包（添加所有物品的单例，数量为0）
    void initializeInventory();


protected:
    Game& game_logic_;

private:
    std::string name;       // 玩家名
    std::string location;   // 位置

    // 核心属性
    double healthiness;     // 健康度
    double strength;        // 力量
    double stamina;         // 耐力
    double agility;         // 敏捷
    double hunger;          // 饱食度
    double fatigue;         // 体力
    double health;          // 生命值

    // 最低属性值
    double minStrength;     // 最低力量
    double minStamina;      // 最低耐力
    double minAgility;      // 最低敏捷

    // 最高属性值
    double maxHunger;       // 最高饱食度
    double maxFatigue;      // 最高体力
    double maxHealth;       // 最高生命值

    double money;           // 积蓄
    double skillPoints;     // 技能点

    std::vector<std::shared_ptr<AbstractItem>> inventory_;  // 背包
};

#endif // PLAYER_H
