#pragma once
#include "Character.h"
#include "Monster.h"
#include "RefurbishStatus.h"
#include <Windows.h>
#include <unordered_map>

class Renderer
{
public:

    void RenderMenu();                            // 메뉴 출력
    void RenderRestMenu(); // 휴식소 메인 메뉴
    void RenderCreatePlayer();
    void RenderMainMenu();
    void RenderRewardSelect();
    void RenderBattleAction();
    void Clear();                                 // 화면 초기화
    void Delay(int ms);
    void PrintTyping(const std::string& text, int speed = 30);

    static int GetVisualLength(const std::string& str);
    static void PrintCenterLine(const std::string& text, int width, std::string color = "\033[37m");
    static void PrintLeftLine(const std::string& text, int width, std::string color = "\033[37m");

    static void PrintTop(int width);
    static void PrintDivider(int width);
    static void PrintBottom(int width);

public:
    Renderer();
    ~Renderer();

    void RenderAreaChoices(const std::vector<std::string>& choices, const std::unordered_map<std::string, std::string>& displayMap);
    void RenderDiceUpgradeList(const std::vector<DiceSlot>& storage); // 강화 가능 주사위 목록
    void RenderHealResult(int healValue, int prevHP, int curHP); // 회복 결과 출력
    void RenderBattleStart(Monster* monster);  // 전투 시작 화면 출력
    void RenderUpgradeResult(UpgradeStatus status, int prevLevel, int curLevel); // 강화 결과 출력
    void RenderStatus(Character* player);   // 플레이어 상태 출력
    //void RenderBattleLog(const string& message);  // 전투 로그 출력

public:
    static Renderer& GetInstance();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
};

