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
			R"(      .---.      )",
			R"(     /:::::(\     )",
			R"(    /:::::::::\    )",
			R"(   |::::0:::::0:|  )",
			R"(    \:::::::::/    )",
			R"(     '~~_~_~~'     )"
		};
		SetHP(40); SetAtk(2); SetDef(0); SetExp(15);
		SetRewards(30, 1, DiceID::D1_2, 8);
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
		SetRewards(60, 1, DiceID::D2_3, 12);
		SetDiceCount(2); SetDiceSides(4);
		break;
	case MonsterType::Ghost:
		SetName("유령");
		visual = {
			R"(    .-~~~-.     )",
			R"(   /  o o  \    )",
			R"(  | ^~~~~~~^ |  )",
			R"(   \  ~~~~  /   )",
			R"(   / \_  _/ \   )",
			R"(  '   ~~  ~~  ' )"
		};
		SetHP(80); SetAtk(8); SetDef(4); SetExp(45);
		SetRewards(90, 1, DiceID::D1_3, 20);
		SetDiceCount(2); SetDiceSides(5);
		break;
	case MonsterType::Zombie:
		SetName("좀비");
		visual = {
			R"(   _[_]_       )",
			R"(   (x x)       )",
			R"(  /|~~-~|\     )",
			R"( / |     | \   )",
			R"(   || | ||     )",
			R"(  _|| | ||_    )"
		};
		SetHP(150); SetAtk(2); SetDef(8); SetExp(60);
		SetRewards(130, 1, DiceID::D2_3, 16);
		SetDiceCount(4); SetDiceSides(3);
		break;
	case MonsterType::Orc:
		SetName("오크");
		visual = {
			R"(   .-------.   )",
			R"(  / @ .--. @\  )",
			R"( |   / \/ \  | )",
			R"(  \ | /__\ |/  )",
			R"(  /_\  ||  /_\ )",
			R"( /   \_||_/   \)"
		};
		SetHP(280); SetAtk(18); SetDef(12); SetExp(120);
		SetRewards(280, 1, DiceID::D3_8, 30);
		SetDiceCount(2); SetDiceSides(8);
		break;

	case MonsterType::Golem:
		SetName("골렘");
		visual = {
			R"( __[###]__     )",
			R"(|  [# #]  |   )",
			R"(|  [###]  |   )",
			R"(|##|   |##|   )",
			R"(|##|   |##|   )",
			R"(|__|   |__|   )"
		};
		SetHP(500); SetAtk(30); SetDef(50); SetExp(300);
		SetRewards(800, 1, DiceID::D2_4, 44);
		SetDiceCount(1); SetDiceSides(12);
		break;

	case MonsterType::Troll:
		SetName("트롤");
		visual = {
			R"(    _____      )",
			R"(   /o   o\     )",
			R"(  |  ___  |    )",
			R"(  /       \    )",
			R"( / |     | \   )",
			R"(/__|_   _|__\  )"
		};
		SetRewards(250, 1, DiceID::D3_8, 30);
		SetHP(230); SetAtk(15); SetDef(12); SetExp(100);
		SetDiceCount(2); SetDiceSides(8);
		break;

	case MonsterType::Dragon:
		SetName("레드 드래곤");
		visual = {
			R"( /\  .  .  /\  )",
			R"(/  \_\  /_/  \ )",
			R"(\ O  >  < O  / )",
			R"( \__/ \/ \__/  )",
			R"(   /|\/\/|\    )",
			R"(  / |    | \   )",
			R"( /__|    |__\  )",
			R"(    W    W     )"
		};
		SetHP(1200); SetAtk(40); SetDef(40); SetExp(800);
		SetRewards(1500, 1, DiceID::D3_8, 48);
		SetDiceCount(3); SetDiceSides(10);

		break;

	case MonsterType::MaxRabbit:
		SetName("만렙토끼");
		visual = {
			R"( /\ *** /\     )",
			R"((  \* */  )    )",
			R"( \ (o o) /     )",
			R"(  | -w- |      )",
			R"(  /|   |\      )",
			R"( (_|   |_)     )"
		};
		SetRewards(2000, 1, DiceID::D3_6, 50);
		SetHP(888); SetAtk(35); SetDef(50); SetExp(1500);
		SetDiceCount(10); SetDiceSides(6);
		break;

	case MonsterType::Mermaid:
		SetName("인어");
		visual = {
			R"(    _)_(        )",
			R"(   (o o)        )",
			R"(  /|=W=|\       )",
			R"( / |   | \      )",
			R"(   /~~~\        )",
			R"(  < ~~~ >       )",
			R"(   \~~~~/       )",
			R"(   >=|=<        )"
		};
		SetHP(320); SetAtk(10); SetDef(20); SetExp(150);
		SetRewards(450, 1, DiceID::D2_4, 32);
		SetDiceCount(4); SetDiceSides(4);
		break;

	case MonsterType::Kraken:
		SetName("심해 크라켄");
		visual = {
			R"( ~\_/~\_/~\_/~ )",
			R"(~/ 0       0 \~)",
			R"( |   _____   | )",
			R"(~\  /~~~~~\  /~)",
			R"( ~~| | | | |~~ )",
			R"(   | | | | |   )",
			R"( ~~' | | | '~~ )",
			R"(     ~ ~ ~     )"
		};
		SetHP(450); SetAtk(12); SetDef(25); SetExp(200);
		SetRewards(600, 1, DiceID::D3_6, 40);
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