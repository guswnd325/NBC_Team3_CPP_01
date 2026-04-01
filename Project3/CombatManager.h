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

static std::string currentAreaName;

class CombatManager
{
private:

    std::unordered_map<std::string, int> areaUnlockLevel = {
       {"Forest",   1},   // √ ±‚ «ÿ±›
       {"Mine",     1},   // √ ±‚ «ÿ±›  (∆Û±§)
       {"Mountain", 1},   // √ ±‚ «ÿ±›  (ªÍ)
       {"Desert",   5},
       {"Cave",     12}, // ∫∏Ω∫¿¸
       {"Alley",    5},
       {"Jungle",   6},
       {"Temple",   100},
       {"Ocean",    8},
       {"Nest",     11},
    };

    std::vector<std::string> areaName = {
    "Forest", "Desert", "Cave", "Temple", "Jungle", "Mine", "Ocean", "Alley", "Mountain", "Nest"
    };

    MonsterManager* monsterManager;
    BattleManager* battleManager;


    std::vector<std::string> unlockedAreas;
    std::unordered_map<std::string, std::string> areaDisplayname =
    {
        {"Forest", "Ω£"},
        {"Desert", "ªÁ∏∑"},
        {"Cave", "µø±º"},
        {"Temple", "Ω≈¿¸"},
        {"Jungle", "¡§±€"},
        {"Mine", "∆Û±§"},
        {"Ocean", "«ÿ¿˙"},
        {"Alley", "µﬁ∞Ò∏Ò"},
        {"Mountain", "ªÍ"},
        {"Nest", "µ’¡ˆ"}
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
    static std::string GetCurrentAreaName();
    void ReduceHp(Actor* actor, Actor* monster);
    void UnlockAreas(int level, bool flag = false);
    void ShowCredit();

    void EndingCredit();
};