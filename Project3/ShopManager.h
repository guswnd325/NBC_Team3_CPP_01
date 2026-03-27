#pragma once
#include <memory>
#include "Gears.h"
#include "ItemSpawnManager.h"
#include "BaseItem.h"
#include "Character.h"

enum class BuyStatus
{
	Success, // 판매 성공
	Possessed,
	InsufficientGold // 골드 부족해서 못삼 ㅠㅠ
};

class ShopManager
{
private:

public:
	static BuyStatus BuyItem(int index, Character* character);
};

