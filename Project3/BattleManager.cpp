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

void BattleManager::DrawDiceDirectly(int num) {
    std::vector<std::string> frame = diceManager.GetDiceFrame(num);

    int startX = 63;
    int startY = Renderer::ZONE_SCREEN_Y + 4; // 위치 점프가 있다면 +4~6 사이로 조정

    for (int i = 0; i < (int)frame.size(); i++) {
        Renderer::GetInstance().MoveCursor(startX, startY + i);
        int vLen = Renderer::GetInstance().GetVisualLength(frame[i]);
        std::cout << frame[i] << std::string(std::max(0, 35 - vLen), ' ');
    }

    // [핵심 추가] 주사위를 다 그린 후 커서를 화면 맨 하단이나 구석으로 치웁니다.
    // 이렇게 하면 이후 발생하는 일반 std::cout이 주사위 칸을 침범하지 못합니다.
    Renderer::GetInstance().MoveCursor(0, 35); // 로그창 아래 빈 공간으로 유도
}

// ---------------------------------------------------------------
// [편법 함수] 주사위 영역만 공백으로 싹 지우는 함수 (cls 없이 지우기)
// ---------------------------------------------------------------
void BattleManager::ClearDiceDirectly() {
    int startX = 63;
    int startY = Renderer::ZONE_SCREEN_Y + 6;
    for (int i = 0; i < 10; i++) {
        Renderer::GetInstance().MoveCursor(startX, startY + i);
        std::cout << std::string(35, ' ');
    }
}

void BattleManager::StartBattle(Character* player, Monster* monster)
{
    // [1] 전투 시작 전 전체 화면 갱신 (틀을 잡고 로그 초기화)
    Renderer::GetInstance().RenderBattleAction(monster, player, {});

    // --- 플레이어 턴 ---
    int playerRoll = diceManager.Roll(player);
    for (int i = 0; i < 10; i++) {
        DrawDiceDirectly(rand() % 20 + 1);
        Sleep(40 + (i * 10));
    }
    DrawDiceDirectly(playerRoll); // 최종 결과 고정

    // 로그만 추가 (화면은 아직 갱신하지 않음 - 깜빡임 방지)
    Renderer::GetInstance().AddBattleLog("플레이어 주사위: [" + std::to_string(playerRoll) + "]");
    Sleep(800);

    // --- 몬스터 턴 준비 ---
    ClearDiceDirectly(); // 주사위 칸만 슥 지우기

    // [팁] 몬스터가 주사위 굴린다는 로그를 미리 반영하기 위해 전체 렌더링 1회
    // 이때 주사위 위치가 살짝 튈 수 있지만, 어차피 비운 상태({})라 자연스럽습니다.
    Renderer::GetInstance().RenderBattleAction(monster, player, {});

    // --- 몬스터 턴 ---
    // monster->RollAttackDice() 내부에서 "주사위 2개 굴림" 등의 std::cout이 있어도
    // 위에서 커서를 치워뒀기 때문에 안전합니다.
    int monsterRoll = monster->RollAttackDice();
    for (int i = 0; i < 10; i++) {
        DrawDiceDirectly(rand() % 12 + 1);
        Sleep(40 + (i * 10));
    }
    DrawDiceDirectly(monsterRoll); // 최종 결과 고정

    Renderer::GetInstance().AddBattleLog(monster->GetName() + " 주사위: [" + std::to_string(monsterRoll) + "]");
    Sleep(800);

    // [최종 동기화] 판정 전 모든 로그와 상태를 화면에 맞춤 (깜빡임 1번)
    Renderer::GetInstance().RenderBattleAction(monster, player, {});

    // --- 판정 및 데미지 계산 ---
    if (playerRoll >= monsterRoll)
    {
        Renderer::GetInstance().AddBattleLog("플레이어가 승리하여 공격합니다!");
        CalculateDamage(player, monster, playerRoll, true);
    }
    else
    {
        Renderer::GetInstance().AddBattleLog(monster->GetName() + "이(가) 승리하여 공격합니다!");
        CalculateDamage(monster, player, monsterRoll, false);
    }
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
void BattleManager::CalculateDamage(Actor* attacker, Actor* defender, int Roll, bool defenderIsMonster)
{
    // 데미지 계산 (최소 1 보장)
    int rawDamage = attacker->GetAtk() + Roll - defender->GetDef();
    int damage = (rawDamage > 1) ? rawDamage : 1;

    
    int currentHp = defender->GetHP() - damage;
    int newHp = (currentHp > 0) ? currentHp : 0;
    defender->SetHP(newHp);

    if (defenderIsMonster) {
        Monster* m = static_cast<Monster*>(defender);

        int startY = Renderer::ZONE_SCREEN_Y + 6;

        
        EffectManager::PlayMonsterHitEffect(m->GetVisual(), 0, startY, 60);

        
        Renderer::GetInstance().RenderBattleAction(m, (Character*)attacker, {});
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