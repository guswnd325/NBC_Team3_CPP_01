#pragma once
class Renderer
{
public:
    void RenderMenu();                            // 메뉴 출력
    void RenderCreatePlayer();
    void RenderMainMenu();
    void Clear();                                 // 화면 초기화

public:
    //void RenderStatus(const Character* player);   // 플레이어 상태 출력
    //void RenderBattleLog(const string& message);  // 전투 로그 출력
};

