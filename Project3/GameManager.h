#pragma once
#include "Character.h"

#include "MonsterManager.h"
#include "CombatManager.h"
#include "ShopManager.h"
#include "RefurbishManager.h"
#include "AudioManager.h"

enum class Player
{

};

class Renderer;

class GameManager
{
private:
    Character* player;

    MonsterManager* monsterManager;
    CombatManager* combatManager;
    ShopManager* shopManager;
    RefurbishManager* refurbishManager;

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