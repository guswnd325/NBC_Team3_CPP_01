#include "BattleManager.h"
#include <iostream>
#include <limits>

// ---------------------------------------------------------------
// Run - 전투 전체 흐름
// ---------------------------------------------------------------
BattleResult BattleManager::Run(Character* player, Monster* monster)
{
    // TODO: Renderer::GetInstance().RenderBattleStart(monster)
    std::cout << "\n==============================" << std::endl;
    std::cout << monster->GetName() << "이(가) 나타났다!" << std::endl;
    std::cout << "HP: " << monster->GetHP()
        << " | ATK: " << monster->GetAtk()
        << " | DEF: " << monster->GetDef() << std::endl;
    std::cout << "==============================\n" << std::endl;

    while (true)
    {
        // TODO: Renderer::GetInstance().RenderBattleAction()
        std::cout << "┌───────────────────────────┐" << std::endl;
        std::cout << "│       ◈ 행동 선택 ◈        │" << std::endl;
        std::cout << "├───────────────────────────┤" << std::endl;
        std::cout << "│  [1] 전투                  │" << std::endl;
        std::cout << "│  [2] 도망                  │" << std::endl;
        std::cout << "└───────────────────────────┘" << std::endl;
        std::cout << " > 행동을 선택해라 : ";

        int input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << std::endl;

        if (input == 1)
        {
            StartBattle(player, monster);
        }
        else if (input == 2)
        {
            if (TryEscape(player, monster))
            {
                // TODO: Renderer::GetInstance().RenderEscapeResult(true)
                std::cout << "도망에 성공했습니다!\n" << std::endl;
                return BattleResult::Escaped;
            }
            else
            {
                // TODO: Renderer::GetInstance().RenderEscapeResult(false)
                std::cout << "도망에 실패! " << monster->GetName()
                    << "이(가) 공격합니다!\n" << std::endl;
                CalculateDamage(monster, player);
            }
        }
        else
        {
            // TODO: Renderer::GetInstance().RenderInvalidInput()
            std::cout << "잘못된 입력입니다. 1 또는 2를 입력해주세요." << std::endl;
            continue;
        }

        // 전투 종료 여부 체크
        if (player->IsDead())
        {
            // TODO: Renderer::GetInstance().RenderBattleOver(false)
            std::cout << "\n플레이어가 사망했습니다..." << std::endl;
            return BattleResult::PlayerDead;
        }

        if (monster->IsDead())
        {
            // TODO: Renderer::GetInstance().RenderBattleOver(monster)
            std::cout << "\n" << monster->GetName() << "을(를) 처치했습니다!" << std::endl;

            int gainedExp = monster->GetExp();
            player->SetExp(player->GetExp() + gainedExp);

            // TODO: Renderer::GetInstance().RenderExpGain(gainedExp)
            std::cout << "경험치 +" << gainedExp << " 획득!\n" << std::endl;

            GiveReward(player, monster);
            return BattleResult::PlayerWin;
        }
    }
}

// ---------------------------------------------------------------
// 주사위 굴리기
// ---------------------------------------------------------------



// ---------------------------------------------------------------
// 전투 - 주사위 비교 후 높은 쪽이 공격 (같으면 플레이어 우선)
// ---------------------------------------------------------------
void BattleManager::StartBattle(Character* player, Monster* monster)
{
    //int playerRoll = RollPlayerDice(player);
    //int monsterRoll = RollMonsterDice(monster);

    int playerRoll = 1;
    int monsterRoll = monster->RollAttackDice();

    std::cout << "플레이어 [" << playerRoll << "] vs "
        << monster->GetName() << " [" << monsterRoll << "]" << std::endl;

    if (playerRoll >= monsterRoll)
    {
        std::cout << "플레이어가 공격합니다!" << std::endl;
        CalculateDamage(player, monster);
    }
    else
    {
        std::cout << monster->GetName() << "이(가) 공격합니다!" << std::endl;
        CalculateDamage(monster, player);
    }

    std::cout << std::endl;
}

// ---------------------------------------------------------------
// 도망 - 플레이어 주사위 > 몬스터 주사위일 때만 성공
// ---------------------------------------------------------------
bool BattleManager::TryEscape(Character* player, Monster* monster)
{
    // TODO: Renderer::GetInstance().RenderEscapeTry()
    std::cout << "[ 도망 시도 ]" << std::endl;

    //int playerRoll = RollPlayerDice(player);
    //int monsterRoll = RollMonsterDice(monster);

    int playerRoll = 10;
    int monsterRoll = monster->RollAttackDice();

    // TODO: Renderer::GetInstance().RenderBattleResult(playerRoll, monsterRoll)
    std::cout << "플레이어 [" << playerRoll << "] vs "
        << monster->GetName() << " [" << monsterRoll << "]" << std::endl;

    return playerRoll > monsterRoll;
}

// ---------------------------------------------------------------
// 데미지 계산 - 최소 1 보장
// ---------------------------------------------------------------
void BattleManager::CalculateDamage(Actor* attacker, Actor* defender)
{
    int damage = std::max(1, attacker->GetAtk() - defender->GetDef());
    int newHp = std::max(0, defender->GetHP() - damage);

    defender->SetHP(newHp);

    // TODO: Renderer::GetInstance().RenderDamage(attacker, defender, damage)
    std::cout << attacker->GetName() << "의 공격! "
        << defender->GetName() << "에게 " << damage << " 데미지!"
        << " (남은 HP: " << defender->GetHP() << ")" << std::endl;
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
    // TODO: Renderer::GetInstance().RenderRewardSelect()
    std::cout << "┌───────────────────────────────┐" << std::endl;
    std::cout << "│         ◈ 보상 선택 ◈          │" << std::endl;
    std::cout << "├───────────────────────────────┤" << std::endl;
    std::cout << "│  [1] 일반 보상                 │" << std::endl;
    std::cout << "│      (휴식권 1회 + 골드)        │" << std::endl;
    std::cout << "│  [2] 리스크 보상               │" << std::endl;
    std::cout << "│      (주사위로 고보상 도전)     │" << std::endl;
    std::cout << "└───────────────────────────────┘" << std::endl;
    std::cout << " > 보상을 선택해라 : ";

    int input;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << std::endl;

    if (1 == input)
        GiveRiskyReward(player, monster);
    else if (2 == input)
        GiveNormalReward(player, monster);
    else
    {
        // TODO: Renderer::GetInstance().RenderInvalidInput()
        std::cout << "잘못된 입력입니다. 1번 일반 보상으로 지급합니다." << std::endl;
        GiveNormalReward(player, monster);
    }
}

void BattleManager::GiveNormalReward(Character* player, Monster* monster)
{
    int gold = monster->GetRewardGold();
    player->AddGold(gold);

    // TODO: Renderer::GetInstance().RenderNormalReward(gold, player->GetGold())
    // TODO: 휴식권 1회 추가 → RestManager 구현 후 연동
    std::cout << "골드 +" << gold << " 획득!" << std::endl;
    std::cout << "휴식권 1회 획득! (현재 골드: " << player->GetGold() << ")" << std::endl;
}

void BattleManager::GiveRiskyReward(Character* player, Monster* monster)
{
    // TODO: Renderer::GetInstance().RenderRiskyReward()
    // TODO: 리스크 보상 시나리오 확정 후 구현
    std::cout << "[ 리스크 보상 도전! ]" << std::endl;

    //std::cout << "도전 결과 : " << roll << std::endl;
    std::cout << "(리스크 보상 상세 내용 미구현)" << std::endl;
}