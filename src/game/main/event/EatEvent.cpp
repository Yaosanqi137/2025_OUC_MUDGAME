#include "EatEvent.h"
#include "../basic/Game.h"  
#include "../basic/Dialog.h" 
#include "../class/item/Food.h"
#include "../class/item/Medicine.h"
#include "../class/entity/Player.h"
#include <iostream>
#include <sstream>


// 食物构造函数
EatEvent::EatEvent(std::shared_ptr<Food> food, std::shared_ptr<Player> player, Game& game)
    : food_(std::move(food)), medicine_(nullptr), player_(std::move(player)), game_(game) {}

// 药品构造函数
EatEvent::EatEvent(std::shared_ptr<Medicine> medicine, std::shared_ptr<Player> player, Game& game)
    : food_(nullptr), medicine_(std::move(medicine)), player_(std::move(player)), game_(game) {}



    
void EatEvent::execute() {
    if (!player_) {
        game_.getDialog().addMessage("系统", "食用失败：无效的玩家引用");
        return;
    }

    std::string playerName = player_->getName();

    try {
        if (food_) {
            executeFood();
        } else if (medicine_) {
            executeMedicine();
        } else {
            game_.getDialog().addMessage("系统", "食用失败：无效的物品");
        }

    } catch (const std::exception& e) {
        /* 等待medicine类实现
        std::string itemName = food_ ? food_->getName() : (medicine_ ? medicine_->getName() : "未知物品");
        game_.getDialog().addMessage("系统", "食用 " + itemName + " 时发生错误");
        std::cerr << "EatEvent: Error while consuming - " << e.what() << std::endl;
        */
    }
}


/*



此方法可能存在潜在问题，使用不成功会增加属性，但不会正常移除物品(当移除物品失败时)


*/
void EatEvent::executeFood() {
    std::string foodName = food_->getName();
    std::string playerName = player_->getName();

    // 获取食物效果
    double healthEffect = food_->getHealthEffect();
    double hungerEffect = food_->getHungerEffect();
    double energyEffect = food_->getEnergyEffect();
    
    // 使用食物
    food_->use(*player_);
    
    // 从背包中移除食物
    /*
    
    removeItemFromInventory暂时没有正确实现，需要背包系统接入
    
    */
    if (player_->removeItemFromInventory(food_)) {
        // 第一行：食用成功信息
        game_.getDialog().addMessage("系统", playerName + " 食用了 " + foodName);
        
        // 第二行：属性变化信息
        std::string effectMsg = generateEffectMessage(healthEffect, hungerEffect, energyEffect);
        game_.getDialog().addMessage("系统", effectMsg);
    } else {
        game_.getDialog().addMessage("系统", "食用失败：物品不存在于背包中");
    }
}




/* 等待medicine类实现
void EatEvent::executeMedicine() {
    std::string medicineName = medicine_->getName();
    std::string playerName = player_->getName();

    // 使用药品（假设 Medicine 也有 use 方法）
    medicine_->use(*player_);
    
    // 从背包中移除药品
    if (player_->removeItemFromInventory(medicine_)) {
        // 第一行：使用成功信息
        game_.getDialog().addMessage("系统", playerName + " 使用了 " + medicineName);
        
        // 第二行：药品效果信息（可以根据 Medicine 类添加类似的方法）
        game_.getDialog().addMessage("系统", "药品效果生效");
    } else {
        game_.getDialog().addMessage("系统", "使用失败：物品不存在于背包中");
    }
}
*/

std::string EatEvent::generateEffectMessage(double healthEffect, double hungerEffect, double energyEffect) const {
    std::stringstream effectMsg;
    effectMsg << "效果：";
    
    bool hasEffect = false;
    
    if (healthEffect != 0) {
        effectMsg << "健康度" << (healthEffect > 0 ? "+" : "") << healthEffect << " ";
        hasEffect = true;
    }
    
    if (hungerEffect != 0) {
        effectMsg << "饱食度" << (hungerEffect > 0 ? "+" : "") << hungerEffect << " ";
        hasEffect = true;
    }
    
    if (energyEffect != 0) {
        effectMsg << "体力" << (energyEffect > 0 ? "+" : "") << energyEffect;
        hasEffect = true;
    }
    
    if (!hasEffect) {
        return "没有产生任何效果";
    }
    
    return effectMsg.str();
}

std::string EatEvent::getDescription() const {
    if (!player_) {
        return "无效的食用事件";
    }
    
    std::string itemName;
    if (food_) {
        itemName = food_->getName();
    } else if (medicine_) {
        /* 等待medicine类实现
        itemName = medicine_->getName();
        */
    } else {
        return "无效的物品";
    }
    
    return player_->getName() + " 使用 " + itemName;
}

// 静态工厂方法（食物）
std::shared_ptr<EatEvent> EatEvent::create(std::shared_ptr<Food> food, 
                                          std::shared_ptr<Player> player, 
                                          Game& game) {
    return std::make_shared<EatEvent>(std::move(food), std::move(player), game);
}

// 静态工厂方法（药品）
std::shared_ptr<EatEvent> EatEvent::create(std::shared_ptr<Medicine> medicine, 
                                          std::shared_ptr<Player> player, 
                                          Game& game) {
    return std::make_shared<EatEvent>(std::move(medicine), std::move(player), game);
}