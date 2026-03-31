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

#define GOLD          "\033[38;2;218;165;32m"
#define BRIGHT_GREEN  "\033[92m"
#define WHITE         "\033[37m"
#define RED           "\033[31m"
#define RESET         "\033[0m"
#define YELLOW        "\033[33m"
#define CYAN          "\033[36m"
#define GRAY          "\033[90m"

class BaseItem;

class Renderer
{
public:
    std::vector<std::string> battleLogs;
    const int MAX_LOGS = 5;

    std::vector<std::string> systemLogs;
    const int MAX_MENU_LOGS = 4;

public:
    static Renderer& GetInstance();

    void MoveCursor(int x, int y);
    void ClearZone(int startY, int height);
    void RenderLog(const std::string& message);
    void Clear();
    void Delay(int ms);
    static int GetVisualLength(const std::string& str);
    void PrintTyping(const std::string& text, int speed = 30);

public:
    void PrintTop(int width);
    void PrintDivider(int width);
    void PrintBottom(int width);
    void PrintCenterLine(const std::string& text, int width, std::string color = WHITE);
    void PrintLeftLine(const std::string& text, int width, std::string color = WHITE);

    // 공용 2분할 엔진
    void RenderSplitScreen(const std::vector<std::string>& leftContent,
        const std::vector<std::string>& diceFrame,
        const std::string& title,
        bool isBattleMode = false);

public:
    // [중요] 모든 주요 화면 출력 함수에 diceFrame 인자를 추가했습니다.
    void RenderMenu(const std::vector<std::string>& diceFrame);
    void RenderMainMenu(const std::vector<std::string>& diceFrame);
    void RenderBattleAction(Monster* monster, Character* player, const std::vector<std::string>& diceFrame);
    void RenderShopItemList(const std::vector<BaseItem*>& itemLists, int playerGold, const std::vector<std::string>& diceFrame);
    void RenderAreaChoices(const std::vector<std::string>& choices, const std::unordered_map<std::string, std::string>& displayMap, const std::vector<std::string>& diceFrame);
    void RenderInventory(BaseItem* slots[], const std::vector<ItemSlot>& gearStorage, const std::vector<DiceSlot>& diceStorage, const std::vector<std::string>& diceFrame);

    // [수정] 보상, 휴식, 생성 화면도 diceFrame을 받도록 변경
    void RenderRewardSelect(const std::vector<std::string>& diceFrame);
    void RenderRestMenu(const std::vector<std::string>& diceFrame);
    void RenderCreatePlayer(const std::vector<std::string>& diceFrame);

    // 결과창 및 단순 정보창 (필요에 따라 diceFrame 추가 가능)
    void RenderStatus(Character* player, const std::vector<std::string>& diceFrame);
    void RenderBuyResult(BuyStatus status, BaseItem* item, int currentGold);
    void RenderDiceUpgradeList(const std::vector<DiceSlot>& storage, const std::vector<std::string>& diceFrame);
    void RenderHealResult(int healValue, int prevHP, int curHP, int maxHP);
    void RenderUpgradeResult(UpgradeStatus status, int prevLevel, int curLevel);
    void RenderEquipResult(const EquipResult& result);
    void RenderDiceUpgradeOption(const std::vector<std::string>& diceFrame);
    void RenderTicketInsufficient();

public:
    Renderer();
    ~Renderer();
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

public:
    static const int UI_WIDTH = 100;
    static const int LEFT_WIDTH = 60;
    static const int RIGHT_WIDTH = 37;
    static const int CONTENT_HEIGHT = 15;

    static const int ZONE_SCREEN_Y = 0;
    static const int ZONE_LOG_Y = 17;
    static const int ZONE_PLAYER_Y = 24;

    void AddBattleLog(const std::string& log) {
        battleLogs.push_back(log);
        if (battleLogs.size() > MAX_LOGS) battleLogs.erase(battleLogs.begin());
    }
    void ClearBattleLogs() { battleLogs.clear(); }

    void AddSystemLog(std::string msg);
    void ClearSystemLogs();
};