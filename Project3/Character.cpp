#include "Character.h"
#include "Renderer.h"
#include "GameManager.h"

constexpr int defaultGold = 0;
constexpr int maxHP = 100;
constexpr int defaultHP = 100;
constexpr int defaultRestTicket = 0;

Character::Character()
{
	level = 1;
	levelUpExp = 60;
	inventory = new Inventory();
	gold = defaultGold;
	restTicket = defaultRestTicket;
}

Character::~Character()
{
	delete inventory;
}

void Character::PlayerDead()
{
	GameManager::GetInstance().SetGameState(GameState::Stoped);
}

void Character::InitializeCharacter(std::string name)
{
	/*
	전사 : (1~6) 주사위 3개, HP=100, ATK=5, DEF=10
마법사 : (1~10) 주사위 2개, HP=100, ATK=10, DEF=5
광대 : (1~24) 주사위 1개, HP=100, ATK=1, DEF=0
	*/

	SetHP(defaultHP);
	SetName(name);

	if (name == "전사")
	{
		SetAtk(5);
		SetDef(5);
		for (int i = 0; i < 3; i++)
		{
			GetInventory()->AddDice(DiceID::D1_6);
		}
	}
	else if (name == "마법사")
	{
		SetAtk(10);
		SetDef(2);
		for (int i = 0; i < 2; i++)
		{
			GetInventory()->AddDice(DiceID::D1_10);
		}
	}
	else if (name == "도박꾼")
	{
		SetAtk(1);
		SetDef(0);

		GetInventory()->AddDice(DiceID::D1_24);
	}
	else if (name == "테스트용")
	{
		SetAtk(99999);
		SetDef(99999);
		restTicket = 99999;
		SetGold(99999);
		for (int i = 0; i < 10; i++)
		{
			GetInventory()->AddDice(DiceID::D5_24);
		}
	}

}

Inventory * Character::GetInventory()
{
	return inventory;
}

int Character::GetGold()
{
	return gold;
}

void Character::SetGold(int value)
{
	gold = value;
}

void Character::AddGold(int amount)
{
	gold += amount;
}

void Character::EquipGear(int id)
{
	inventory->EquipByIndex(id);
}

void Character::UseDice(DiceID id)
{
	inventory->UseDice(id);
}

void Character::SetRestTicket(int value)
{
	restTicket = value;
}

int Character::GetRestTicket() const
{
	return restTicket;
}

void Character::LevelUp()
{
	AudioManager::PlaySFX(SFXList::LevelUp);
	
	while (true)
	{
		if (GetExp() > (int)levelUpExp)
		{
			SetExp(GetExp() - (int)levelUpExp);
			level++;
			levelUpExp *= 1.5;
		}
		else break;
	}

}

float Character::GetLevelUpExp() const
{
	return levelUpExp;
}

int Character::GetLevel() const
{
	return level;
}
