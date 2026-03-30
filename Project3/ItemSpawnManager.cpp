#include "ItemSpawnManager.h"
#include "Gears.h"
#include "BaseItem.h"

BaseItem * ItemSpawnManager::MakeItem(Gears type)
{
	switch (type)
	{
	case Gears::Sword:
		return new Sword();
	case Gears::Bow:
		return new Bow();
	case Gears::Mace:
		return new Mace();
	case Gears::Helmet:
		return new Helmet();
	case Gears::Halo:
		return new Halo();
	case Gears::PaperArmor:
		return new PaperArmor();
	case Gears::BallisticPlate:
		return new BallisticPlate();
	case Gears::MilitaryBoots:
		return new MilitaryBoots();
	case Gears::DragonscalePlateSabatons:
		return new DragonscalePlateSabatons();
	case Gears::TutorTears:
		return new TutorTears();
	case Gears::Wing:
		return new Wing();
	}

	return nullptr;
}
