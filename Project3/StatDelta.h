#pragma once

struct StatDelta
{
public:
	int atk, def;

	StatDelta(int atk = 0, int def = 0)
	{
		this->atk = atk;
		this->def = def;
	}
};