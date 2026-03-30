#include "Character.h"
#include "Renderer.h"

constexpr int defaultGold = 200;
constexpr int maxHP = 100;
constexpr int defaultRestTicket = 0;

Character::Character()
{
	inventory = new Inventory();
	gold = defaultGold;
	restTicket = defaultRestTicket;
}

Character::~Character()
{
	delete inventory;
}

void Character::InitializeCharacter(std::string name)
{
	/*
	전사 : (1~6) 주사위 3개, HP=100, ATK=5, DEF=10
마법사 : (1~10) 주사위 2개, HP=100, ATK=10, DEF=5
광대 : (1~24) 주사위 1개, HP=100, ATK=1, DEF=0
	*/

	SetHP(maxHP);
	if (name == "전사")
	{
		SetAtk(5);
		SetDef(10);
	}
	else if (name == "마법사")
	{
		SetAtk(10);
		SetDef(5);
	}
	else if (name == "고점맨")
	{
		SetAtk(1);
		SetDef(0);
	}

	SetHP(maxHP);
	SetName(name);
}

Inventory * Character::GetInventory()
{
	return inventory;
}

int Character::GetGold()
{
	return gold;
}

void Character::SetGold(int value)
{
	gold = value;
}

void Character::AddGold(int amount)
{
	gold += amount;
}

void Character::EquipGear(int id)
{
	inventory->EquipByIndex(id);
}

void Character::UseDice(DiceID id)
{
	inventory->UseDice(id);
}

void Character::SetRestTicket(int value)
{
	restTicket = value;
}

int Character::GetRestTicket() const
{
	return restTicket;
}
