#pragma once
#include <string>

constexpr int MAX_HP = 100;

class Actor
{
private:
	std::string name;
	int hp, gold, atk, def, exp;
public:
	void SetName(std::string name) { this -> name = name; }
	std::string GetName() { return name; }
	
	bool IsDead() { return hp <= 0; }

	void SetHP(int value) { hp = value; }
	int GetHP() { return hp; }
	
	void SetGold(int value) { gold = value; }
	int GetGold() { return gold; }

	void SetAtk(int value) { atk = value; }
	int GetAtk() { return atk; }

	void SetDef(int value) { def = value; }
	int GetDef() { return def; }

	void SetExp(int value) { exp = value; }
	int GetExp() { return exp; }
};