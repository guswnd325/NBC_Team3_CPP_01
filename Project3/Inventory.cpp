#include "Inventory.h"
#include "BaseItem.h"

const std::vector<ItemSlot> &Inventory::GetGearStorege()
{
	return gearStorege;
}

const std::vector<DiceSlot>& Inventory::GetDiceStorege()
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

EquipStatus Inventory::Equip(int index)
{
	// 장비 착용 여부 확인
	// user
	BaseItem* gear = gearStorege[index].item;

	int type = (int)gear->GetType();
	if (nullptr == slots[(int)type])
	{
		slots[(int)type] = gear;
		return EquipStatus::Equip;
	}

	if (slots[(int)type]->GetName() == gear->GetName())
	{
		return EquipStatus::Overlap;
	}
	else // 장비 장착 중이지만, 다른 장비로 교체하는 경우
	{
		slots[(int)type] = gear;
		return EquipStatus::Changed;
	}
}

EquipStatus Inventory::Equip(BaseItem* gear)
{

	// 장비 착용 여부 확인

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
	return -1;
}

int Inventory::GetInventoryDiceIndex(int id)
{
	for (int i = 0; i < diceStorege.size(); i++)
	{
		if (diceStorege[i].dice->GetId() == id)
		{
			return i;
		}
	}
	return -1;
}

bool Inventory::AddGear(BaseItem* gear)
{
	int index = GetInventoryGearIndex(gear->GetID());
	if (index == NOT_FOUND)
	{
		// 없는경우 새로 추가
		gearStorege.push_back(ItemSlot(gear, 1));
	}
	else
	{
		// 이미 있는 장비면 갯수만 추가
		gearStorege[index].count++;
	}

	int type = (int)gear->GetType();
	
	// 장착중인 아이템 없으면 자동 장착
	if (slots[(int)type] == nullptr)
	{
		Equip(gear);
		return true; // 자동 장착된 경우 True 반환
	}

	return false; // 자동 장착된 경우 False 반환
}

void Inventory::AddDice(Dice* dice)
{
	int index = GetInventoryDiceIndex(dice->GetId());
	if (index == NOT_FOUND)
	{
		// 없는경우 새로 추가
		diceStorege.push_back(DiceSlot(dice, 1));
	}
	else
	{
		// 이미 있는 장비면 갯수만 추가
		diceStorege[index].count++;
	}
}

void Inventory::UseDice(int id)
{
	int index = GetInventoryDiceIndex(id);

	int count = diceStorege[index].count;

	if (count >= 1)
	{
		// 주사위 사용
		diceStorege[index].count--;
	}
}

int Inventory::GetDiceCount(int id)
{
	int index = GetInventoryDiceIndex(id);
	return diceStorege[index].count;
}

