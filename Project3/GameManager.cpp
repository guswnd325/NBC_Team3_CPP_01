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
		AudioManager::GetInstance().PlayBGM(BGMList::Main_Theme);
		Renderer::GetInstance().RenderMenu(std::vector<std::string>());

		InputResult input = Tools<int>::Input(1, 2);

		switch (input.value)
		{
		case 1:
			Renderer::GetInstance().Clear();
			CreateCharacter();
			Renderer::GetInstance().Clear();

			
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
	Renderer::GetInstance().RenderCreatePlayer(std::vector<std::string>());

	std::string job;

	PlayerExist playerExist = PlayerExist::PlayerExistTrue;

	while ((int)playerExist)
	{
		InputResult input = Tools<int>::Input(1, 4);

		switch (input.value)
		{
		case 1:
			job = "전사";
			break;
		case 2:
			job = "마법사";
			break;
		case 3:
			job = "도박꾼";
			break;
		case 4:
			job = "테스트용";
			break;

		default:
		{
			Renderer::GetInstance().AddSystemLog("잘못된 입력입니다. 제공된 번호로 입력해주세요!");
			Renderer::GetInstance().RenderCreatePlayer(std::vector<std::string>());
			continue;
		}
		}

		if (player != nullptr)
		{
			delete player;
			player = nullptr;
		}

		player = new Character();
		player->InitializeCharacter(job);

		playerExist = PlayerExist::PlayerExistFalse;
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
	Renderer::GetInstance().ClearSystemLogs();

	state = GameState::Running;

	while (state == GameState::Running)
	{
		AudioManager::PlayBGM(BGMList::Village);

		Renderer::GetInstance().RenderMainMenu(std::vector<std::string>());

		InputResult input = Tools<int>::Input(1, 4);

		switch (input.value)
		{
		case 1:
		{
			AudioManager::PlayBGM(BGMList::Exploer);
			AudioManager::PlaySFX(SFXList::FootStep);
			Renderer::GetInstance().ClearSystemLogs();
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
			AudioManager::PlayBGM(BGMList::Shop);
			Renderer::GetInstance().ClearSystemLogs();
			shopManager->Run(player);
			break;
		}
		case 3:
		{
			Renderer::GetInstance().ClearSystemLogs();
			player->GetInventory()->Run();
			break;
		}

		case 4:
			Renderer::GetInstance().ClearSystemLogs();
			AudioManager::PlayBGM(BGMList::Rest);
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
	state = GameState::Running;
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