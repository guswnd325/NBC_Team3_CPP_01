#pragma once
#include <memory>
#include <Windows.h>
#include "Gears.h"
#include "ItemSpawnManager.h"
#include "BaseItem.h"
#include "Character.h"
#include "Renderer.h"
#include "buyStatus.h"

class ShopManager
{
private:
    std::vector<BaseItem*> itemLists;
    ItemSpawnManager *itemSpawnner;
public:
    ShopManager();
    ~ShopManager();
    const std::vector<BaseItem*>& GetItemLists();
    bool CheckPossessd(int itemID, Character * character);
    std::pair<BuyStatus, BaseItem*> BuyItem(int index, Character* character);
    void Run(Character* character);
};