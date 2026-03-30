#pragma once
#include <iostream>
#include <string>
#include <random>
#include "Actor.h"

class Character;

enum class DiceID
{
	D1_2,   // 1~2
	D1_3,   // 1~3
	D1_4,   // 1~4
	D1_6, 
    D1_8,
	D2_3,   
	D2_5,   
	D3_6,       
};

struct Dice
{
    int minSide;
    int maxSide;
    DiceID id;

    Dice(DiceID diceId) { SetByID(diceId); }

    // 단순히 현재 ID를 알려주는 기능은 구조체에 적합합니다.
    DiceID GetId() const { return id; }
    int GetMin() const { return minSide; }
    int GetMax() const { return maxSide; }

    // ID 기반 초기 설정
    void SetByID(DiceID diceId) {
        id = diceId;
        switch (id) {
        case DiceID::D1_2: minSide = 1; maxSide = 2; break;
        case DiceID::D1_3: minSide = 1; maxSide = 3; break;
        case DiceID::D1_4: minSide = 1; maxSide = 4; break;
        case DiceID::D1_6: minSide = 1; maxSide = 6; break;
        case DiceID::D1_8: minSide = 1; maxSide = 8; break;
        case DiceID::D2_3: minSide = 2; maxSide = 3; break;
        case DiceID::D2_5: minSide = 2; maxSide = 5; break;
        case DiceID::D3_6: minSide = 3; maxSide = 6; break;
        }
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

	//주사위 소리 재생
	//void PlayRollSound();
};

