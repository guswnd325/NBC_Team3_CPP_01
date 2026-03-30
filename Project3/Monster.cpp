#include "Monster.h"
#include <iostream>
#include <random>

Monster::Monster(MonsterType type, MonsterGrade grade) {
	this->type = type;
	this->grade = grade;
	InitializeMonster();
}

void Monster::InitializeMonster() {
	switch (type)
	{
	case MonsterType::Slime:
		SetName("슬라임");
		visual = {
			"      .---.      ",
			"   ./:::::(\\     ", // 명암처리와 하이라이트(\\)
			"  /::::::::::\\   ", // 입체감 있는 몸체
			" |::::\033[94mO\033[0m:::::\033[94mO\033[0m:|  ", // 눈 부분 파란색 강조(선택사항)
			"  \\::::::::::/   ",
			"   '~~\033[91m_~_\033[0m~~' "
		};
		SetHP(20); SetAtk(5); SetDef(0); SetExp(10); 
		SetRewards(20, 1, DiceID::D1_2, 4);
		SetDiceCount(1); SetDiceSides(6);
		break;
	case MonsterType::Goblin:
		SetName("고블린");
		visual = {
			"   ^  __  ^     ",
			"  / \\/  \\/ \\    ",
			" (  O    O  )   ",
			"  \\   vv   /    ",
			"   '------'     "
		};
		SetHP(40); SetAtk(10); SetDef(5); SetExp(20);
		SetRewards(50, 1, DiceID::D1_3, 7);
		SetDiceCount(2); SetDiceSides(4);
		break;
	case MonsterType::Orc:
		SetHP(50); SetAtk(1); SetDef(10); SetExp(30);
		SetRewards(80, 1, DiceID::D2_3, 10);
		SetDiceCount(3); SetDiceSides(4);
	case MonsterType::Golem:
		SetName("골렘");
		SetHP(150); SetAtk(25); SetDef(20); SetExp(100);
		SetRewards(150, 1, DiceID::D1_4, 10);
		SetDiceCount(1); SetDiceSides(12);
		break;
	case MonsterType::Troll:
		SetHP(80); SetAtk(20); SetDef(15); SetExp(40);
		SetRewards(200, 1, DiceID::D1_6, 10);
		SetDiceCount(2); SetDiceSides(6);
		break;
	case MonsterType::MaxRabbit:
		SetName("만렙토끼");
		SetHP(500); SetAtk(60); SetDef(40); SetExp(500);
		SetRewards(1000, 1, DiceID::D3_6, 15);
		SetDiceCount(20); SetDiceSides(2);
		break;
	default:
		break;
	}
}

int Monster::RollAttackDice() {
	int total = 0;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, diceSides);

	std::cout << GetName() << "이(가) 주사위 " << diceCount << "개를 굴립니다: ";

	for (int i = 0; i < diceCount; ++i) {
		int roll = dis(gen);
		total += roll;
		std::cout << "[" << roll << "] ";
	}

	std::cout << " (총합: " << total << ")" << std::endl;
	return total;
}

void Monster::SetRewards(int gold, int recover, DiceID diceID, int challenge) {
	rewardGold = gold;
	recoverAmount = recover;
	rewardDiceID = diceID;
	diceChallengeValue = challenge;
}