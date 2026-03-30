#pragma once
#include <memory>
#include "BaseItem.h"

class ItemSpawnManager
{
public:
	static BaseItem* MakeItem(Gears type);
};