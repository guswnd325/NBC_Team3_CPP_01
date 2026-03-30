#include "GameManager.h"
#include "Renderer.h"
#include "BattleManager.h"
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
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

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
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');


	std::string job;

	player = new Character();
	switch (input)
	{
	case 1:
		job = "전사";
		for (int i = 0; i < 3; i++)
		{
			player->GetInventory()->AddDice(DiceID::D1_6);
		}
		break;
	case 2:
		job = "마법사";
		for (int i = 0; i < 2; i++)
		{
			player->GetInventory()->AddDice(DiceID::D2_5);
		}
		break;
	case 3:
		job = "고점맨";
			player->GetInventory()->AddDice(DiceID::D1_8);
		break;
	default:
		break;
	}

	player->InitializeCharacter(job);
}

Character * GameManager::GetCharacter() const
{
	return player;
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
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');


		switch (input)
		{
		case 1:
		{
			BattleResult result = combatManager->Run(player);
			if (result == BattleResult::PlayerDead)
			{
				std::cout << "게임 오버..." << std::endl;
				return; // StartGame() 종료 → 메인화면으로
			}
			break;
		}
		case 2:
		{
			shopManager->Run(player);
			break;
		}
		case 3:
		{
			player->GetInventory()->Run();
			break;
		}
		
		case 4:
			//restManager->Run(player);

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