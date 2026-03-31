#include "CombatManager.h"
#include "BattleManager.h"
#include "Renderer.h"

void CombatManager::GenerateAreaChoices()
{
    if (unlockedAreas.empty())
        UnlockAreas(1);

    std::vector<std::string> shuffle = unlockedAreas; 
    std::random_device rd;
    std::mt19937 Rand(rd());
    std::shuffle(shuffle.begin(), shuffle.end(), Rand);
    currentChoices.clear();
    for (int i = 0; i < 3; i++)
    {
        currentChoices.push_back(shuffle[i]);
    }
}

void CombatManager::UnlockAreas(int level)
{
    unlockedAreas.clear();
    for (auto& pair : areaUnlockLevel)
    {
        if (level >= pair.second)
            unlockedAreas.push_back(pair.first);
    }
}

void CombatManager::DisplayChoices()
{
    Renderer::GetInstance().RenderAreaChoices(currentChoices, areaDisplayname);
}

//이거 반환 값 그대로 MonsterManager의 SpawnManager에 넣으면 됩니다
std::string CombatManager::SelectArea() {
    int input;

    while (true) {
        
        // 1. 입력 성공 여부와 범위 확인
        if (std::cin >> input && input >= 1 && input <= 3) {
            // 입력 성공 시: 버퍼를 비우고 즉시 결과 반환 (함수 종료)
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            return currentChoices[input - 1];
        }

        std::cout << "잘못된 입력입니다. 1, 2, 3 중 하나를 입력해주세요." << std::endl;

        std::cin.clear(); // 에러 플래그 초기화
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // 잘못된 입력 제거

    }

    return currentChoices[input - 1];
}

void CombatManager::ReduceHp(Actor* attacker, Actor* defender)
{
    int damage = std::max(1, attacker->GetAtk() - defender->GetDef());
    int newHealth = defender->GetHP() - damage;

    // 체력이 0 미만으로 내려가지 않도록 보정 (출력을 0으로 표시)
    if (newHealth < 0) newHealth = 0;

    defender->SetHP(newHealth);
    std::cout << defender->GetName() << "의 남은 HP: " << defender->GetHP( ) << std::endl;
}


//게임 매니저에서 이것만 호출하면 됩니다
BattleResult  CombatManager::Run(Character* player)
{
    UnlockAreas(player->GetLevel());
    GenerateAreaChoices();
    DisplayChoices();
    std::string selectedArea = SelectArea();
    Monster* monster = monsterManager->SpawnMonster(selectedArea);

    return battleManager->Run(player, monster);
}








