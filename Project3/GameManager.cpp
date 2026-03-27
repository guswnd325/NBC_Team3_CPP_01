#include "GameManager.h"
#include <iostream>

// ---------------------------------------------------------------
// 싱글톤
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
	renderer->Clear();
	renderer->RenderCreatePlayer();

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
	while (!player->IsDead())
	{
		renderer->Clear();
		//renderer->RenderStatus(player);
		renderer->RenderMainMenu();

		int input;
		std::cin >> input;

		switch (input)
		{
		case 1: 
		{
			// combatManager 함수 호출
			
			break;
		}
		case 2:
		{
			// shopManager 함수 호출
			
			break;
		}	
		case 3:
		{
			// restManager 함수 호출
			
			break;
		}
		default:
			//renderer->RenderError("잘못된 입력입니다.");
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
	renderer = new Renderer();
	battleManager = new BattleManager(renderer);

	combatManager = new CombatManager();
	shopManager = new ShopManager();
	restManager = new RestManager();
}

GameManager::~GameManager()
{
	delete restManager;
	delete shopManager;
	delete combatManager;
	delete battleManager;
	delete player;

	delete renderer;
}