#include "Character.h"

void Character::InitializeCharacter(std::string name)
{
	SetName(name);
}

Inventory & Character::GetInventory()
{
	return inventory;
}

int Character::GetGold()
{
	return gold;
}

void Character::EquipGear(int id)
{
	inventory.Equip(id);
}

void Character::UseDice(int id)
{
	inventory.UseDice(id);
}
