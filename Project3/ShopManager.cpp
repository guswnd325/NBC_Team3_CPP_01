
#include "ShopManager.h"


ShopManager::ShopManager()
{
	for (int i = 1; i < (int)Gears::SlotMax; i++)
	{
		itemLists.push_back(ItemSpawnManager::MakeItem(Gears(i)));
	}
}

const std::vector<BaseItem*>& ShopManager::GetItemLists()
{
	return itemLists;
}

std::streamsize ShopManager::GetMaxLengthByVector(std::vector<BaseItem*>& v)
{
	std::streamsize size = 0;
	for (auto x : v)
	{
		size = max(size, v.size());
	}
	return size;
}
void ShopManager::Run(Character* character)
{
	while (true)
	{

		Renderer& renderer = Renderer::GetInstance();
		renderer.Clear();
		//renderer.ShowShopItems();
		
		// void Renderer::ShowShopItems()
		{
			std::cout << "[0] 메뉴로 돌아가기" << std::endl << std::endl;

			std::cout << "== 상점 판매 아이템 리스트 ==" << std::endl;
			for (int i = 0; i < itemLists.size(); i++)
			{
				std::cout << "[" + std::to_string(i + 1) + "] " + itemLists[i]->GetName() + " | " + itemLists[i]->GetTypeToString(itemLists[i]->GetType()) + " | " + std::to_string(itemLists[i]->GetPrice()) + "G" << std::endl;
			}

			std::cout << "구매할 아이템 번호를 입력 : ";
		}

		int buyItemIndex;

		std::cin >> buyItemIndex;
		std::string message = "";

		// 숫자 이외의 입력 및 0은 종료처리
		
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			message = "잘못된 입력입니다.";
			// renderer.RenderMessage(message);
			// renderer.Delay(4);
			continue;
		}
		else if (!buyItemIndex)
		{
			// 메뉴로 돌아가기
			break;
		}
		if (buyItemIndex >= itemLists.size())
		{
			std::cout << "Index Error" << std::endl;
			Sleep(2000);
			continue;
		}
		
		std::pair<BuyStatus, BaseItem *> status = BuyItem(buyItemIndex, character);
		
		renderer.Clear();

		switch (status.first)
		{
			case BuyStatus::Success:
			{
				std::string itemName = status.second->GetName();
				
				character->GetInventory()->EquipByBaseItem(status.second);
				message = "[" + itemName + "]" + "을/를 구매하였습니다.";
			}
			break;
			case BuyStatus::InsufficientGold:
			{
				std::string dist = std::to_string(abs(character->GetGold() - status.second->GetPrice()));
				message = "골드가 [" + dist + "]" + "만큼 부족합니다.";
				
				delete status.second;
			}
			break;
			case BuyStatus::Possessed:
			{
				std::string itemName = status.second->GetName();
				message = "[" + itemName + "] " + "아이템은 이미 보유중입니다.";
				
				delete status.second;
				break;
			}
		}
		// 처리 결과 출력
		

		{
			std::cout << message << std::endl;
			Sleep(2000);
		}
		// renderer.RenderMessage(message);
		// renderer.Delay(4);


	}
}

bool ShopManager::CheckPossessd(int itemID, Character * character)
{
	std::vector<ItemSlot> gearStorege = character->GetInventory()->GetGearStorege();
	for (int i = 0; i < gearStorege.size(); i++)
	{
		if (gearStorege[i].item->GetID() == itemID)
		{
			return true;
		}
	}
	return false;
}

std::pair<BuyStatus, BaseItem *> ShopManager::BuyItem(int index, Character * character)
{
	// 구매하고자 하는 아이템
	BaseItem* item = ItemSpawnManager::MakeItem(Gears(index));

	int itemPrice = item->GetPrice();

	// 구매 성공
	int playerGold = character->GetGold();

	if (CheckPossessd(item->GetID(), character))
	{
		return { BuyStatus::Possessed, item };
	}
	else if (itemPrice <= playerGold)
	{
		character->GetInventory()->AddGear(item);
		character->SetGold(character->GetGold() - itemPrice);
		return {BuyStatus::Success, item };
	}
	// 구매 실패(골드 부족)
	else if (itemPrice > playerGold)
	{
		return { BuyStatus::InsufficientGold,  item };
	}
	return { BuyStatus::Default, nullptr };
	
}


