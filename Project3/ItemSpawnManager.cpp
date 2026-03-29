#include "ItemSpawnManager.h"
#include "Gears.h"
#include "BaseItem.h"

BaseItem * ItemSpawnManager::MakeItem(int type)
{
	switch (type)
	{
	case (int)Gears::SilverSword:
		return new SilverSword();

	case (int)Gears::GoldHelmet:
		return new GoldHelmet();

	case (int)Gears::GoldBodyArmor:
		return new GoldBodyArmor();

	case (int)Gears::GoldBoots:
		return new GoldBoots();

	case (int)Gears::SilverRing:
		return new SilverRing();
	}

	return nullptr;
}
