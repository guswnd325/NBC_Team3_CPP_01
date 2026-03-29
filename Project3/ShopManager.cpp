#include "ShopManager.h"


ShopManager::ShopManager()
{

}

void ShopManager::Run(Character* character)
{
	while (true)
	{
		//Renderer::GetInstance().ShowShop();
		
		int buyItemIndex;

		std::cin >> buyItemIndex;

		bool fail = std::cin.fail();

		// 숫자 이외의 입력 및 0은 종료처리
		if (!buyItemIndex || fail)
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}

		// 상점 -> 판매 아이템 리스트 출력해줌 -> 인덱스 입력하라고 함

		std::pair<BuyStatus, BaseItem *> status = BuyItem(buyItemIndex, character);

		std::string message = "";

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
		// Renderer::GetInstance().RenderMessage(message);
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


