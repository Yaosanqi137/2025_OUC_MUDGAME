#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <string>
#include "../entity/Player.h"
#include <cassert>             // 用于调试时的断言检查

class AbstractItem {
public:
    // 构造函数：支持默认初始化和带名称/描述的初始化
    AbstractItem() : amount_(0), name_(""), intro_(""), isUsable_(true) {}
    AbstractItem(const std::string& name, const std::string& intro) 
        : amount_(0), name_(name), intro_(intro), isUsable_(true) {
        assert(!name.empty() && "物品名称不能为空");  // 保留调试断言，确保名称有效
    }
    
    // 虚析构函数：确保派生类析构函数能正确调用
    virtual ~AbstractItem() = default;

    // 获取物品名称：直接返回成员变量
    const std::string& getName() const { return name_; }
    
    // 获取物品描述：保留非虚函数，因为不需要派生类重写
    const std::string& getIntro() const { return intro_; }
    
    // 获取物品价格：纯虚函数，强制派生类实现价格逻辑
    virtual int getPrice() const = 0;
    
    // 使用物品：修改为返回bool类型，同时保留纯虚函数特性
    virtual bool use(Player& user) = 0;

    // 物品数量管理：恢复合法性检查
    [[nodiscard]] int getAmount() const { return amount_; }
    void setAmount(int amount) { 
        assert(amount >= 0 && "物品数量不能为负数");  // 恢复断言检查
        amount_ = amount; 
    }
    void addAmount(int amountToAdd) { 
        setAmount(amount_ + amountToAdd);  // 复用setAmount的合法性检查
    }
    
    // 物品可用性判断
    bool isUsable() const { return isUsable_; }

protected:
    int amount_;         // 物品数量（支持堆叠）
    std::string name_;   // 物品名称
    std::string intro_;  // 物品描述
    bool isUsable_;      // 物品可用性
};

#endif // ABSTRACTITEM_H
