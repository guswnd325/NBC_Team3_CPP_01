#include "BattleManager.h"
#include "EffectManager.h" // 추가
#include <iostream>
#include <limits>
#include <string>

// ---------------------------------------------------------------
// Run - 전투 전체 흐름
// ---------------------------------------------------------------
BattleResult BattleManager::Run(Character* player, Monster* monster)
{
    Renderer::GetInstance().ClearBattleLogs();
    Renderer::GetInstance().AddBattleLog(monster->GetName() + "(이)가 나타났다!");
    while (true)
    {
        Renderer::GetInstance().RenderBattleAction(monster, player, std::vector<std::string>());

        InputResult input = Tools<int>::Input(1, 2);

        std::cout << std::endl;

        switch (input.value)
        {
        case 1:
        {
            StartBattle(player, monster);
            break;
        }
        case 2:
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
                CalculateDamage(monster, player, monsterRoll,false);
            }

            break;
        }
        default:
        {
            Renderer::GetInstance().AddBattleLog("잘못된 입력입니다. 1 또는 2를 입력해주세요.");
            Sleep(3000);
            break;
        }
        }

        // 전투 종료 여부 체크
        if (player->IsDead())
        {
            player->PlayerDead();
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

            int gainedExp = monster->GetExp();
            player->SetExp(player->GetExp() + gainedExp);

            Renderer::GetInstance().AddBattleLog("경험치 " + std::to_string(gainedExp) + " 획득");

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
        CalculateDamage(player, monster, playerRoll, true);
    }
    else
    {
        Renderer::GetInstance().AddBattleLog(monster->GetName() + "이(가) 공격합니다!");
        CalculateDamage(monster, player, monsterRoll, false);
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
void BattleManager::CalculateDamage(Actor* attacker, Actor* defender, int Roll, bool defenderIsMonster)
{
    int damage = std::max(1, attacker->GetAtk() + Roll - defender->GetDef());
    int newHp = std::max(0, defender->GetHP() - damage);
    defender->SetHP(newHp);

    if (defenderIsMonster) {
        Monster* m = static_cast<Monster*>(defender);

        // Renderer가 이미지를 그리는 시작 줄 계산
        // ZONE_SCREEN_Y + 상단 UI 5줄
        int startY = Renderer::ZONE_SCREEN_Y + 5;

        // 이펙트 실행 (이제 startX는 위에서 통째로 그리므로 무시해도 됩니다)
        EffectManager::PlayMonsterHitEffect(m->GetVisual(), 0, startY, 60);

        // 이펙트 후 전체 화면 리프레시 (로그 등 나머지 UI 복구)
        Renderer::GetInstance().RenderBattleAction(m, (Character*)attacker, std::vector<std::string>());
    }

    Renderer::GetInstance().AddBattleLog(attacker->GetName() + "의 공격! " +
        defender->GetName() + "에게 " + std::to_string(damage) + " 데미지!");
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
    Renderer::GetInstance().RenderRewardSelect(std::vector<std::string>());

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