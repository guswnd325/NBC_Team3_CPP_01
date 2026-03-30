#include "ShopManager.h"


ShopManager::ShopManager()
{

}

void ShopManager::Run(Character* character)
{
	while (true)
	{
		Renderer& renderer = Renderer::GetInstance();
		//renderer.ShowShopItems();
		// [0] 메뉴로 돌아가기
		 
		// == 상점 아이템 리스트 ==
		// [1] 강동욱 튜터님의 눈물
		// [2] 

		
		int buyItemIndex;

		std::cin >> buyItemIndex;
		std::string message = "";

		// 숫자 이외의 입력 및 0은 종료처리
		if (!buyItemIndex)
		{
			// 메뉴로 돌아가기
			break;
		}
		else if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			message = "잘못된 입력입니다.";
			// renderer.RenderMessage(message);
			// renderer.Delay(4);
			continue;
		}

		// 상점 -> 판매 아이템 리스트 출력해줌 -> 인덱스 입력하라고 함

		std::pair<BuyStatus, BaseItem *> status = BuyItem(buyItemIndex, character);

		switch (status.first)
		{
			case BuyStatus::Success:
			{
				std::string itemName = status.second->GetName();
				message = "[" + itemName + "]" + "을/를 구매하였습니다.";
			}
			break;
			case BuyStatus::InsufficientGold:
			{
				std::string dist = std::to_string(abs(character->GetGold() - status.second->GetPrice()));
				message = "골드가 [" + dist + "]" + "만큼 부족합니다.";
			}
			break;
		}
		// 처리 결과 출력
		// renderer.RenderMessage(message);
		// renderer.Delay(4);


	}
}

std::pair<BuyStatus, BaseItem *> ShopManager::BuyItem(int index, Character * character)
{
	// 구매하고자 하는 아이템
	BaseItem* item = ItemSpawnManager::MakeItem(index);

	int itemPrice = item->GetPrice();

	// 구매 성공
	int playerGold = character->GetGold();

	if (itemPrice <= playerGold)
	{
		character->GetInventory()->AddGear(item);
		return {BuyStatus::Success, item };
	}
	// 구매 실패(골드 부족)
	else// if (itemPrice > playerGold)
	{
		return { BuyStatus::InsufficientGold,  item };
	}

	//
	//return { BuyStatus::Default, "Dummy" };
}


