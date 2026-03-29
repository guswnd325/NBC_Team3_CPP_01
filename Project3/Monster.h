#pragma once
#include "Actor.h"
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
	Troll,
	Dragon
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

	int advancedDiceID;
	int diceChallengeValue;

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

	void SetRewards(int gold, int recover, int diceID, int challenge);
	int GetRewardGold() { return rewardGold; }
	int GetRecoverAmount() { return recoverAmount; }
	int GetAdvancedDiceID() { return advancedDiceID; }
	int GetDiceChallengeValue() { return diceChallengeValue; }

	int RollAttackDice();
	void InitializeMonster();
	const std::vector<std::string>& GetVisual() const { return visual; }
};

