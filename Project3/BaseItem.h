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
	std::string GetTypeToString(SlotItems value);
	// 기본적으로 스탯 조정을 하지만, 아이템 별로 장착 효과를 달리 구현할 수 있도록 virtual 선언
	virtual void Equip(Character * character);
	virtual void UnEquip(Character * character);
};

class Sword : public BaseItem
{
public:
	Sword() : BaseItem("강동욱 튜터님의 뿅망치", SlotItems::Weapon, StatDelta(10), 200, (int)Gears::Sword) {}
};

class Bow : public BaseItem
{
public:
	Bow() : BaseItem("승리의 검", SlotItems::Weapon, StatDelta(20), 1000, (int)Gears::Bow) {}
};

class Mace : public BaseItem
{
public:
	Mace() : BaseItem("박경호 튜터님의 철퇴", SlotItems::Weapon, StatDelta(30), 2000, (int)Gears::Mace) {}
};

class Helmet : public BaseItem
{
public:
	Helmet() : BaseItem("전진우 매니저님의 헬멧", SlotItems::Helmet, StatDelta(0, 5), 300, (int)Gears::Helmet) {}
};

class Halo : public BaseItem
{
public:
	Halo() : BaseItem("현묵님의 헤일로", SlotItems::Helmet, StatDelta(0, 5), 300, (int)Gears::Halo) {}
};

class PaperArmor : public BaseItem
{
public:
	PaperArmor() : BaseItem("종이 갑옷", SlotItems::BodyArmor, StatDelta(0, 10), 500, (int)Gears::PaperArmor) {}
};

class BallisticPlate : public BaseItem
{
public:
	BallisticPlate() : BaseItem("방탄플레이트", SlotItems::BodyArmor, StatDelta(0, 15), 1000, (int)Gears::BallisticPlate) {}
};

class MilitaryBoots : public BaseItem
{
public:
	MilitaryBoots() : BaseItem("군화", SlotItems::Boots, StatDelta(0, 5), 100, (int)Gears::MilitaryBoots) {}
};

class DragonscalePlateSabatons : public BaseItem
{
public:
	DragonscalePlateSabatons() : BaseItem("용비늘 판금 장화", SlotItems::Boots, StatDelta(0, 10), 800, (int)Gears::DragonscalePlateSabatons) {}
};

class TutorTears : public BaseItem
{
public:
	TutorTears() : BaseItem("진수인 매니저님의 눈물", SlotItems::Accessory, StatDelta(5,4), 1103, (int)Gears::TutorTears) {}
};

class Wing : public BaseItem
{
public:
	Wing() : BaseItem("이선정 매니저님의 날개", SlotItems::Accessory, StatDelta(4, 5), 1004, (int)Gears::Wing) {}
};