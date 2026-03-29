#include "CombatManager.h"
#include "BattleManager.h"

void CombatManager::GenerateAreaChoices()
{
    std::vector<std::string> shuffle = areaName;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffle.begin(), shuffle.end(), g);

    currentChoices.clear();
    for (int i = 0; i < 3; i++)
    {
        currentChoices.push_back(shuffle[i]);
    }
}

void CombatManager::DisplayChoices()
{
    std::cout << "┌───────────────────────────────────────────┐" << std::endl;
    std::cout << "│          ◈ 탐험 지역 선택 ◈           │" << std::endl;
    std::cout << "├───────────────────────────────────────────┤" << std::endl;
    std::cout << "│  어디로 이동하시겠습니까?                 │" << std::endl;

    for (int i = 0; i < currentChoices.size(); ++i) {
        std::string englishName = currentChoices[i];
        std::string koreanName = areaDisplayname[englishName];

        // 만약 맵에 등록되지 않은 지역명이 들어올 경우를 대비한 예외 처리
        if (koreanName.empty()) {
            koreanName = englishName; // 한국어가 없으면 영어라도 출력
        }

        std::cout << "│ " << " [" << i + 1 << "] " << koreanName << "│ " << std::endl;
    }
    std::cout << "└───────────────────────────────────────────┘" << std::endl;
}

//이거 반환 값 그대로 MonsterManager의 SpawnManager에 넣으면 됩니다
std::string CombatManager::SelectArea() {
    int input;

    while (true) {
        std::cout << " > 이동할 좌표 입력하라 : ";

        // 1. 입력 성공 여부와 범위 확인
        if (std::cin >> input && input >= 1 && input <= 3) {
            // 입력 성공 시: 버퍼를 비우고 즉시 결과 반환 (함수 종료)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return currentChoices[input - 1];
        }

        std::cout << "잘못된 입력입니다. 1, 2, 3 중 하나를 입력해주세요." << std::endl;

        std::cin.clear(); // 에러 플래그 초기화
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 잘못된 입력 제거

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
void CombatManager::Run(Character* player)
{
    GenerateAreaChoices();
    DisplayChoices();
    std::string selectedArea = SelectArea();
    Monster* monster = monsterManager->SpawnMonster(selectedArea);

    //monster주석처리
    battleManager->Run(player, monster);
}








