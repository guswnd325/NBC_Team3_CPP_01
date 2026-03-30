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


void DiceManager::DiceAnimationRoll()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    std::vector<std::vector<std::string>> frames = {
        
        { "+-------+",
          "|       |",
          "|   .   |",
          "|       |",
          "+-------+" },

          
          { "+-------+",
            "|     . |",
            "|       |",
            "| .     |",
            "+-------+" },

           
            { "+-------+",
              "|     . |",
              "|   .   |",
              "| .     |",
              "+-------+" },

           
              { "+-------+",
                "| .   . |",
                "|       |",
                "| .   . |",
                "+-------+" },

                
                { "+-------+",
                  "| .   . |",
                  "|   .   |",
                  "| .   . |",
                  "+-------+" },

                  
                  { "+-------+",
                    "| .   . |",
                    "| .   . |",
                    "| .   . |",
                    "+-------+" }
    };

    const int FRAME_LINES = 5;

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    srand(static_cast<unsigned int>(time(nullptr)));

    AudioManager::GetInstance().PlaySFX(SFXList::dice_roll);

    for (const std::string& line : frames[0]) {
        std::cout << "\t" << line << "\n";
    }

    for (int i = 1; i < 16; ++i) {
        std::cout << "\033[" << FRAME_LINES << "A";
        int frameIdx = rand() % frames.size();
        for (const std::string& line : frames[frameIdx]) {
            std::cout << "\r\t" << line << "          \n";
        }
        std::cout.flush();

        int delay = 10+ (i * i);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    std::cout << "\n\t[ 결과 확인 중... ]" << std::endl;
}


int DiceManager::Roll(Character* character)
{
    int totalSum = 0;

    // 캐릭터의 인벤토리에서 주사위 목록을 가져옵니다.
    auto& diceList = character->GetInventory()->GetDiceStorege();

    DiceAnimationRoll();

    for (const auto& slot : diceList)
    {
        Dice* dice = slot.dice;
        int count = slot.count;

        if (dice == nullptr) continue; 

        //주사위 객체가 가진 min~max 범위를 사용
        std::uniform_int_distribution<int> dis(dice->minSide, dice->maxSide);

        for (int i = 0; i < count; i++)
        {
            int roll = dis(gen);
            totalSum += roll;

            
            std::cout << "[" << dice->minSide << "~" << dice->maxSide << " 범위: " << roll << "] ";
        }
    }

    std::cout << "\n총합: " << totalSum << std::endl;

    return totalSum;
}

void DiceManager::SetDiceID(Dice& dice)
{
    int min = dice.minSide;
    int max = dice.maxSide;

    if (min == 1 && max == 2)      dice.id = DiceID::D1_2;
    else if (min == 1 && max == 3) dice.id = DiceID::D1_3;
    else if (min == 1 && max == 4) dice.id = DiceID::D1_4;
    else if (min == 1 && max == 6) dice.id = DiceID::D1_6;
    else if (min == 1 && max == 8) dice.id = DiceID::D1_8;
    else if (min == 1 && max == 24) dice.id = DiceID::D1_24;
    else if (min == 2 && max == 3) dice.id = DiceID::D2_3;
    else if (min == 2 && max == 5) dice.id = DiceID::D2_5;
    else if (min == 3 && max == 6) dice.id = DiceID::D3_6;
    // 필요에 따라 추가해도 될듯
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
    if (min == 1) {
        if (max == 2) return DiceID::D1_2;
        if (max == 3) return DiceID::D1_3;
        if (max == 4) return DiceID::D1_4;
        if (max == 6) return DiceID::D1_6;
        if (max == 8) return DiceID::D1_8;
        if (max == 24) return DiceID::D1_24;
    }
    else if (min == 2) {
        if (max == 3) return DiceID::D2_3;
        if (max == 5) return DiceID::D2_5;
    }
    else if (min == 3) {
        if (max == 6) return DiceID::D3_6;
    }

    
    return DiceID::D1_6;
}


