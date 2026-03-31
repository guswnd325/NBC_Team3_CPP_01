#include "BattleManager.h"
#include <iostream>
#include <limits>
#include <string>

// ---------------------------------------------------------------
// Run - 전투 전체 흐름
// ---------------------------------------------------------------
BattleResult BattleManager::Run(Character* player, Monster* monster)
{
    while (true)
    {
        Renderer::GetInstance().RenderBattleAction(monster, player);

        InputResult input = Tools<int>::Input(1, 2);

        std::cout << std::endl;

        if (1 == input.value)
        {
            StartBattle(player, monster);
        }
        else if (2 == input.value)
        {
            int monsterRoll = 0;

            if (TryEscape(player, monster, monsterRoll))
            {
                Renderer::GetInstance().AddBattleLog("도망에 성공했습니다!");
                Sleep(3000);
                return BattleResult::Escaped;
            }
            else
            {
                Renderer::GetInstance().AddBattleLog("도망에 실패! " + monster->GetName() + "이(가) 공격합니다!");
                CalculateDamage(monster, player, monsterRoll);
            }
        }
        else
        {
            Renderer::GetInstance().AddBattleLog("잘못된 입력입니다. 1 또는 2를 입력해주세요.");
            Sleep(3000);
            continue;
        }

        // 전투 종료 여부 체크
        if (player->IsDead())
        {
            Renderer::GetInstance().AddBattleLog("플레이어가 사망했습니다...");
            Sleep(3000);
            return BattleResult::PlayerDead;
        }

        if (monster->IsDead())
        {
            if (monster->GetType() == MonsterType::MaxRabbit)
            {
                Renderer::GetInstance().AddBattleLog(monster->GetName() + "을(를) 처치했습니다!");
                return BattleResult::PlayerClear;
            }

            Renderer::GetInstance().AddBattleLog(monster->GetName() + "을(를) 처치했습니다!");

            player->SetExp(player->GetExp() + monster->GetExp());
            if (player->GetExp() >= player->GetLevelUpExp())
                player->LevelUp();

            Renderer::GetInstance().AddBattleLog("경험치 " + std::to_string(monster->GetExp()) + " 획득! (현재 경험치 : " + std::to_string(player->GetExp()) + ")");

            GiveReward(player, monster);
            return BattleResult::PlayerWin;
        }
    }
}

// ---------------------------------------------------------------
// 전투 - 주사위 비교 후 높은 쪽이 공격 (같으면 플레이어 우선)
// ---------------------------------------------------------------
void BattleManager::StartBattle(Character* player, Monster* monster)
{
    int playerRoll = diceManager.Roll(player);
    int monsterRoll = monster->RollAttackDice();

    Renderer::GetInstance().AddBattleLog("플레이어의 주사위의 눈은 [" + std::to_string(playerRoll) + "] 입니다!");
    Renderer::GetInstance().AddBattleLog(monster->GetName() + "의 주사위의 눈은 [" + " [" + std::to_string(monsterRoll) + "] 입니다!");

    if (playerRoll >= monsterRoll)
    {
        Renderer::GetInstance().AddBattleLog("플레이어가 공격합니다!");
        CalculateDamage(player, monster, playerRoll);
    }
    else
    {
        Renderer::GetInstance().AddBattleLog(monster->GetName() + "이(가) 공격합니다!");
        CalculateDamage(monster, player, monsterRoll);
    }

    std::cout << std::endl;
}

// ---------------------------------------------------------------
// 도망 - 플레이어 주사위 > 몬스터 주사위일 때만 성공
// ---------------------------------------------------------------
bool BattleManager::TryEscape(Character* player, Monster* monster, int& outMonsterRoll)
{
    Renderer::GetInstance().AddBattleLog("[ 도망 시도 ]");

    int playerRoll = diceManager.Roll(player);
    int monsterRoll = monster->RollAttackDice();

    // 아웃파라미터로 몬스터 roll값 저장
    outMonsterRoll = monsterRoll;

    Renderer::GetInstance().AddBattleLog("플레이어 [" + std::to_string(playerRoll) + "] vs " + monster->GetName() + " [" + std::to_string(monsterRoll) + "]");

    return playerRoll > monsterRoll;
}

// ---------------------------------------------------------------
// 데미지 계산 - 최소 1 보장
// ---------------------------------------------------------------
void BattleManager::CalculateDamage(Actor* attacker, Actor* defender, int Roll)
{
    int damage = std::max(1, attacker->GetAtk() + Roll - defender->GetDef());
    int newHp = std::max(0, defender->GetHP() - damage);

    defender->SetHP(newHp);

    Renderer::GetInstance().AddBattleLog(attacker->GetName() + "의 공격! " + defender->GetName() + "에게 " + std::to_string(damage) + " 데미지!" + " (남은 HP : " + std::to_string(defender->GetHP()) + ")");
}

// ---------------------------------------------------------------
// 전투 종료 체크
// ---------------------------------------------------------------
bool BattleManager::IsOver(Character* player, Monster* monster)
{
    return player->IsDead() || monster->IsDead();
}

// ---------------------------------------------------------------
// 보상 선택
// ---------------------------------------------------------------
void BattleManager::GiveReward(Character* player, Monster* monster)
{
    Renderer::GetInstance().RenderRewardSelect();

    InputResult input = Tools<int>::Input(1, 2);

    std::cout << std::endl;

    if (1 == input.value)
        GiveNormalReward(player, monster);
    else if (2 == input.value)
        GiveRiskyReward(player, monster);
    else
    {
        Renderer::GetInstance().AddBattleLog("잘못된 입력입니다. 1번 일반 보상으로 지급합니다.");
        GiveNormalReward(player, monster);
    }
}

void BattleManager::GiveNormalReward(Character* player, Monster* monster)
{
    int gold = monster->GetRewardGold();
    player->AddGold(gold);
    int current = player->GetRestTicket();
    player->SetRestTicket(current + 1);

    Renderer::GetInstance().AddBattleLog("골드 " + std::to_string(gold) + " 획득!" + "휴식권 1회 획득! (현재 골드: " + std::to_string(player->GetGold()) + ")");
    Renderer::GetInstance().AddBattleLog("휴식권 1회 획득! (현재 휴식권 : " + std::to_string(player->GetRestTicket()) + ")");
}

void BattleManager::GiveRiskyReward(Character* player, Monster* monster)
{
    Renderer::GetInstance().AddBattleLog("[ 리스크 보상 도전! ]");

    int playerRoll = diceManager.Roll(player);
    int monsterGetDice = monster->GetDiceChallengeValue();

    if (playerRoll >= monsterGetDice)
    {
        Renderer::GetInstance().AddBattleLog("성공!");
        player->GetInventory()->AddDice(monster->GetRewardDiceID());
    }
    else
    {
        Renderer::GetInstance().AddBattleLog("실패!");
    }

    Sleep(3000);
}