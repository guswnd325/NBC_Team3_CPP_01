#pragma once
#include <vector>
#include "Monster.h"

class MonsterManager {
public:
	MonsterManager()
	{
		srand(time(NULL));
	}

	Monster* SpawnMonster(std::string areaName);
	Monster* SpawnBoss(MonsterType type);
};