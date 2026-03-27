#pragma once
#include "Character.h"
#include "Renderer.h"
#include "BattleManager.h"

class GameManager
{
private:
	static GameManager* instance;

	Character* player;
	Renderer* renderer;
	BattleManager* battleManager;

private:
	GameManager();
	~GameManager();

public:
	static GameManager* GetInstance();

public:
	void Run();

private:
	void StartGame();
};

