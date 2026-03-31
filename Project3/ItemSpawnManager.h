#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include "BaseItem.h"

class ItemSpawnManager
{
private:
    using Creator = std::function<BaseItem* ()>;
    std::unordered_map<Gears, Creator> itemRegistry;
public:
	BaseItem* MakeItem(Gears type);
    ItemSpawnManager();
};