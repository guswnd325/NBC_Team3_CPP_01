#pragma once
#include <vector>
#include "Monster.h"

class MonsterManager {
public:
	Monster* SpawnMonster(std::string areaName);
	Monster* SpawnBoss(MonsterType type);
};