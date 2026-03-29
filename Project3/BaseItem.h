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
	StatDelta statDelta; // 아이템으로 인해 변화되는 수치를 담는 구조체
	SlotItems type; // item Index
	
public:
	BaseItem(std::string name, SlotItems type, StatDelta statDelta, int price, int id) : name(name), type(type), statDelta(statDelta), price(price), id(id) {}
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

class SilverSword : public BaseItem
{
public:
	SilverSword() : BaseItem("SilverSword", SlotItems::Weapon, {}, 100, (int)Gears::SilverSword) {}
};


class GoldHelmet : public BaseItem
{
public:
	GoldHelmet() : BaseItem("GoldHelmet", SlotItems::Helmet, StatDelta(), 100, (int)Gears::GoldHelmet) {}
};


class GoldBodyArmor : public BaseItem
{
public:
	GoldBodyArmor() : BaseItem("GoldBodyArmor", SlotItems::BodyArmor, StatDelta(), 100, (int)Gears::GoldBodyArmor) {}
};


class GoldBoots : public BaseItem
{
public:
	GoldBoots() : BaseItem("GoldBoots", SlotItems::Helmet, StatDelta(), 100, (int)Gears::GoldBoots) {}

};

class SilverRing : public BaseItem
{
public:
	SilverRing() : BaseItem("SilverRing", SlotItems::Accessory, StatDelta(), 100, (int)Gears::SilverRing) {}
};
