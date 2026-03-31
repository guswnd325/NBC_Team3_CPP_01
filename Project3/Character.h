#pragma once
#include <algorithm>
#include "Actor.h"
#include "Inventory.h"
#include "Tools.h"

class Renderer;
class GameManager;

class Character : public Actor
{
private:
	Inventory *inventory;
	int gold, restTicket, level;
	float levelUpExp;

public:

	Character();
	~Character();
	void InitializeCharacter(std::string name);
	Inventory* GetInventory();
	int GetGold();
	void PlayerDead();
	void SetRestTicket(int value);
	int GetRestTicket() const;
	void SetGold(int value);
	void AddGold(int amount);
	void LevelUp();
	float GetLevelUpExp() const;
	int GetLevel() const;
	void EquipGear(int id);
	void UseDice(DiceID id);
};