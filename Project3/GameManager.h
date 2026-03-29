#pragma once
#include "Character.h"
#include "Renderer.h"

#include "MonsterManager.h"
#include "CombatManager.h"
#include "ShopManager.h"
#include "RestManager.h"

class GameManager
{
private:
	static GameManager* instance;

	Character* player;
	Renderer* renderer;

	MonsterManager* monsterManager;
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

