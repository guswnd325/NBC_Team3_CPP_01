#pragma once
#include <iostream>
#include "Character.h" 
#include "DiceManager.h"
#include "Tools.h" 
#include "Renderer.h"
#include "RefurbishStatus.h"

using namespace std;

class GameManager;

constexpr int upgradeUnit = 1;


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