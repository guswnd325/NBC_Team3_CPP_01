#pragma once
#include "Renderer.h"

class BattleManager
{
private:
    Renderer* renderer;   // 전투 결과 출력용
public:
    BattleManager(Renderer* renderer);

public:
    //void StartBattle(Character* player, Character* enemy);
    //void CalculateDamage(Character* attacker, Character* defender);
    //bool IsOver();
};

