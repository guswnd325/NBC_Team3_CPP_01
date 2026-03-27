#include "GameManager.h"
#include <iostream>

// ---------------------------------------------------------------
// 教臂沛
// ---------------------------------------------------------------
GameManager* GameManager::instance = nullptr;

GameManager* GameManager::GetInstance()
{
	if (instance == nullptr)
		instance = new GameManager();
	return instance;
}

// ---------------------------------------------------------------
// Run Function
// ---------------------------------------------------------------
void GameManager::Run()
{
	while (true)
	{
		renderer->RenderMenu();

		int input;
		std::cin >> input;

		switch (input)
		{
		case 1:
			renderer->Clear();
			StartGame();
			break;
		default:
			break;
		}
	}

}

void GameManager::StartGame()
{
	renderer->RenderCreatePlayer();

	std::string job;
	std::cin >> job;

	player = new Character(job);
}

// ---------------------------------------------------------------
// 积己磊 / 家戈磊
// ---------------------------------------------------------------
GameManager::GameManager()
	: player(nullptr)
{
	renderer = new Renderer();
	battleManager = new BattleManager(renderer);
}

GameManager::~GameManager()
{
	delete battleManager;
	delete player;
	delete renderer;
}