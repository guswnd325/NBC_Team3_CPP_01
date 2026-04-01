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

void CombatManager::UnlockAreas(int level, bool isNewUnlock)
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
                    // 실제로 해금.      
                    std::string displayName = areaDisplayname[areaKey];
                    Renderer::GetInstance().AddSystemLog("새로운 지역 해금: [" + displayName + "]!", BRIGHT_YELLOW);
                    Renderer::GetInstance().RenderRewardSelect(std::vector<std::string>());
                }
            }
        }
    }

    if (isNewUnlock)
    {
        Renderer::GetInstance().Delay(sleepTime_fast);
        Renderer::GetInstance().ClearSystemLogs();
    }

}

std::string CombatManager::GetCurrentAreaName()
{
    return currentAreaName;
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
                Renderer::GetInstance().ClearSystemLogs();
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
    
    currentAreaName = areaDisplayname[selectedArea];
    // 맵 이름 돚거

    if (selectedArea.empty()) // 0 입력 시 마을로
        return BattleResult::Escaped; // 혹은 별도의 ReturnToTown 값

    AudioManager::PlayAreaBGM(selectedArea);
    Monster* monster = monsterManager->SpawnMonster(selectedArea);
    return battleManager->Run(player, monster, this);
}


void CombatManager::ShowCredit()
{
    int totalRolls = DiceManager::GetRollCount();
    const int DISPLAY_WIDTH = 60; // RenderSplitScreen의 한쪽 영역 대략적 너비

    // [헬퍼 함수] 색상 코드를 제외한 실제 길이를 계산하여 중앙 공백 생성
    auto GetCenterPadding = [&](std::string text) {
        int vLen = Renderer::GetInstance().GetVisualLength(text);
        int pad = (DISPLAY_WIDTH - vLen) / 2;
        return std::string(std::max(0, pad), ' ');
        };

    // 각 줄의 내용 정의 (색상 코드 포함)
    std::vector<std::string> rawLines = {
        "",
        "",
        "",
        "==============================",
        "GAME OVER",
        "==============================",
        "",
        "\033[93m사실 나도 게임을 이겨본적이 없어...\033[0m",
        "",
        "\033[93m총 주사위 굴린 횟수 : \033[97m" + std::to_string(totalRolls) + "회\033[0m",
        "",
        "\033[97m플레이해주셔서 감사합니다.\033[0m",
        "",
        "\033[40mDeveloped by 3조\033[0m",
        ""
    };

    std::vector<std::string> currentContent;
    std::vector<std::string> emptyRight;

    for (const auto& line : rawLines)
    {
        // 빈 줄이 아닐 경우에만 중앙 정렬 공백 추가
        if (!line.empty()) {
            currentContent.push_back(GetCenterPadding(line) + line);
        }
        else {
            currentContent.push_back("");
        }

        // 화면 갱신
        Renderer::GetInstance().RenderSplitScreen(
            currentContent,
            emptyRight,
            "THANKS FOR PLAYING",
            false
        );
        Sleep(700); // 한 줄씩 스르륵 나타나는 연출
    }

    Sleep(3000);
}