#include "ShopManager.h"

ShopManager::ShopManager()
{
	itemSpawnner = new ItemSpawnManager();
	for (int i = 1; i < (int)Gears::SlotMax; i++)
	{
		itemLists.push_back(itemSpawnner->MakeItem(Gears(i)));
	}
}

ShopManager::~ShopManager()
{
	delete itemSpawnner;
}

const std::vector<BaseItem*>& ShopManager::GetItemLists()
{
	return itemLists;
}
void ShopManager::Run(Character* character)
{
	while (true)
	{
		Renderer& renderer = Renderer::GetInstance();
		renderer.Clear();
		
		renderer.RenderShopItemList(itemLists, character->GetGold(), std::vector<std::string>());

		InputResult input = Tools<int>::Input(0, (int)itemLists.size());

		if (input.status == InputStatus::Fail) continue;
		if (input.status == InputStatus::Exit) break;
		if (input.status == InputStatus::IndexOver) continue;

		std::pair<BuyStatus, BaseItem *> status = BuyItem(input.value, character);
		
		if (status.first == BuyStatus::Success)
		{
			AudioManager::PlaySFX(SFXList::Buy_Item);
		}
		else
		{
			AudioManager::PlaySFX(SFXList::Error);
		}
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
	BaseItem* item = itemSpawnner->MakeItem(Gears(index));

	int itemPrice = item->GetPrice();

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
	else if (itemPrice > playerGold)
	{
		return { BuyStatus::InsufficientGold,  item };
	}
	return { BuyStatus::Default, nullptr };
	
}


