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
void DiceManager::DiceAnimationRoll(const std::vector<int>& results)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    std::vector<std::vector<std::string>> frames = {
        { "+-------+", "|       |", "|   .   |", "|       |", "+-------+" },
        { "+-------+", "|     . |", "|       |", "| .     |", "+-------+" },
        { "+-------+", "|     . |", "|   .   |", "| .     |", "+-------+" },
        { "+-------+", "| .   . |", "|       |", "| .   . |", "+-------+" },
        { "+-------+", "| .   . |", "|   .   |", "| .   . |", "+-------+" },
        { "+-------+", "| .   . |", "| .   . |", "| .   . |", "+-------+" }
    };

    const int FRAME_LINES = 5;
    int diceCount = (int)results.size();

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    srand(static_cast<unsigned int>(time(nullptr)));
    AudioManager::GetInstance().PlaySFX(SFXList::dice_roll);

   
    for (int line = 0; line < FRAME_LINES; ++line) {
        std::cout << "\t";
        for (int d = 0; d < diceCount; ++d) {
            std::cout << frames[0][line] << "  ";
        }
        std::cout << "\n";
    }

    
    for (int i = 1; i < 16; ++i) {
        std::cout << "\033[" << FRAME_LINES << "A";

        std::vector<int> frameIdxs(diceCount);
        for (int d = 0; d < diceCount; ++d) {
            frameIdxs[d] = rand() % frames.size();
        }

        for (int line = 0; line < FRAME_LINES; ++line) {
            std::cout << "\r\t";
            for (int d = 0; d < diceCount; ++d) {
                std::cout << frames[frameIdxs[d]][line] << "  ";
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
            int resultFrame = results[d] - 1;
            if (resultFrame < 0) resultFrame = 0;
            if (resultFrame >= (int)frames.size()) resultFrame = (int)frames.size() - 1;
            std::cout << frames[resultFrame][line] << "  ";
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
    std::cout << "\n\t총합: " << total << std::endl;
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

    DiceAnimationRoll(results);

    return totalSum;
}


void DiceManager::SetDiceID(Dice& dice)
{
    int min = dice.minSide;
    int max = dice.maxSide;

    if (min == 1) {
        if (max == 2)  dice.id = DiceID::D1_2;
        else if (max == 3)  dice.id = DiceID::D1_3;
        else if (max == 4)  dice.id = DiceID::D1_4;
        else if (max == 5)  dice.id = DiceID::D1_5;
        else if (max == 6)  dice.id = DiceID::D1_6;
        else if (max == 7)  dice.id = DiceID::D1_7;
        else if (max == 8)  dice.id = DiceID::D1_8;
        else if (max == 9)  dice.id = DiceID::D1_9;
        else if (max == 10) dice.id = DiceID::D1_10;
        else if (max == 11) dice.id = DiceID::D1_11;
        else if (max == 12) dice.id = DiceID::D1_12;
        else if (max == 13) dice.id = DiceID::D1_13;
        else if (max == 14) dice.id = DiceID::D1_14;
        else if (max == 15) dice.id = DiceID::D1_15;
        else if (max == 16) dice.id = DiceID::D1_16;
        else if (max == 17) dice.id = DiceID::D1_17;
        else if (max == 18) dice.id = DiceID::D1_18;
        else if (max == 19) dice.id = DiceID::D1_19;
        else if (max == 20) dice.id = DiceID::D1_20;
        else if (max == 21) dice.id = DiceID::D1_21;
        else if (max == 22) dice.id = DiceID::D1_22;
        else if (max == 23) dice.id = DiceID::D1_23;
        else if (max == 24) dice.id = DiceID::D1_24;
        else if (max == 25) dice.id = DiceID::D1_25;
        else if (max == 26) dice.id = DiceID::D1_26;
        else if (max == 27) dice.id = DiceID::D1_27;
    }
    else if (min == 2) {
        if (max == 3)       dice.id = DiceID::D2_3;
        else if (max == 4)  dice.id = DiceID::D2_4;
        else if (max == 5)  dice.id = DiceID::D2_5;
        else if (max == 6)  dice.id = DiceID::D2_6;
        else if (max == 7)  dice.id = DiceID::D2_7;
        else if (max == 8)  dice.id = DiceID::D2_8;
        else if (max == 9)  dice.id = DiceID::D2_9;
        else if (max == 10) dice.id = DiceID::D2_10;
        else if (max == 11) dice.id = DiceID::D2_11;
        else if (max == 12) dice.id = DiceID::D2_12;
        else if (max == 13) dice.id = DiceID::D2_13;
        else if (max == 14) dice.id = DiceID::D2_14;
        else if (max == 15) dice.id = DiceID::D2_15;
        else if (max == 16) dice.id = DiceID::D2_16;
        else if (max == 17) dice.id = DiceID::D2_17;
        else if (max == 18) dice.id = DiceID::D2_18;
        else if (max == 19) dice.id = DiceID::D2_19;
        else if (max == 20) dice.id = DiceID::D2_20;
        else if (max == 21) dice.id = DiceID::D2_21;
        else if (max == 22) dice.id = DiceID::D2_22;
        else if (max == 23) dice.id = DiceID::D2_23;
        else if (max == 24) dice.id = DiceID::D2_24;
        else if (max == 25) dice.id = DiceID::D2_25;
        else if (max == 26) dice.id = DiceID::D2_26;
        else if (max == 27) dice.id = DiceID::D2_27;
    }
    else if (min == 3) {
        if (max == 4)       dice.id = DiceID::D3_4;
        else if (max == 5)  dice.id = DiceID::D3_5;
        else if (max == 6)  dice.id = DiceID::D3_6;
        else if (max == 7)  dice.id = DiceID::D3_7;
        else if (max == 8)  dice.id = DiceID::D3_8;
        else if (max == 9)  dice.id = DiceID::D3_9;
        else if (max == 10) dice.id = DiceID::D3_10;
        else if (max == 11) dice.id = DiceID::D3_11;
        else if (max == 12) dice.id = DiceID::D3_12;
        else if (max == 13) dice.id = DiceID::D3_13;
        else if (max == 14) dice.id = DiceID::D3_14;
        else if (max == 15) dice.id = DiceID::D3_15;
        else if (max == 16) dice.id = DiceID::D3_16;
        else if (max == 17) dice.id = DiceID::D3_17;
        else if (max == 18) dice.id = DiceID::D3_18;
        else if (max == 19) dice.id = DiceID::D3_19;
        else if (max == 20) dice.id = DiceID::D3_20;
        else if (max == 21) dice.id = DiceID::D3_21;
        else if (max == 22) dice.id = DiceID::D3_22;
        else if (max == 23) dice.id = DiceID::D3_23;
        else if (max == 24) dice.id = DiceID::D3_24;
        else if (max == 25) dice.id = DiceID::D3_25;
        else if (max == 26) dice.id = DiceID::D3_26;
        else if (max == 27) dice.id = DiceID::D3_27;
    }
    else if (min == 4) {
        if (max == 5)       dice.id = DiceID::D4_5;
        else if (max == 6)  dice.id = DiceID::D4_6;
        else if (max == 7)  dice.id = DiceID::D4_7;
        else if (max == 8)  dice.id = DiceID::D4_8;
        else if (max == 9)  dice.id = DiceID::D4_9;
        else if (max == 10) dice.id = DiceID::D4_10;
        else if (max == 11) dice.id = DiceID::D4_11;
        else if (max == 12) dice.id = DiceID::D4_12;
        else if (max == 13) dice.id = DiceID::D4_13;
        else if (max == 14) dice.id = DiceID::D4_14;
        else if (max == 15) dice.id = DiceID::D4_15;
        else if (max == 16) dice.id = DiceID::D4_16;
        else if (max == 17) dice.id = DiceID::D4_17;
        else if (max == 18) dice.id = DiceID::D4_18;
        else if (max == 19) dice.id = DiceID::D4_19;
        else if (max == 20) dice.id = DiceID::D4_20;
        else if (max == 21) dice.id = DiceID::D4_21;
        else if (max == 22) dice.id = DiceID::D4_22;
        else if (max == 23) dice.id = DiceID::D4_23;
        else if (max == 24) dice.id = DiceID::D4_24;
        else if (max == 25) dice.id = DiceID::D4_25;
        else if (max == 26) dice.id = DiceID::D4_26;
        else if (max == 27) dice.id = DiceID::D4_27;
    }
    else if (min == 5) {
        if (max == 6)       dice.id = DiceID::D5_6;
        else if (max == 7)  dice.id = DiceID::D5_7;
        else if (max == 8)  dice.id = DiceID::D5_8;
        else if (max == 9)  dice.id = DiceID::D5_9;
        else if (max == 10) dice.id = DiceID::D5_10;
        else if (max == 11) dice.id = DiceID::D5_11;
        else if (max == 12) dice.id = DiceID::D5_12;
        else if (max == 13) dice.id = DiceID::D5_13;
        else if (max == 14) dice.id = DiceID::D5_14;
        else if (max == 15) dice.id = DiceID::D5_15;
        else if (max == 16) dice.id = DiceID::D5_16;
        else if (max == 17) dice.id = DiceID::D5_17;
        else if (max == 18) dice.id = DiceID::D5_18;
        else if (max == 19) dice.id = DiceID::D5_19;
        else if (max == 20) dice.id = DiceID::D5_20;
        else if (max == 21) dice.id = DiceID::D5_21;
        else if (max == 22) dice.id = DiceID::D5_22;
        else if (max == 23) dice.id = DiceID::D5_23;
        else if (max == 24) dice.id = DiceID::D5_24;
        else if (max == 25) dice.id = DiceID::D5_25;
        else if (max == 26) dice.id = DiceID::D5_26;
        else if (max == 27) dice.id = DiceID::D5_27;
    }
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
    Dice temp(DiceID::D1_2);
    temp.minSide = min;
    temp.maxSide = max;
    SetDiceID(temp);
    return temp.id;
}

