#pragma once
#include "Renderer.h"
#include "Character.h"
#include "Monster.h"



class BattleManager
{
public:
    BattleManager();

public:
    void Run(Character* player, Monster* monster);

private:
    //void StartBattle(Character* player, Character* enemy);
    //void CalculateDamage(Character* attacker, Character* defender);
    //bool IsOver();
};

