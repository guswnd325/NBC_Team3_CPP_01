#pragma once

struct StatDelta
{
public:
	int hp, mp, atk, def, luck;

	StatDelta(int hp = 0, int mp = 0, int atk = 0, int def = 0, int luck = 0)
	{
		this->hp = hp;
		this->mp = mp;
		this->atk = atk;
		this->def = def;
		this->luck = luck;
	}
};