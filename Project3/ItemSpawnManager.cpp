#include "ItemSpawnManager.h"
#include "Gears.h"
#include "BaseItem.h"

ItemSpawnManager::ItemSpawnManager()
{
    itemRegistry[Gears::Sword] = []() { return new Sword(); };
    itemRegistry[Gears::Bow] = []() { return new Bow(); };
    itemRegistry[Gears::Mace] = []() { return new Mace(); };
    itemRegistry[Gears::Helmet] = []() { return new Helmet(); };
    itemRegistry[Gears::Halo] = []() { return new Halo(); };
    itemRegistry[Gears::PaperArmor] = []() { return new PaperArmor(); };
    itemRegistry[Gears::BallisticPlate] = []() { return new BallisticPlate(); };
    itemRegistry[Gears::MilitaryBoots] = []() { return new MilitaryBoots(); };
    itemRegistry[Gears::DragonscalePlateSabatons] = []() { return new DragonscalePlateSabatons(); };
    itemRegistry[Gears::TutorTears] = []() { return new TutorTears(); };
    itemRegistry[Gears::Wing] = []() { return new Wing(); };
}

BaseItem * ItemSpawnManager::MakeItem(Gears type)
{
    auto it = itemRegistry.find(type);
    if (it != itemRegistry.end()) {
        return it->second();
    }
    return nullptr;
}
