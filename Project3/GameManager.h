#pragma once
#include "Character.h"

#include "MonsterManager.h"
#include "CombatManager.h"
#include "ShopManager.h"
#include "RestManager.h"

class GameManager
{
private:
    Character* player;

    MonsterManager* monsterManager;
    CombatManager* combatManager;
    ShopManager* shopManager;
    RestManager* restManager;

private:
    GameManager();
    ~GameManager();

public:
    static GameManager& GetInstance();

    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
    
    Character* GetCharacter() const;
public:
    void Run();

private:
    void CreateCharacter();
    void StartGame();
};