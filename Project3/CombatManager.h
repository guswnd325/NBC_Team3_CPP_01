#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <unordered_map>
#include "Monster.h"
#include "MonsterManager.h"
#include "BattleManager.h"
#include "Actor.h"

class Renderer;

class CombatManager
{
private:

    std::unordered_map<std::string, int> areaUnlockLevel = {
       {"Forest",   1},   // √ ±‚ «ÿ±›
       {"Mine",     1},   // √ ±‚ «ÿ±›  (∆Û±§)
       {"Mountain", 1},   // √ ±‚ «ÿ±›  (ªÍ)
       {"Desert",   2},
       {"Cave",     8},
       {"Alley",    12},
       {"Jungle",   16},
       {"Temple",   20},
       {"Ocean",    25},
       {"Nest",     30},
    };

    std::vector<std::string> areaName = {
    "Forest", "Desert", "Cave", "Temple", "Jungle", "Mine", "Ocean", "Alley", "Mountain", "Nest"
    };

    MonsterManager* monsterManager;
    BattleManager* battleManager;


    std::vector<std::string> unlockedAreas;
    std::unordered_map<std::string, std::string> areaDisplayname =
    {
        {"Forest", " Ω£ "},
        {"Desert", " ªÁ∏∑"},
        {"Cave", " µø±º"},
        {"Temple", " Ω≈¿¸"},
        {"Jungle", " ¡§±€"},
        {"Mine", " ∆Û±§"},
        {"Ocean", " «ÿ¿˙"},
        {"Alley", "µﬁ∞Ò∏Ò"},
        {"Mountain", " ªÍ "},
        {"Nest", " µ’¡ˆ"}
    };

    std::vector<std::string> currentChoices;

public:
    CombatManager(MonsterManager* m) : monsterManager(m) {
        battleManager = new BattleManager();
    };

    ~CombatManager() { delete battleManager; }

    void GenerateAreaChoices();
    void DisplayChoices();
    std::string SelectArea();

    BattleResult Run(Character* player);
    void ReduceHp(Actor* actor, Actor* monster);
    void UnlockAreas(int level);
};