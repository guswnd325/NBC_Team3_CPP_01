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
    
    std::vector<std::string> previousAreas = unlockedAreas;

    
    unlockedAreas.clear();
    for (auto& pair : areaUnlockLevel)
    {
        const std::string& areaKey = pair.first;
        int requiredLevel = pair.second;

        if (level >= requiredLevel)
        {
            unlockedAreas.push_back(areaKey);

            
            auto it = std::find(previousAreas.begin(), previousAreas.end(), areaKey);
            if (it == previousAreas.end())
            {
                
                if (level > 1)
                {
                    
                    std::string displayName = areaDisplayname[areaKey];
                    Renderer::GetInstance().AddSystemLog("새로운 지역 해금: [" + displayName + "]!");
                }
            }
        }
    }
}

void CombatManager::DisplayChoices()
{
    Renderer::GetInstance().RenderAreaChoices(currentChoices, areaDisplayname, std::vector<std::string>());
}

std::string CombatManager::SelectArea() {
    int input;
    while (true) {
        if (std::cin >> input) {
            if (input == 0) {
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                return ""; // 빈 문자열 반환 → 호출부에서 마을로 처리
            }
            if (input >= 1 && input <= 3) {
                std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
                return currentChoices[input - 1];
            }
        }
        Renderer::GetInstance().AddSystemLog("잘못된 입력입니다. 1, 2, 3 중 하나를 선택하세요.");
        DisplayChoices();
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }
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
BattleResult CombatManager::Run(Character* player)
{
    UnlockAreas(player->GetLevel());
    GenerateAreaChoices();
    DisplayChoices();
    std::string selectedArea = SelectArea();

    if (selectedArea.empty()) // 0 입력 시 마을로
        return BattleResult::Escaped; // 혹은 별도의 ReturnToTown 값

    Monster* monster = monsterManager->SpawnMonster(selectedArea);
    return battleManager->Run(player, monster, this);
}







