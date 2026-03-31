#include "BattleManager.h"
#include "EffectManager.h" // 추가
#include "CombatManager.h"
#include <iostream>
#include <limits>
#include <string>

// ---------------------------------------------------------------
// Run - 전투 전체 흐름
// ---------------------------------------------------------------
BattleResult BattleManager::Run(Character* player, Monster* monster, CombatManager* combatManager)
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
            Renderer::GetInstance().AddBattleLog("경험치 " + std::to_string(gainedExp) + " 획득! (현재 경험치 : " + std::to_string(player->GetExp()) + ")");

            if (player->GetExp() >= player->GetLevelUpExp())
            {
                player->LevelUp();
                Renderer::GetInstance().AddSystemLog("Level Up! (" + std::to_string(player->GetLevel()) + ")");
                combatManager->UnlockAreas(player->GetLevel());
            }    
                
            GiveReward(player, monster);
            return BattleResult::PlayerWin;
        }
    }
}


//렌더러 최대한 변경하지 않기 위한 바로 주사위 프레임을 사용하는 함수
void BattleManager::DrawDiceDirectly(int num) {
    std::vector<std::string> frame = diceManager.GetDiceFrame(num);

    int startX = 63;
    int startY = Renderer::ZONE_SCREEN_Y + 4; 

    for (int i = 0; i < (int)frame.size(); i++) {
        Renderer::GetInstance().MoveCursor(startX, startY + i);
        int vLen = Renderer::GetInstance().GetVisualLength(frame[i]);
        std::cout << frame[i] << std::string(std::max(0, 35 - vLen), ' ');
    }
    Renderer::GetInstance().MoveCursor(0, 35); 
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
    
    Renderer::GetInstance().RenderBattleAction(monster, player, {});

    AudioManager::GetInstance().PlaySFX(SFXList::Dice_Roll);
    // --- 플레이어 턴 ---
    int playerRoll = diceManager.Roll(player);
    for (int i = 0; i < 10; i++) {
        DrawDiceDirectly(rand() % 20 + 1);
        Sleep(40 + (i * 10));
    }
    DrawDiceDirectly(playerRoll);
    Renderer::GetInstance().AddBattleLog("플레이어 주사위 결과: [" + std::to_string(playerRoll) + "]");
    Sleep(800);

    
    ClearDiceDirectly();

    // [몬스터 배틀 로그에 추가]
    Renderer::GetInstance().AddBattleLog(monster->GetName() + "이(가) " + std::to_string(monster->GetDiceCount()) + "개의 주사위를 굴립니다!");
    Renderer::GetInstance().RenderBattleAction(monster, player, {}); 

    AudioManager::GetInstance().PlaySFX(SFXList::Dice_Roll);
    int monsterRoll = monster->RollAttackDice(); 
    for (int i = 0; i < 10; i++) {
        DrawDiceDirectly(rand() % 12 + 1);
        Sleep(40 + (i * 10));
    }
    DrawDiceDirectly(monsterRoll);

    // 결과 로그 추가
    Renderer::GetInstance().AddBattleLog(monster->GetName() + "의 주사위 결과: [" + std::to_string(monsterRoll) + "]");
    Sleep(800);

    
    Renderer::GetInstance().RenderBattleAction(monster, player, {});

    
    if (playerRoll >= monsterRoll) {
        Renderer::GetInstance().AddBattleLog("플레이어 승리! 공격 개시.");
        CalculateDamage(player, monster, playerRoll, true);
    }
    else {
        Renderer::GetInstance().AddBattleLog(monster->GetName() + " 승리! 반격 당함.");
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