#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <string>
#include "../entity/Player.h"

class AbstractItem {
public:
	virtual ~AbstractItem() = default;

	// 获取物品名称
	virtual const std::string& getName() const = 0;
	// 获取物品价格
	virtual int getPrice() const = 0;
	// 使用物品
	virtual void use(Player& user) = 0;
};

#endif // ABSTRACTITEM_H
