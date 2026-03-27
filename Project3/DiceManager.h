#pragma once
#include <iostream>
#include <string>
#include <random>
#include "Actor.h"


struct Dice
{
	int side;			//주사위 면 수
	int id;				//주사위 ID

	Dice (int sides = 6, int diceId = 0) : side(sides), id(diceId) {}

	int GetId()
	{
		return side;
	}

	int GetSide()
	{
		return side;
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

	// 이제 Roll 함수가 Dice 구조체를 매개변수로 받습니다.
	int Roll(const Dice& diceInfo);

	// 보상으로 주사위 면수를 늘려주는 기능 (구조체 값을 수정해서 반환)
	void UpgradeDice(Dice& targetDice, int amount);
	
	//주사위 소리 재생
	//void PlayRollSound();
};

