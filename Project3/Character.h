#pragma once
#include <algorithm>
#include "Actor.h"
#include "Inventory.h"
#include "Tools.h"

class Character : public Actor
{
private:
	Inventory inventory;
	int gold;
public:
	void InitializeCharacter(std::string name);
	Inventory& GetInventory();
	int GetGold();
	void EquipGear(int id);
	void UseDice(int id);
};