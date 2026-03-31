#pragma once
#include "Character.h"
#include "Monster.h"
#include "RefurbishStatus.h"
#include "Inventory.h"
#include "buyStatus.h"
#include <vector>
#include <string>
#include <Windows.h>
#include <unordered_map>


#define GOLD         "\033[38;2;218;165;32m"
#define BRIGHT_GREEN "\033[92m"
#define WHITE        "\033[37m"
#define RED          "\033[31m"
#define RESET        "\033[0m"
#define YELLOW       "\033[33m"
#define CYAN         "\033[36m"
#define GRAY         "\033[90m"

class BaseItem;

class Renderer
{
public:
    std::vector<std::string> battleLogs;
    const int MAX_LOGS = 5;

public:
    static Renderer& GetInstance();

    void MoveCursor(int x, int y);
    void ClearZone(int startY, int height);
    void RenderLog(const std::string& message);

public:
    void PrintTop(int width);
    void PrintDivider(int width);
    void PrintBottom(int width);
    void PrintCenterLine(const std::string& text, int width, std::string color = WHITE);
    void PrintLeftLine(const std::string& text, int width, std::string color = WHITE);

public:
    void RenderMenu();
    void RenderRewardSelect();
    void RenderRestMenu();
    void RenderCreatePlayer();
    void RenderMainMenu();
    void RenderBattleStart(Monster* monster);
    void RenderBattleAction(Monster* monster);
    void RenderStatus(Character* player);
    void RenderShopItemList(const std::vector<BaseItem*>& itemLists, int playerGold);
    void RenderInventory(BaseItem* slots[], const std::vector<ItemSlot>& gearStorage, const std::vector<DiceSlot>& diceStorage);

public:
    void RenderBuyResult(BuyStatus status, BaseItem* item, int currentGold);
    void RenderAreaChoices(const std::vector<std::string>& choices, const std::unordered_map<std::string, std::string>& displayMap);
    void RenderDiceUpgradeList(const std::vector<DiceSlot>& storage);
    void RenderHealResult(int healValue, int prevHP, int curHP, int maxHP);
    void RenderUpgradeResult(UpgradeStatus status, int prevLevel, int curLevel);
    void RenderEquipResult(const EquipResult& result);
    void RenderDiceUpgradeOption();
    void RenderTicketInsufficient();

public:
    void Clear();
    void Delay(int ms);
    static int GetVisualLength(const std::string& str); // static 유지
    void PrintTyping(const std::string& text, int speed = 30);


public:
    Renderer();
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

public:
    // 레이아웃 상수 (클래스 내부에서 관리)
    static const int UI_WIDTH = 100;
    static const int ZONE_SCREEN_Y = 0;
    static const int ZONE_LOG_Y = 17;
    static const int ZONE_PLAYER_Y = 24;

    void AddBattleLog(const std::string& log) {
        battleLogs.push_back(log);
        // 설정한 줄 수보다 많아지면 가장 오래된 로그(맨 앞) 삭제
        if (battleLogs.size() > MAX_LOGS) {
            battleLogs.erase(battleLogs.begin());
        }
    }
    void ClearBattleLogs() { battleLogs.clear(); }
};


