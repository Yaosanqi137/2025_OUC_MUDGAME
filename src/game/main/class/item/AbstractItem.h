// file: AbstractItem.h

#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <string>

class Player;

class AbstractItem {
public:
	AbstractItem() : amount_(0), name_(""), intro_(""), isUsable_(true) {}
	AbstractItem(const std::string& name, const std::string& intro) : amount_(0), name_(name), intro_(intro), isUsable_(true) {}
	virtual ~AbstractItem() = default;

	// 获取物品名称 - 现在直接返回成员变量
	const std::string& getName() const { return name_; }
	// 获取物品描述 - 现在直接返回成员变量
	const std::string& getIntro() const { return intro_; }
	// 获取物品价格
	virtual int getPrice() const = 0;
	// 使用物品
	virtual void use(Player& user) = 0;

	[[nodiscard]] int getAmount() const { return amount_; }
	void setAmount(int amount) { amount_ = amount; }
	void addAmount(int amount_to_add) { amount_ += amount_to_add; }
	bool isUsable() const { return isUsable_; }

protected:
	int amount_;
	std::string name_;
	std::string intro_;
	bool isUsable_;
};

#endif // ABSTRACTITEM_H