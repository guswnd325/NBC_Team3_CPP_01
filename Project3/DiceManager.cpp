#include "DiceManager.h"
#include "Character.h"
#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

std::random_device DiceManager::rd;
std::mt19937 DiceManager::gen(DiceManager::rd());

DiceManager::DiceManager() {}
DiceManager::~DiceManager() {}

/*
void DiceManager::PlayRollSound()
{
	PlaySound(TEXT("dice_roll.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
}
*/

int DiceManager::Roll(Character* character)
{
    int totalSum = 0;

    // 캐릭터의 인벤토리에서 주사위 목록을 가져옵니다.
    auto& diceList = character->GetInventory()->GetDiceStorege();

    for (const auto& slot : diceList)
    {
        Dice* dice = slot.dice;
        int count = slot.count;

        if (dice == nullptr) continue; 

        // 핵심 수정: 1~side가 아니라 주사위 객체가 가진 min~max 범위를 사용합니다.
        std::uniform_int_distribution<int> dis(dice->minSide, dice->maxSide);

        for (int i = 0; i < count; i++)
        {
            int roll = dis(gen);
            totalSum += roll;

            // 출력 메시지도 범위에 맞게 수정
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


