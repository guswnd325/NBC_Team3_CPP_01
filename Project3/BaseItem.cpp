#include "BaseItem.h"

std::string BaseItem::GetName() const
{
	return name;
}

SlotItems BaseItem::GetType() const
{
	return type;
}

std::string BaseItem::GetTypeToString(SlotItems value)
{
	switch (value)
	{
		case SlotItems::Weapon:
			return "¹«±â";
		case SlotItems::Helmet:
			return "Çï¸ä";
		case SlotItems::BodyArmor:
			return "°©¿Ê";
		case SlotItems::Boots:
			return "½Å¹ß";
		case SlotItems::Accessory:
			return "Àå½Å±¸";
	}
}

int BaseItem::GetPrice() const
{
	return price;
}

int BaseItem::GetID() const
{
	return id;
}

BaseItem::~BaseItem()
{

}


void BaseItem::SetPrice(int value) noexcept
{
	price = value;
}


void BaseItem::Equip(Character * character) 
{
	// StatDelta ½ºÅÈ ÇÃ·¹ÀÌ¾î Àû¿ë
	character->SetAtk(character->GetAtk() + statDelta.atk);
	character->SetDef(character->GetDef() + statDelta.def);
}

void BaseItem::UnEquip(Character* character)
{
	// StatDelta ½ºÅÈ ÇÃ·¹ÀÌ¾î¿¡¼­ »©±â
	character->SetAtk(character->GetAtk() - statDelta.atk);
	character->SetDef(std::max(1, character->GetDef() - statDelta.def));
}

const StatDelta& BaseItem::GetStatDelta() const
{
	return statDelta;
}