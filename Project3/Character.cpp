#include "Character.h"
#include "Renderer.h"

constexpr int defaultGold = 100;

Character::Character()
{
	inventory = new Inventory();
	gold = defaultGold;
}

Character::~Character()
{
	delete inventory;
}

void Character::InitializeCharacter(std::string name)
{
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

void Character::UseDice(int id)
{
	inventory->UseDice(id);
}
