#pragma once
#include <memory>
#include "Gears.h"
#include "ItemSpawnManager.h"
#include "BaseItem.h"
#include "Character.h"
#include "Renderer.h"

enum class BuyStatus
{
    Success,
    Possessed,
    InsufficientGold,
    Default,
};

class ShopManager
{
    std::vector<BaseItem*> itemLists;
public:
    ShopManager();
    const std::vector<BaseItem*>& GetItemLists();
    std::pair<BuyStatus, BaseItem*> BuyItem(int index, Character* character);
    void Run(Character* character);
};