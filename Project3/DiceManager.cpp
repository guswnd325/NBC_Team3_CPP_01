#include "DiceManager.h"
#include "Character.h"
#include <windows.h>
#include <mmsystem.h>
#include "AudioManager.h"

#pragma comment(lib, "winmm.lib")

std::random_device DiceManager::rd;
std::mt19937 DiceManager::gen(DiceManager::rd());

DiceManager::DiceManager() {}
DiceManager::~DiceManager() {}


void DiceManager::DiceAnimationRollNumber(const std::vector<int>& results)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    const int FRAME_LINES = 5;
    int diceCount = (int)results.size();

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    srand(static_cast<unsigned int>(time(nullptr)));
    AudioManager::GetInstance().PlaySFX(SFXList::Dice_Roll);

    
    auto makeFrame = [](int num) -> std::vector<std::string> {
        std::string numStr = std::to_string(num);
        
        std::string center;
        if (numStr.size() == 1)       center = "  " + numStr + "  ";
        else if (numStr.size() == 2)  center = "  " + numStr + " ";
        else                          center = " " + numStr + " ";

        return {
            "+-------+",
            "|       |",
            "| " + center + " |",
            "|       |",
            "+-------+"
        };
        };

    
    auto firstFrame = makeFrame(rand() % 9 + 1);
    for (int line = 0; line < FRAME_LINES; ++line) {
        std::cout << "\t";
        for (int d = 0; d < diceCount; ++d) {
            std::cout << firstFrame[line] << "  ";
        }
        std::cout << "\n";
    }

    
    for (int i = 1; i < 16; ++i) {
        std::cout << "\033[" << FRAME_LINES << "A";

        std::vector<std::vector<std::string>> frameIdxs(diceCount);
        for (int d = 0; d < diceCount; ++d) {
            frameIdxs[d] = makeFrame(rand() % 9 + 1);
        }

        for (int line = 0; line < FRAME_LINES; ++line) {
            std::cout << "\r\t";
            for (int d = 0; d < diceCount; ++d) {
                std::cout << frameIdxs[d][line] << "  ";
            }
            std::cout << "\n";
        }
        std::cout.flush();

        int delay = 10 + (i * i);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    
    std::cout << "\033[" << FRAME_LINES << "A";
    for (int line = 0; line < FRAME_LINES; ++line) {
        std::cout << "\r\t";
        for (int d = 0; d < diceCount; ++d) {
            auto resultFrame = makeFrame(results[d]);
            std::cout << resultFrame[line] << "  ";
        }
        std::cout << "\n";
    }
    std::cout.flush();

    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    
    std::cout << "\n\t";
    for (int d = 0; d < diceCount; ++d) {
        std::cout << "[  " << results[d] << "  ]" << "  ";
    }

    int total = 0;
    for (int r : results) total += r;
    std::cout << std::endl;
}


int DiceManager::Roll(Character* character)
{
    int totalSum = 0;
    auto& diceList = character->GetInventory()->GetDiceStorege();

    std::vector<int> results;
    for (const auto& slot : diceList)
    {
        Dice* dice = slot.dice;
        if (dice == nullptr) continue;
        std::uniform_int_distribution<int> dis(dice->minSide, dice->maxSide);
        for (int i = 0; i < slot.count; i++)
        {
            int roll = dis(gen);
            results.push_back(roll);
            totalSum += roll;
        }
    }

    DiceAnimationRollNumber({totalSum});

    return totalSum;
}


void DiceManager::SetDiceID(Dice& dice)
{
    int min = dice.minSide;
    int max = dice.maxSide;

    int offset = 0;
    int count = 27;  // 26 → 27
    for (int m = 1; m < min; m++) {
        offset += count;
        count--;
    }
    dice.id = static_cast<DiceID>(offset + (max - min)); 
}


void DiceManager::UpdateMin(Character* character, int value) {
    if (!character || !character->GetInventory()) return;

    
    auto& diceList = character->GetInventory()->GetDiceStorege();

    if (diceList.empty()) {
        std::cout << "수정할 주사위가 없습니다." << std::endl;
        return;
    }

    
    std::cout << "\n=== 수정 가능한 주사위 목록 ===" << std::endl;
    for (int i = 0; i < (int)diceList.size(); ++i) {
        if (diceList[i].dice) {
            std::cout << i << ": [" << diceList[i].dice->minSide << "~" << diceList[i].dice->maxSide << "]" << std::endl;
        }
    }

    int choice = 0;
    std::cout << "수정할 주사위 번호를 입력하세요 (0 ~ " << diceList.size() - 1 << "): ";
    std::cin >> choice;

   
    if (choice >= 0 && choice < (int)diceList.size() && diceList[choice].dice) {
        Dice* targetDice = diceList[choice].dice;
        targetDice->minSide += value;

        
        SetDiceID(*targetDice);

        std::cout << choice << "번 주사위 최소값 수정 완료. 현재 범위: ["
            << targetDice->minSide << "~" << targetDice->maxSide << "]\n";
    }
    else {
        std::cout << "잘못된 선택입니다." << std::endl;
    }
}


void DiceManager::UpdateMax(Character* character, int value) {
    if (!character || !character->GetInventory()) return;

    
    auto& diceList = character->GetInventory()->GetDiceStorege();

    if (diceList.empty()) {
        std::cout << "수정할 주사위가 없습니다." << std::endl;
        return;
    }

    
    std::cout << "\n=== 최대값 수정 가능 목록 ===" << std::endl;
    for (int i = 0; i < (int)diceList.size(); ++i) {
        if (diceList[i].dice) {
            std::cout << i << ": [" << diceList[i].dice->minSide << "~" << diceList[i].dice->maxSide << "]" << std::endl;
        }
    }

    
    int choice = 0;
    std::cout << "수정할 주사위 번호를 입력하세요 (0 ~ " << (int)diceList.size() - 1 << "): ";
    std::cin >> choice;

    
    if (choice >= 0 && choice < (int)diceList.size() && diceList[choice].dice) {
        Dice* targetDice = diceList[choice].dice;

        
        targetDice->maxSide += value;

        //안전 장치: 최대값이 최소값보다 작아지지 않게 방지 
        if (targetDice->maxSide < targetDice->minSide) {
            targetDice->maxSide = targetDice->minSide;
            std::cout << "주의: 최대값이 최소값보다 작을 수 없어 최소값과 동일하게 설정되었습니다.\n";
        }

       
        SetDiceID(*targetDice);

        std::cout << choice << "번 주사위 최대값 수정 완료. 현재 범위: ["
            << targetDice->minSide << "~" << targetDice->maxSide << "]\n";
    }
    else {
        std::cout << "잘못된 선택입니다." << std::endl;
    }
}

//
DiceID DiceManager::MakeDiceId(int min, int max)
{
    int offset = 0;
    int count = 27;  // 26 → 27
    for (int m = 1; m < min; m++) {
        offset += count;
        count--;
    }
    return static_cast<DiceID>(offset + (max - min)); 
}