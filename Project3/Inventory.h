#pragma once
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include "SlotItems.h"
#include "DiceManager.h"
#define NOT_FOUND -1

class BaseItem;

enum class EquipStatus
{
	Equip, // 문제없이 장비된 경우 반환
	Overlap, // 같은 장비를 착용하길 시도하는 경우 반환
	Changed	// 장비가 교체되었을 때 반환
};

struct ItemSlot 
{
	BaseItem* item;
	int count;	
	ItemSlot(BaseItem* item, int count) : item(item), count(count) {}
};

struct DiceSlot {
public:
	Dice* dice;
	int count;

	DiceSlot(Dice* dice, int count) : dice(dice), count(count) {}
};

class Inventory
{
private:

	// 슬롯(장비)
	BaseItem * slots[(int)SlotItems::SlotMax];
	
	// 저장소(장비) 
	std::vector<ItemSlot> gearStorege;
	

	// 저장소(주사위)
	std::vector<DiceSlot> diceStorege;

public:
	// 메모리 해제
	void Release();

	const std::vector<ItemSlot> &GetGearStorege();
	const std::vector<DiceSlot> &GetDiceStorege();
	
	// 장비 착용
	EquipStatus Equip(int index);
	EquipStatus Equip(BaseItem * gear);

	void AddDice(Dice* dice);
	void UseDice(int id);
	int GetDiceCount(int id);
	bool AddGear(BaseItem* gear);

	int GetInventoryGearIndex(int id);
	int GetInventoryDiceIndex(int id);
	
};

