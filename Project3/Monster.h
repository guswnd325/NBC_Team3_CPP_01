#pragma once
#include "Actor.h"
#include "DiceManager.h"
#include <vector>

enum class MonsterGrade {
	Normal,
	Elite,
	Boss
};
enum class MonsterType {
	Slime,
	Goblin,
	Orc,
	Golem,
	MaxRabbit,
	Mermaid,
	Kraken,
	Ghost,
	Troll,
	Dragon,
	Zombie
};
class Monster : public Actor{
private:
	MonsterType type;
	MonsterGrade grade;
	std::vector<std::string> visual;
	int rewardGold;
	int recoverAmount;
	int diceCount;
	int diceSides;

	DiceID rewardDiceID;
	int diceChallengeValue;

	int maxHp;

public:

	Monster(MonsterType type, MonsterGrade grade);
	int GetDiceCount() { return diceCount; }
	void SetDiceCount(int value) { diceCount = value; }

	int GetDiceSides() { return diceSides; }
	void SetDiceSides(int value) { diceSides = value; }

	MonsterType GetType() { return type; }
	void SetType(MonsterType value) { type = value; }

	MonsterGrade GetGrade() { return grade; }
	void SetGrade(MonsterGrade value) { grade = value; }

	void SetRewards(int gold, int recover, DiceID diceID, int challenge);
	int GetRewardGold() { return rewardGold; }
	int GetRecoverAmount() { return recoverAmount; }
	DiceID GetRewardDiceID() { return rewardDiceID; }
	int GetDiceChallengeValue() { return diceChallengeValue; }

	int RollAttackDice();
	void InitializeMonster();
	const std::vector<std::string>& GetVisual() const { return visual; }


	int GetMaxHp() { return maxHp; }
	void SetMaxHp(int value) { maxHp = value; }
};

