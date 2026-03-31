#include "Inventory.h"
#include "BaseItem.h"
#include "Character.h"
#include "Renderer.h"
#include "GameManager.h"

Inventory::Inventory()
{
	for (int i = 0; i < (int)SlotItems::SlotMax; i++)
	{
		slots[i] = nullptr;
	}
}

void Inventory::Run()
{
	while (true)
	{
		Renderer& renderer = Renderer::GetInstance();
		Character * character = GameManager::GetInstance().GetCharacter();
		renderer.RenderInventory(character->GetLevel(), character->GetExp(), character->GetLevelUpExp(), character->GetRestTicket(), slots, gearStorege, diceStorege, std::vector<std::string>());

		InputResult input = Tools<int>::Input(0, (int)gearStorege.size());

		if (input.status == InputStatus::Fail) continue;
		if (input.status == InputStatus::Exit) break;
		if (input.status == InputStatus::IndexOver) continue;

		EquipResult result = EquipByIndex(input.value - 1);
		if (result.status == EquipStatus::Overlap)
		{
			AudioManager::PlaySFX(SFXList::Error);
		}
		else
		{
			AudioManager::PlaySFX(SFXList::Equip);
		}
		renderer.RenderEquipResult(result);
	}
}

std::vector<ItemSlot> &Inventory::GetGearStorege()
{
	return gearStorege;
}

std::vector<DiceSlot>& Inventory::GetDiceStorege()
{
	return diceStorege;
}

void Inventory::Release()
{
	for (int i = 0; i < gearStorege.size(); i++)
	{
		delete gearStorege[i].item;
	}

	for (int i = 0; i < diceStorege.size(); i++)
	{
		delete diceStorege[i].dice;
	}
}

EquipResult Inventory::EquipByIndex(int index)
{
	BaseItem* gear = gearStorege[index].item;

	int type = (int)gear->GetType();

	EquipResult result;

	if (nullptr == slots[(int)type])
	{
		slots[(int)type] = gear;
		result.item = gear;
		result.status = EquipStatus::Equip;
	}

	if (slots[(int)type]->GetName() == gear->GetName())
	{
		result.prevItem = slots[(int)type]->GetName();
		result.item = gear;
		result.status = EquipStatus::Overlap;
	}
	else 
	{
		result.prevItem = slots[(int)type]->GetName();
		slots[(int)type] = gear;
		result.item = gear;
		result.status = EquipStatus::Changed;
	}

	return result;
}

EquipStatus Inventory::EquipByBaseItem(BaseItem* gear)
{

	int type = (int)gear->GetType();
	if (nullptr == slots[(int)type])
	{
		slots[(int)type] = nullptr;
		slots[(int)type] = gear;
		return EquipStatus::Equip;
	}

	if (slots[(int)type]->GetName() == gear->GetName())
	{
		return EquipStatus::Overlap;
	}
	else // 장비 장착 중이지만, 다른 장비로 교체하는 경우
	{
		slots[(int)type] = nullptr;
		slots[(int)type] = gear;
		return EquipStatus::Changed;
	}
}

int Inventory::GetInventoryGearIndex(int id)
{
	for (int i = 0; i < gearStorege.size(); i++)
	{
		if (gearStorege[i].item->GetID() == id) 
		{
			return i;
		}
	}
	return NOT_FOUND;
}

int Inventory::GetInventoryDiceIndex(DiceID id)
{
	for (int i = 0; i < diceStorege.size(); i++)
	{
		if (diceStorege[i].dice->GetId() == id)
		{
			return i;
		}
	}
	return NOT_FOUND;
}

bool Inventory::AddGear(BaseItem* gear)
{
	int index = GetInventoryGearIndex(gear->GetID());
	if (index == NOT_FOUND)
	{
		gearStorege.push_back(ItemSlot(gear, 1));
	}
	else
	{
		gearStorege[index].count++;
	}

	int type = (int)gear->GetType();
	
	if (slots[(int)type] == nullptr)
	{
		EquipByBaseItem(gear);
		return true; // 자동 장착된 경우 True 반환
	}

	return false; // 자동 장착된 경우 False 반환
}

void Inventory::AddDice(DiceID id)
{
	int index = GetInventoryDiceIndex(id);
	if (index == NOT_FOUND)
	{
		diceStorege.push_back(DiceSlot(new Dice(id), 1));
	}
	else
	{
		diceStorege[index].count++;
	}
}

void Inventory::UseDice(DiceID id)
{
	int index = GetInventoryDiceIndex(id);

	int count = diceStorege[index].count;

	if (count >= 1)
	{
		diceStorege[index].count--;
	}
}

int Inventory::GetDiceCount(DiceID id)
{
	int index = GetInventoryDiceIndex(id);
	return diceStorege[index].count;
}
