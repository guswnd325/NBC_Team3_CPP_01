#pragma once
#include <iostream>
#include "Character.h" 
#include "DiceManager.h"
#include "Tools.h" 
#include "Renderer.h"
using namespace std;

class GameManager;

constexpr int upgradeUnit = 1;

enum class RestOption
{
    Upgrade = 1,
    Heal
};

enum class UpgradeType
{
    Max,
    Min,
};

enum class UpgradeStatus
{
    Success,
    TicketInsufficient,
    MaxUpgrade,
};

enum class HealStatus
{
    Success,
    TicketInsufficient,
    MaxHP,
};

struct UpgradeResult
{
    UpgradeStatus status;
    int upgradeLevel;
};
struct RestResult
{
    HealStatus result;
    int healValue;
};


// Base Class
class RefurbishManager
{
private:
    DiceManager diceManager;
public:
    RefurbishManager();
    ~RefurbishManager();
    
    UpgradeResult UpgradeDice(int index, UpgradeType type, Character* character);

    RestResult Rest(Character* character);
    void Run();
};