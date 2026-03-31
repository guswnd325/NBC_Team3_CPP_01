#pragma once
#include "Character.h"

#include "MonsterManager.h"
#include "CombatManager.h"
#include "ShopManager.h"
#include "RefurbishManager.h"
#include "AudioManager.h"

enum class PlayerExist
{
    PlayerExistFalse,
    PlayerExistTrue
};


enum class GameState
{
    Running,
    Stoped
};

class Renderer;

class GameManager
{
private:
    Character* player;
    GameState state;
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
    void SetGameState(GameState state);
private:
    void CreateCharacter();
    void StartGame();
};