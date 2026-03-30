#pragma once
#include "Character.h"
#include "Monster.h"

class Renderer
{
public:

    void RenderMenu();                            // 메뉴 출력
    void RenderCreatePlayer();
    void RenderMainMenu();
    void RenderRewardSelect();
	void RenderBattleAction();
    void Clear();                                 // 화면 초기화
    
public:
    Renderer();
    ~Renderer();

    void RenderBattleStart(Monster* monster);  // 전투 시작 화면 출력
    void RenderStatus(Character* player);   // 플레이어 상태 출력
    //void RenderBattleLog(const string& message);  // 전투 로그 출력

public:
    static Renderer& GetInstance();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

