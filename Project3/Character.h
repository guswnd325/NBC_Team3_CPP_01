#pragma once
#include <string>
#include "Actor.h"

class Character : public Actor
{
public:
	Character();
	Character(std::string job);
};

