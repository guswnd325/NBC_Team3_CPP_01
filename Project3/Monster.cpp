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
		SetHP(40); SetAtk(2); SetDef(0); SetExp(15);
		SetRewards(30, 1, DiceID::D1_2, 4);
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
		SetHP(60); SetAtk(3); SetDef(2); SetExp(25);
		SetRewards(60, 1, DiceID::D2_3, 6);
		SetDiceCount(2); SetDiceSides(4);
		break;
	case MonsterType::Ghost:
		SetName("유령");
		SetHP(80); SetAtk(8); SetDef(4); SetExp(45);
		SetRewards(90, 1, DiceID::D1_3, 10);
		SetDiceCount(2); SetDiceSides(5);
		break;
	case MonsterType::Zombie:
		SetName("좀비");
		SetHP(150); SetAtk(2); SetDef(8); SetExp(60);
		SetRewards(130, 1, DiceID::D2_3, 8);
		SetDiceCount(4); SetDiceSides(3);
		break;
	case MonsterType::Orc:
		SetName("오크");
		SetHP(280); SetAtk(18); SetDef(12); SetExp(120);
		SetRewards(280, 1, DiceID::D3_8, 15);
		SetDiceCount(2); SetDiceSides(8);
		break;
	case MonsterType::Golem:
		SetName("골렘");
		SetHP(500); SetAtk(30); SetDef(50); SetExp(300);
		SetRewards(800, 1, DiceID::D2_4, 22);
		SetDiceCount(1); SetDiceSides(12);
		break;
	case MonsterType::Troll:
		SetName("트롤");
		SetRewards(250, 1, DiceID::D3_8, 15);
		SetHP(230); SetAtk(15); SetDef(12); SetExp(100);
		SetDiceCount(2); SetDiceSides(8);
		break;
	case MonsterType::Dragon:
		SetName("레드 드래곤");
		SetHP(1200); SetAtk(40); SetDef(40); SetExp(800);
		SetRewards(1500, 1, DiceID::D3_8, 24);
		SetDiceCount(3); SetDiceSides(10);
		break;
	case MonsterType::MaxRabbit:
		SetName("만렙토끼");
		SetRewards(2000, 1, DiceID::D3_6, 25);
		SetHP(888); SetAtk(35); SetDef(50); SetExp(1500);
		SetDiceCount(10); SetDiceSides(6);
		break;
	case MonsterType::Mermaid:
		SetName("인어");
		SetHP(320); SetAtk(10); SetDef(20); SetExp(150);
		SetRewards(450, 1, DiceID::D2_4, 16);
		SetDiceCount(4); SetDiceSides(4);
		break;
	case MonsterType::Kraken:
		SetName("심해 크라켄");
		SetHP(450); SetAtk(12); SetDef(25); SetExp(200);
		SetRewards(600, 1, DiceID::D3_6, 20);
		SetDiceCount(6); SetDiceSides(4);
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