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
    int minSide;    // 최저 값
    int maxSide;    // 최대 값
    DiceID id;      // 주사위 식별자

    // DiceID를 매개변수로 받는 생성자
    Dice(DiceID diceId) : id(diceId)
    {
        switch (id)
        {
        case DiceID::D1_2:   minSide = 1; maxSide = 2;  break;
        case DiceID::D1_3:   minSide = 1; maxSide = 3;  break;
        case DiceID::D1_4:   minSide = 1; maxSide = 4;  break;
        case DiceID::D1_6:   minSide = 1; maxSide = 6;  break;
        case DiceID::D1_8:   minSide = 1; maxSide = 8;  break;
        case DiceID::D2_3:   minSide = 2; maxSide = 3;  break;
        case DiceID::D2_5:   minSide = 2; maxSide = 5;  break;
        case DiceID::D3_6:   minSide = 3; maxSide = 6;  break;
        default:             minSide = 1; maxSide = 6;  break;
        }
    }

    // 게터 함수들
    int GetMin() const { return minSide; }
    int GetMax() const { return maxSide; }
    DiceID GetId() const { return id; }
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

	// 이제 Roll 함수가 Dice 구조체를 매개변수로 받습니다.
	int Roll(Character* character);
	
	//주사위 소리 재생
	//void PlayRollSound();
};

