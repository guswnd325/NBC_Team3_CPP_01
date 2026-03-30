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

