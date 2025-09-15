#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "../../basic/Game.h"
#include "../../basic/Dialog.h"
#include "../item/AbstractItem.h"
#include "../skill/Skill.h"
#include "../skill/SkillFactory.h"
#include "../skill/SkillTreeManager.h"
#include "../../event/TrainingEvent.h"

enum class TrainingType;
class TrainingEvent;
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
    [[nodiscard]] double getStrength() const;           // 力量
    [[nodiscard]] double getStamina() const;            // 耐力
    [[nodiscard]] double getAgility() const;            // 敏捷
    [[nodiscard]] double getHunger() const;             // 饱食度
    [[nodiscard]] double getFatigue() const;            // 体力
    [[nodiscard]] double getHealth() const;             // 生命值

    [[nodiscard]] double getSavings() const;            // 积蓄

    // 获取本身属性
    [[nodiscard]] double getSpeed() const;              // 速度


    // 更改属性
    void addStrength(double value);                     // 力量
    void addStamina(double value);                      // 耐力
    void addAgility(double value);                      // 敏捷
    
    void addHunger(double value);                       // 饱食度
    void addFatigue(double value);                      // 体力
    void addSavings(double value);                      // 积蓄
    void addHealth(double value);                       // 生命值

    // 急救系统
    void triggerEmergencyRescue();                      // 触发急救逻辑

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
    [[nodiscard]] double getMaxStamina() const;         // 最高耐力

    // 设置最高属性值
    void setMaxHunger(double value);                    // 最高饱食度
    void setMaxFatigue(double value);                   // 最高体力
    void setMaxHealth(double value);                    // 最高生命值
    void setMaxStamina(double value);                   // 最高耐力

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

    // 技能点操作
    double getSkillPoints() const;
    void addSkillPoints(double value);

    // 技能操作
    std::vector<std::shared_ptr<Skill>>& getSkills();
    void addSkill(std::shared_ptr<Skill> skill);

    // 技能学习相关
    bool learnSkill(const std::string& skillName);
    bool canLearnSkill(const std::string& skillName) const;
    std::vector<std::string> getAvailableSkills() const;
    std::vector<std::string> getLearnedSkillNames() const;

    
    // 初始化背包（添加所有物品的单例，数量为0）
    void initializeInventory();


    // 敌人解锁相关
    bool isEnemyUnlocked(int enemyId) const;
    void unlockEnemy(int enemyId);
    void unlockNextEnemy(int currentEnemyId);
    int getHighestUnlockedEnemy() const;

    // 训练系统相关
    std::shared_ptr<TrainingEvent> getTrainingSystem();
    bool train(TrainingType type);
    bool canTrain(TrainingType type) const;

    // 下面是特殊技能效果函数
    double getSustainDamageRate() const;
    double getUpperBodySustainDamageRate() const;
    double getLowerBodySustainDamageRate() const;
    double getFatigueConsumeRate() const;
    double getExHitRate() const;

    void setSustainDamageRate(double rate);
    void setUpperBodySustainDamageRate(double rate);
    void setLowerBodySustainDamageRate(double rate);
    void setFatigueConsumeRate(double rate);
    void setExHitRate(double rate);
    // 上面是特殊技能效果函数

    // 获取和设置额外的最大属性值(吃东西或使用物品改变)
    double getExMaxHealth() const;
    double getExMaxFatigue() const;
    double getExMaxHunger() const;
    void setExMaxHealth(double value);
    void setExMaxFatigue(double value);
    void setExMaxHunger(double value);

    // 获取和设置游戏难度
    int getGameDifficulty() const;
    void setGameDifficulty(int level);

    // 指令集( Skill )提示
    std::vector<std::string> getAllSkillsInfo() const;
    std::vector<std::string> getLearnableSkillsInfo() const;
    bool learnSkillById(int skillId);

    Game& getGameLogic();

    // 检查日期变化并应用衰减
    void checkAndApplyDailyDecay(Game& game);
protected:
    Game& game_logic_;

private:
    std::string name;       // 玩家名
    std::string location;   // 位置

    int gameDifficulty;      // 玩家的游戏难度 ( 1是简单, 2是普通, 3是困难 )

    // 核心属性
    double strength;        // 力量
    double stamina;         // 耐力
    double agility;         // 敏捷
    double hunger;          // 饱食度(数值)
    double fatigue;         // 体力值(数值)
    double health;          // 生命值(数值)

    // 最低属性值
    double minStrength;     // 最低力量
    double minStamina;      // 最低耐力
    double minAgility;      // 最低敏捷

    // 最高属性值
    double maxHunger;       // 最高饱食度(饥饿槽)
    double maxFatigue;      // 最高体力值(体力槽)
    double maxHealth;       // 最高生命值(血量槽)
    double maxStamina;      // 最高耐力值

    // 吃东西或技能增加的血量上限，体力上限，饱食上限
    double exMaxHunger;
    double exMaxFatigue;
    double exMaxHealth;


    // 下面这块是特殊技能专用属性
    double sustainDamageRate;           // 普遍受到伤害倍数
    double upperBodySustainDamageRate;  // 上肢受到伤害倍数
    double lowerBodySustainDamageRate;     // 下肢施加伤害倍数
    double fatigueConsumeRate;          // 体力消耗倍数
    double exHitRate;                   // 额外命中率
    // 上面这块是特殊技能专用属性


    double money;           // 积蓄
    double skillPoints;     // 技能点

    std::vector<std::shared_ptr<Skill>> skills_;    // 已解锁的技能列表

    SkillTreeManager skillTreeManager; // 技能树管理器

    std::vector<std::shared_ptr<AbstractItem>> inventory_;  // 背包

    std::vector<bool> unlockedEnemies_; // 解锁的敌人列表

    // ATTENTION: 每日训练经验减少调用
    // player->getTrainingSystem()->applyDailyExperienceDecay();
    std::shared_ptr<TrainingEvent> trainingSystem; // 训练系统

    // 用于训练系统计算是否衰减经验的属性
    unsigned int lastDecayYear;  // 上次应用衰减的年份
    unsigned int lastDecayMonth; // 上次应用衰减的月份
    unsigned int lastDecayDay;   // 上次应用衰减的日期
};

#endif // PLAYER_H
