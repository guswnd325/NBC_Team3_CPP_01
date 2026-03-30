#pragma once
#include <iostream>
#include "Character.h" 
#include "DiceManager.h"
#include "Tools.h" 
#include "Renderer.h"
using namespace std;

class GameManager;

constexpr int upgradeUnit = 1;

enum class UpgradeType
{
    Min,
    Max,
};

enum class UpgradeResult
{
    Success,
    TicketInsufficient,
    MaxUpgrade,
};

enum class HealResult
{
    Success,
    MaxHP,
};

// Base Class
class RestManager
{
private:
    DiceManager diceManager;
public:
    RestManager();
    ~RestManager();
    
    UpgradeResult UpgradeDice(int index, UpgradeType type, Character* character);

    void Rest(Character* character);
    void Run();
};