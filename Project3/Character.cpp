#include "Character.h"
#include "Renderer.h"

constexpr int defaultGold = 200;
constexpr int defaultHP = 50;
constexpr int defaultRestTicket = 5;

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
	SetHP(defaultHP);
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
