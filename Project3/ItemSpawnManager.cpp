#include "ItemSpawnManager.h"
#include "Gears.h"
#include "BaseItem.h"

BaseItem * ItemSpawnManager::MakeItem(int type)
{
	switch (type)
	{
	case (int)Gears::SliverSword:
		return new SliverSword();

	case (int)Gears::GoldHelmet:
		return new GoldHelmet();

	case (int)Gears::GoldBodyArmor:
		return new GoldBodyArmor();

	case (int)Gears::GoldBoots:
		return new GoldBoots();

	case (int)Gears::SliverRing:
		return new SliverRing();
	}

	return nullptr;
}
