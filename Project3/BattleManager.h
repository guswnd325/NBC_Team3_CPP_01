#pragma once
#include "Character.h"
#include "Monster.h"
#include "Renderer.h"
#include "DiceManager.h"


class CombatManager;

constexpr DWORD sleepTime_fast = 800;
constexpr DWORD sleepTime_slow = 1200;

const std::string playerMsgColor = BRIGHT_GREEN;
const std::string monsterMsgColor = BRIGHT_RED;

enum class BattleResult
{
    PlayerWin,
    PlayerDead,
    Escaped,
    PlayerClear
};

class BattleManager
{
public:
    BattleResult Run(Character* player, Monster* monster, CombatManager* combatManage);

private:
    DiceManager diceManager;

    //int RollPlayerDice(Character* player);
    //int RollMonsterDice(Monster* monster);

    void StartBattle(Character* player, Monster* monster);
    bool TryEscape(Character* player, Monster* monster, int& outMonsterRoll);
    void CalculateDamage(Actor* attacker, Actor* defender, int Roll, bool defenderIsMonster);
    bool IsOver(Character* player, Monster* monster);

    void GiveReward(Character* player, Monster* monster);
    void GiveNormalReward(Character* player, Monster* monster);
    void GiveRiskyReward(Character* player, Monster* monster);

    void DrawDiceDirectly(int num);

    void ClearDiceDirectly();
};