#pragma once
#include <memory>
#include "Gears.h"
#include "ItemSpawnManager.h"
#include "BaseItem.h"
#include "Character.h"
#include "Renderer.h"

enum class BuyStatus
{
	Success, // 구매 성공
	Possessed,
	InsufficientGold, // 골드 부족해서 못삼 ㅠㅠ
	Default,
};

class ShopManager
{
public:
	ShopManager();
	std::pair<BuyStatus, BaseItem*> BuyItem(int index, Character* character);
	void Run(Character* character);
};

