#include "MonsterManager.h"
#include <ctime>

Monster* MonsterManager::SpawnMonster(std::string areaName) {
    MonsterType selectedType = MonsterType::Slime;

    if (areaName == "Forest") {
        selectedType = (rand() % 10 < 7) ? MonsterType::Slime : MonsterType::Goblin;
    }
    else if (areaName == "Desert") {
        selectedType = MonsterType::Golem; 
    }

    MonsterGrade selectedGrade = MonsterGrade::Normal;
    int roll = rand() % 100;
    if (roll < 20) { 
        selectedGrade = MonsterGrade::Elite;
    }

    return new Monster(selectedType, selectedGrade);
}

Monster* MonsterManager::SpawnBoss(MonsterType type) {

    return new Monster(type, MonsterGrade::Boss);
}