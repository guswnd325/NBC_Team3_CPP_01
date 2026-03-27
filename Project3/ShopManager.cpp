#include "ShopManager.h"

BuyStatus ShopManager::BuyItem(int index, Character * character)
{
	// 구매하고자 하는 아이템
	BaseItem* item = ItemSpawnManager::MakeItem(index);

	int itemPrice = item->GetPrice();

	// 구매 성공
	int playerGold = character->GetGold();

	if (itemPrice <= playerGold)
	{
		character->GetInventory().AddGear(item);
		return BuyStatus::Success;
	}
	// 구매 실패(골드 부족)
	else if (itemPrice > playerGold)
	{
		return BuyStatus::InsufficientGold;
	}

}
