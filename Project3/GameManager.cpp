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
			combatManager->Run(player);
			break;
		}
		case 2:
		{
			// renderer->RenderShop();

			int input;
			std::cin >> input;

			// 유효 범위 체크
			if (input < 1 || input >(int)Gears::SlotMax)
			{
				// renderer->RenderError("잘못된 입력입니다.");
				break;
			}

			// 사용자 입력 1~5 → Gears enum 0~4로 변환
			BuyStatus status = shopManager->BuyItem(input - 1, player);

			switch (status)
			{
			case BuyStatus::Success:
				// renderer->RenderMessage("구매 성공!");
				break;
			case BuyStatus::InsufficientGold:
				// renderer->RenderMessage("골드 부족!");
				break;
			case BuyStatus::Possessed:
				// renderer->RenderMessage("이미 보유 중!");
				break;
			}
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

	// 추후 각 클래스에서 Render 기능을 사용하기 위해 인자로 renderer를 보내줘야 함.
	combatManager = new CombatManager(renderer);
	shopManager = new ShopManager();
	restManager = new RestManager();
}

GameManager::~GameManager()
{
	delete restManager;
	delete shopManager;
	delete combatManager;
	delete player;

	delete renderer;
}