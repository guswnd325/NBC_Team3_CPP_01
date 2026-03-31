#pragma once
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <thread>
#include <chrono>
#include "Actor.h"


class Character;
class AudioManager;

enum class DiceID
{
    // min = 1 (27개: D1_1 ~ D1_27)
    D1_1, D1_2, D1_3, D1_4, D1_5, D1_6,
    D1_7, D1_8, D1_9, D1_10, D1_11,
    D1_12, D1_13, D1_14, D1_15, D1_16,
    D1_17, D1_18, D1_19, D1_20, D1_21,
    D1_22, D1_23, D1_24, D1_25, D1_26,
    D1_27,
    // min = 2 (26개: D2_2 ~ D2_27)
    D2_2, D2_3, D2_4, D2_5, D2_6, D2_7,
    D2_8, D2_9, D2_10, D2_11, D2_12,
    D2_13, D2_14, D2_15, D2_16, D2_17,
    D2_18, D2_19, D2_20, D2_21, D2_22,
    D2_23, D2_24, D2_25, D2_26, D2_27,
    // min = 3 (25개: D3_3 ~ D3_27)
    D3_3, D3_4, D3_5, D3_6, D3_7, D3_8,
    D3_9, D3_10, D3_11, D3_12, D3_13,
    D3_14, D3_15, D3_16, D3_17, D3_18,
    D3_19, D3_20, D3_21, D3_22, D3_23,
    D3_24, D3_25, D3_26, D3_27,
    // min = 4 (24개: D4_4 ~ D4_27)
    D4_4, D4_5, D4_6, D4_7, D4_8, D4_9,
    D4_10, D4_11, D4_12, D4_13, D4_14,
    D4_15, D4_16, D4_17, D4_18, D4_19,
    D4_20, D4_21, D4_22, D4_23, D4_24,
    D4_25, D4_26, D4_27,
    // min = 5 (23개: D5_5 ~ D5_27)
    D5_5, D5_6, D5_7, D5_8, D5_9, D5_10,
    D5_11, D5_12, D5_13, D5_14, D5_15,
    D5_16, D5_17, D5_18, D5_19, D5_20,
    D5_21, D5_22, D5_23, D5_24, D5_25,
    D5_26, D5_27,
};

struct Dice
{
    int minSide;
    int maxSide;
    int upgradeCount;
    DiceID id;

    Dice(DiceID diceId) : upgradeCount(0) {
        SetByID(diceId);
    }

    DiceID GetId() const { return id; }
    int GetMin() const { return minSide; }
    int GetMax() const { return maxSide; }
    int GetUpgradeCount() const { return upgradeCount; }

    std::string DiceIdToString() const
    {
        return std::to_string(minSide) + "-" + std::to_string(maxSide) + " 주사위";
    }

    void SetUpgradeCount(int count) {
        upgradeCount = (count < 0) ? 0 : count;
    }


    void SetByID(DiceID diceId) {
        id = diceId;
        int idx = static_cast<int>(diceId);

        int min = 1;
        int count = 27;  // 26 → 27
        while (idx >= count) {
            idx -= count;
            min++;
            count--;
        }
        minSide = min;
        maxSide = min + idx;  // min + 1 + idx → min + idx
    }
};

class DiceManager
{
private:

	//난수 생성
	static std::random_device rd;
	static std::mt19937 gen;


public:
	DiceManager();
	~DiceManager();

	int Roll(Character* character);

    //최대값, 최소값 변경 시 주사위 id업데이트
    void SetDiceID(Dice& dice);

    void UpdateMin(Character* character, int value);

    void UpdateMax(Character* character, int value);

    DiceID MakeDiceId(int min, int max);

    void DiceAnimationRollNumber(int result);

    std::vector<std::string>GetDiceFrame(int num);

};

