#include "ItemSpawnManager.h"
#include "Gears.h"
#include "BaseItem.h"

BaseItem * ItemSpawnManager::MakeItem(Gears type)
{
	switch (type)
	{
	case Gears::SilverSword:
		return new SilverSword();
	case Gears::GoldSword:
		return new GoldSword();
	case Gears::GoldHelmet:
		return new GoldHelmet();

	case Gears::GoldBodyArmor:
		return new GoldBodyArmor();

	case Gears::GoldBoots:
		return new GoldBoots();

	case Gears::SilverRing:
		return new SilverRing();
	}

	return nullptr;
}
