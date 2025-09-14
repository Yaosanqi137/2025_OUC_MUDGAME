#ifndef EATEVENT_H
#define EATEVENT_H

#include <memory>
#include <string>

// 前向声明
class Food;
class Medicine;
class Player;
class Game;

class EatEvent {
public:
    /**
     * @brief 构造函数（食物）
     */
    EatEvent(std::shared_ptr<Food> food, std::shared_ptr<Player> player, Game& game);
    
    /**
     * @brief 构造函数（药品）
     */
    EatEvent(std::shared_ptr<Medicine> medicine, std::shared_ptr<Player> player, Game& game);
    
    /**
     * @brief 执行食用事件
     */
    void execute();
    
    /**
     * @brief 获取事件描述
     */
    std::string getDescription() const;

    /**
     * @brief 静态工厂方法（食物）
     */
    static std::shared_ptr<EatEvent> create(std::shared_ptr<Food> food, 
                                           std::shared_ptr<Player> player, 
                                           Game& game);
    
    /**
     * @brief 静态工厂方法（药品）
     */
    static std::shared_ptr<EatEvent> create(std::shared_ptr<Medicine> medicine, 
                                           std::shared_ptr<Player> player, 
                                           Game& game);

private:
    std::shared_ptr<Food> food_;
    std::shared_ptr<Medicine> medicine_;
    std::shared_ptr<Player> player_;
    Game& game_;
    
    // 执行食用逻辑的辅助方法
    void executeFood();

    /*


    void executeMedicine();

    
    */
    
    // 生成属性变化信息
    std::string generateEffectMessage(double healthEffect, double hungerEffect, double energyEffect) const;
};

#endif // EATEVENT_H