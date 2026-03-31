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
	while (true)
	{
		Renderer::GetInstance().RenderMenu();

		InputResult input = Tools<int>::Input(1, 2);

		switch (input.value)
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
	Renderer::GetInstance().ClearSystemLogs();
	Renderer::GetInstance().Clear();
	Renderer::GetInstance().RenderCreatePlayer();

	while (true)
	{
		if (player != nullptr)
		{
			delete player;
			player = nullptr;
		}

		std::string job;

		InputResult input = Tools<int>::Input(1, 3);

		switch (input.value)
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
		case 4:
			job = "테스트용";
		default:
		{
			Renderer::GetInstance().AddSystemLog("잘못된 입력입니다. 제공된 번호로 입력해주세요!");
			Sleep(3000);
			continue;
		}
		}

		player = new Character();
		player->InitializeCharacter(job);
	}
	
}

Character* GameManager::GetCharacter() const
{
	return player;
}

void GameManager::SetGameState(GameState state)
{
	this->state = state;
}

void GameManager::StartGame()
{
	while (state == GameState::Running)
	{
		Renderer::GetInstance().Clear();
		Renderer::GetInstance().RenderMainMenu();

		InputResult input = Tools<int>::Input(1, 4);

		switch (input.value)
		{
		case 1:
		{
			BattleResult result = combatManager->Run(player);
			if (BattleResult::PlayerDead == result)
			{
				std::cout << "게임 오버..." << std::endl;
				return; // StartGame() 종료 → 메인화면으로
			}
			if (BattleResult::PlayerClear == result)
			{
				std::cout << "게임 클리어..." << std::endl;
				return;
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
			refurbishManager->Run();

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
	refurbishManager = new RefurbishManager();
}

GameManager::~GameManager()
{
	delete refurbishManager;
	delete shopManager;
	delete combatManager;
	delete monsterManager;
	delete player;
}