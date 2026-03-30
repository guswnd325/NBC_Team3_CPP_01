#pragma once
#include "Character.h"

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

    void RenderStatus(Character* player);   // 플레이어 상태 출력
    //void RenderBattleLog(const string& message);  // 전투 로그 출력

public:
    static Renderer& GetInstance();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

