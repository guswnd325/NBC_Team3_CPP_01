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
		SetName("ΩΩ∂Û¿”");
		SetHP(20); SetAtk(5); SetDef(0); SetExp(10); SetGold(5); 
		SetRewards(20, 1, 101, 4);
		SetDiceCount(1); SetDiceSides(6);
		break;
	case MonsterType::Goblin:
		SetName("∞Ì∫Ì∏∞");
		SetHP(40); SetAtk(10); SetDef(5); SetExp(20); SetGold(10);
		SetRewards(50, 1, 102, 7);
		SetDiceCount(2); SetDiceSides(4);
		break;
	case MonsterType::Golem:
		SetName("∞Ò∑Ω");
		SetHP(150); SetAtk(25); SetDef(20); SetExp(100);
		SetRewards(150, 1, 105, 10);
		SetDiceCount(1); SetDiceSides(12);
		break;
	case MonsterType::Troll:
		SetHP(80); SetAtk(20); SetDef(15); SetExp(40); SetGold(30);
		SetRewards(200, 1, 105, 10);
		SetDiceCount(2); SetDiceSides(6);
		break;
	case MonsterType::MaxRabbit:
		SetName("∏∏∑æ≈‰≥¢");
		SetHP(500); SetAtk(60); SetDef(40); SetExp(500);
		SetRewards(1000, 1, 999, 15);
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

	std::cout << GetName() << "¿Ã(∞°) ¡÷ªÁ¿ß " << diceCount << "∞≥∏¶ ±º∏≥¥œ¥Ÿ: ";

	for (int i = 0; i < diceCount; ++i) {
		int roll = dis(gen);
		total += roll;
		std::cout << "[" << roll << "] ";
	}

	std::cout << " (√—«’: " << total << ")" << std::endl;
	return total;
}

void Monster::SetRewards(int gold, int recover, int diceID, int challenge) {
	rewardGold = gold;
	recoverAmount = recover;
	advancedDiceID = diceID;
	diceChallengeValue = challenge;
}