#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "../../basic/Game.h"

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
    [[nodiscard]] double getFatigue() const;            // 疲劳值

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
    void addFatigue(double value);                      // 疲劳值
    void addSavings(double value);                      // 积蓄

    // 获取最低属性值
    [[nodiscard]] double getMinStrength() const;        // 最低力量
    [[nodiscard]] double getMinStamina() const;         // 最低耐力
    [[nodiscard]] double getMinAgility() const;         // 最低敏捷
    
    // 设置最低属性值
    void setMinStrength(double value);                  // 最低力量
    void setMinStamina(double value);                   // 最低耐力 
    void setMinAgility(double value);                   // 最低敏捷

    // 销毁Item函数(Food; Medicine)
    /*


    template<typename T>
    bool removeItemFromInventory(const std::shared_ptr<T>& item);


    */

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
    double fatigue;         // 疲劳值

    // 最低属性值
    double minStrength;     // 最低力量
    double minStamina;      // 最低耐力
    double minAgility;      // 最低敏捷

    double money;           // 积蓄
    double skillPoints;     // 技能点
};

#endif // PLAYER_H
