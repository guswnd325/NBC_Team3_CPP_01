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

    auto& diceList = character->GetInventory()->GetDiceStorege();

    for (const auto& slot : diceList)
    {
        Dice* dice = slot.dice;
        int count = slot.count;

        std::uniform_int_distribution<int> dis(1, dice->side);

        for (int i = 0; i < count; i++)
        {
            int roll = dis(gen);
            totalSum += roll;

            std::cout << "[" << dice->side << "면체: " << roll << "] ";
        }
    }

    std::cout << "총합: " << totalSum << std::endl;

    return totalSum;
}

void DiceManager::UpgradeDice(Dice& targetDice, int amount)
{
	targetDice.side += amount;

	std::cout << "주사위의 최대 수치가 " << targetDice.side << " (으)로 증가했습니다." << std::endl;

}
