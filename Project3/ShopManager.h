#pragma once
#include <memory>
#include "Gears.h"
#include "ItemSpawnManager.h"
#include "BaseItem.h"
#include "Character.h"

enum class BuyStatus
{
    Success,
    Possessed,
    InsufficientGold,
    Default,
};

class ShopManager
{
public:
    ShopManager();
    std::pair<BuyStatus, BaseItem*> BuyItem(int index, Character* character);
    void Run(Character* character);
};