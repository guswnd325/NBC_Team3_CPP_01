#pragma once
#include "Character.h"
#include "Renderer.h"
#include "BattleManager.h"
#include "CombatManager.h"
#include "ShopManager.h"
#include "RestManager.h"

class GameManager
{
private:
	static GameManager* instance;

	Character* player;
	Renderer* renderer;

	CombatManager* combatManager;
	ShopManager* shopManager;
	RestManager* restManager;

private:
	GameManager();
	~GameManager();

public:
	static GameManager* GetInstance();

public:
	void Run();

private:
	void CreateCharacter();
	void StartGame();
};

