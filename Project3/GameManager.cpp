#include "GameManager.h"
#include "Renderer.h"
#include <iostream>

// ---------------------------------------------------------------
// 싱글톤
// ---------------------------------------------------------------
GameManager& GameManager::GetInstance()
{
	static GameManager instance;
	return instance;
}

// ---------------------------------------------------------------
// Run Function
// ---------------------------------------------------------------
void GameManager::Run()
{
	Renderer& renderer = Renderer::GetInstance();

	while (true)
	{
		renderer.RenderMenu();

		int input;
		std::cin >> input;

		switch (input)
		{
		case 1:
			CreateCharacter();
			StartGame();
			break;
		case 2:
			return;
		default:
			break;
		}
	}
}

void GameManager::CreateCharacter()
{
	Renderer& renderer = Renderer::GetInstance();

	renderer.Clear();
	renderer.RenderCreatePlayer();

	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	int input;
	std::cin >> input;

	std::string job;

	switch (input)
	{
	case 1:
		job = "전사";
		break;
	case 2:
		job = "마법사";
		break;
	case 3:
		job = "고점맨";
		break;
	default:
		break;
	}

	player = new Character();
	player->InitializeCharacter(job);
}

void GameManager::StartGame()
{
	Renderer& renderer = Renderer::GetInstance();

	while (!player->IsDead())
	{
		renderer.Clear();
		renderer.RenderMainMenu();

		int input;
		std::cin >> input;

		switch (input)
		{
		case 1:
			combatManager->Run(player);
			break;
		case 2:
			shopManager->Run(player);
			break;
		case 3:
			// restManager 함수 호출
			break;
		default:
			break;
		}
	}
}

// ---------------------------------------------------------------
// 생성자 / 소멸자
// ---------------------------------------------------------------
GameManager::GameManager()
	: player(nullptr)
{
	monsterManager = new MonsterManager();
	combatManager = new CombatManager(monsterManager);
	shopManager = new ShopManager();
	restManager = new RestManager();
}

GameManager::~GameManager()
{
	delete restManager;
	delete shopManager;
	delete combatManager;
	delete monsterManager;
	delete player;
}