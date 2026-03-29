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

class CombatManager
{
private:
    std::vector<std::string> areaName = {
    "Forest", "Desert", "Cave"
    };

    MonsterManager* monsterManager;
    BattleManager* battleManager;

    std::unordered_map<std::string, std::string> areaDisplayname =
    {
        {"Forest", " ½£ "},
        {"Desert", " »ç¸·"},
        {"Cave", " µ¿±¼"},
        {"Temple", " ½ÅÀü"},
        {"Jungle", " Á¤±Û"},
        {"Mine", " Æó±¤"},
        {"Ocean", " ÇØÀú"},
        {"Alley", "µŞ°ñ¸ñ"},
        {"Mountain", " »ê "},
        {"Nest", " µÕÁö"}
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

    void Run(Character* player);
    void ReduceHp(Actor* actor, Actor* monster);
};