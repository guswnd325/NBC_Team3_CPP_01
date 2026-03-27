#pragma once
#include <string>
#include "StatDelta.h"
#include "SlotItems.h"
#include "Gears.h"
#include "Character.h"

class BaseItem
{
private:
	std::string name;
	int price;
	int id;
	StatDelta statDelta; // 아이템으로 인해 변화되는 수치 구조체
	SlotItems type; // item Index
	
public:
	BaseItem(std::string name, SlotItems type, StatDelta statDelta, int price) : name(name), type(type), statDelta(statDelta), price(price) {}
	virtual ~BaseItem();
	
	const StatDelta& GetStatDelta() const;
	std::string GetName() const;
	SlotItems GetType() const;
	int GetID() const;
	
	int GetPrice() const;
	void SetPrice(int value) noexcept;

	// 기본적으로 스탯 조정을 하지만, 아이템 별로 장착 효과를 달리 구현할 수 있도록 virtual 선언
	virtual void Equip(Character * character);
	virtual void UnEquip(Character * character);
};

class SliverSword : public BaseItem
{
public:
	SliverSword() : BaseItem("SliverSword", SlotItems::Weapon, StatDelta(), 100) {}
};


class GoldHelmet : public BaseItem
{
public:
	GoldHelmet() : BaseItem("GoldHelmet", SlotItems::Helmet, StatDelta(), 100) {}
};


class GoldBodyArmor : public BaseItem
{
public:
	GoldBodyArmor() : BaseItem("GoldBodyArmor", SlotItems::BodyArmor, StatDelta(), 100) {}
};


class GoldBoots : public BaseItem
{
public:
	GoldBoots() : BaseItem("GoldBoots", SlotItems::Helmet, StatDelta(), 100) {}

};

class SliverRing : public BaseItem
{
public:
	SliverRing() : BaseItem("SliverRing", SlotItems::Accessory, StatDelta(), 100) {}
};
