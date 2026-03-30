#pragma once
#include "Character.h"
#include "Monster.h"
#include "Renderer.h"
#include "DiceManager.h"

enum class BattleResult
{
    PlayerWin,
    PlayerDead,
    Escaped,
};

class BattleManager
{
public:
    BattleResult Run(Character* player, Monster* monster);

private:
    DiceManager diceManager;

    //int RollPlayerDice(Character* player);
    //int RollMonsterDice(Monster* monster);

    void StartBattle(Character* player, Monster* monster);
    bool TryEscape(Character* player, Monster* monster);
    void CalculateDamage(Actor* attacker, Actor* defender);
    bool IsOver(Character* player, Monster* monster);

    void GiveReward(Character* player, Monster* monster);
    void GiveNormalReward(Character* player, Monster* monster);
    void GiveRiskyReward(Character* player, Monster* monster);
};