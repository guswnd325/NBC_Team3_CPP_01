
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
		
		renderer.RenderShopItemList(itemLists);

		int buyItemIndex;

		std::cin >> buyItemIndex;
		
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			continue;
		}
		else if (!buyItemIndex)
		{
			break;
		}
		if (buyItemIndex >= itemLists.size())
		{
			continue;
		}
		
		std::pair<BuyStatus, BaseItem *> status = BuyItem(buyItemIndex, character);
		
		renderer.RenderBuyResult(status.first, status.second, character->GetGold());
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


