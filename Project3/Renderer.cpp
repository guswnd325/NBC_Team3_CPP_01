#include "Renderer.h"
#include "BaseItem.h"
#include <iostream>
#include <iomanip>

#define GOLD    "\033[38;2;218;165;32m"
#define BRIGHT_GREEN "\033[92m"
#define WHITE   "\033[37m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"
#define CYAN        "\033[36m"
#define GRAY        "\033[90m"


Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

Renderer& Renderer::GetInstance()
{
    static Renderer instance;
    return instance;
}

int Renderer::GetVisualLength(const std::string& str) {
    int length = 0;
    for (int i = 0; i < str.length(); i++) {
        if ((unsigned char)str[i] >= 0x80) {
            length += 2; i += 1; // 2바이트 문자 보정
        }
        else {
            length += 1;
        }
    }
    return length;
}

// 테두리 선 그리기 헬퍼
void PrintBorderLine(int width, const std::string& left, const std::string& mid, const std::string& right) {
    std::cout << GOLD << left;

    for (int i = 0; i < (width - 2); i++) std::cout << "-";

    std::cout << right << RESET << std::endl;
}

void Renderer::PrintTop(int width) { PrintBorderLine(width, "┌", "─", "┐"); }
void Renderer::PrintDivider(int width) { PrintBorderLine(width, "├", "─", "┤"); }
void Renderer::PrintBottom(int width) { PrintBorderLine(width, "└", "─", "┘"); }

void Renderer::PrintCenterLine(const std::string& text, int width, std::string color) {
    int vLen = GetVisualLength(text);
    int padding = (width - 2 - vLen) / 2;
    int extra = (width - 2 - vLen) % 2;
    std::cout << GOLD << "│" << RESET;
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << color << text << RESET;
    for (int i = 0; i < padding + extra; i++) std::cout << " ";
    std::cout << GOLD << "│" << RESET << std::endl;
}

// 5. 왼쪽 정렬 라인
void Renderer::PrintLeftLine(const std::string& text, int width, std::string color) {
    int vLen = GetVisualLength(text);
    int innerWidth = width - 2; // 테두리 제외 실제 안쪽 너비

    // '│  ' (테두리 1칸 + 공백 2칸 = 총 3칸)을 이미 썼으므로
    // 남은 공백은 (전체너비 - 테두리2칸 - 글자길이 - 왼쪽공백2칸) 입니다.
    int padding = innerWidth - vLen - 2;

    if (padding < 0) padding = 0; // 글자가 너무 길어 넘치는 것 방지

    std::cout << GOLD << "│  " << RESET << color << text << RESET;
    for (int i = 0; i < padding; i++) std::cout << " ";
    std::cout << GOLD << "│" << RESET << std::endl;
}
const int UI_WIDTH = 60;

void Renderer::RenderMenu() {
    PrintTop(UI_WIDTH);
    PrintCenterLine("Dice Agent", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[1] START GAME", UI_WIDTH, YELLOW);
    PrintLeftLine("[2] EXIT", UI_WIDTH, RED);
    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > InPut :" << RESET;
}

void Renderer::RenderCreatePlayer() {
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 캐릭터 집행자 선택 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[1] 전  사  (안정형: 주사위 2 ~ 4)", UI_WIDTH);
    PrintLeftLine("[2] 마법사  (표준형: 주사위 3 ~ 5)", UI_WIDTH);
    PrintLeftLine("[3] 모험가  (도박형: 주사위 1 ~ 8)", UI_WIDTH);
    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > 집행자를 선택해라 : " << RESET;
}

void Renderer::RenderMainMenu() {
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 현재 행동 선택 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[1] 탐  사 (새로운 지역을 조사합니다)", UI_WIDTH);
    PrintLeftLine("[2] 상  점 (아이템을 구매하거나 팝니다)", UI_WIDTH);
    PrintLeftLine("[3] 장비창 (인벤토리)", UI_WIDTH);
    PrintLeftLine("[4] 휴  식 (체력을 회복하고 재정비합니다)", UI_WIDTH);
    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > 행동을 선택해라 : " << RESET;
}

void Renderer::RenderRewardSelect() {
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 보상 선택 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[1] 일반 보상 (휴식권 1회 + 골드)", UI_WIDTH);
    PrintLeftLine("[2] 리스크 보상 (주사위로 고보상 도전)", UI_WIDTH);
    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > 보상을 선택해라 : " << RESET;
}

void Renderer::RenderBattleAction() {
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 행동 선택 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[1] 전투", UI_WIDTH);
    PrintLeftLine("[2] 도망", UI_WIDTH);
    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > 행동을 선택해라 : " << RESET;
}

void Renderer::RenderBattleStart(Monster* monster) {
    if (!monster) return;

    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 야생의 몬스터가 나타났다! ]", UI_WIDTH, RED);
    PrintDivider(UI_WIDTH);
    PrintLeftLine(" 이름 : " + monster->GetName(), UI_WIDTH);

    std::string stats = " HP: " + std::to_string(monster->GetHP()) +
        " | ATK: " + std::to_string(monster->GetAtk()) +
        " | DEF: " + std::to_string(monster->GetDef());
    PrintLeftLine(stats, UI_WIDTH, WHITE);
    PrintBottom(UI_WIDTH);

    std::cout << std::endl;
    for (const std::string& line : monster->GetVisual()) {
        std::cout << "      " << line << std::endl;
    }
    std::cout << std::endl;
}

void Renderer::RenderStatus(Character* player) {
    if (!player) return;

    PrintTop(UI_WIDTH);
    std::string topInfo = " [ PLAYER STATUS ]          REST TICKET : " + std::to_string(player->GetRestTicket());
    PrintLeftLine(topInfo, UI_WIDTH, WHITE);

    // 닉네임과 HP 분할 영역 (수동 조절이 필요한 특수 케이스)
    std::cout << GOLD << "├──────────────────────────────┬─────────────────────────────┤" << std::endl;

    std::string nameLine = " 닉네임 : " + player->GetName();
    std::string hpLine = " HP : " + std::to_string(player->GetHP()) + " / " + std::to_string(MAX_HP);

    std::cout << GOLD << "│" << WHITE << nameLine;
    for (int i = 0; i < 30 - GetVisualLength(nameLine); i++) std::cout << " ";
    std::cout << GOLD << "│" << WHITE << hpLine;
    for (int i = 0; i < 29 - GetVisualLength(hpLine); i++) std::cout << " ";
    std::cout << GOLD << "│" << RESET << std::endl;

    PrintDivider(UI_WIDTH);
    PrintLeftLine(" ATK : " + std::to_string(player->GetAtk()) + " | DEF : " + std::to_string(player->GetDef()), UI_WIDTH);
    PrintLeftLine(" 소지금 : " + std::to_string(player->GetGold()) + " G", UI_WIDTH, YELLOW);
    PrintDivider(UI_WIDTH);
    PrintLeftLine(" [ 소지한 주사위 ]", UI_WIDTH, CYAN);

    Inventory* inv = player->GetInventory();
    if (inv) {
        const auto& diceList = inv->GetDiceStorege();
        if (diceList.empty()) {
            PrintLeftLine("   (소지 중인 주사위가 없습니다.)", UI_WIDTH, GRAY);
        }
        else {
            for (const auto& slot : diceList) {
                if (!slot.dice) continue;
                std::string dName;
                switch (slot.dice->GetId()) {
                case DiceID::D1_2: dName = "1-2 주사위"; break;
                case DiceID::D1_3: dName = "1-3 주사위"; break;
                case DiceID::D1_4: dName = "1-4 주사위"; break;
                case DiceID::D1_6: dName = "1-6 주사위"; break;
                case DiceID::D1_8: dName = "1-8 주사위"; break;
                case DiceID::D2_3: dName = "2-3 주사위"; break;
                case DiceID::D2_5: dName = "2-5 주사위"; break;
                case DiceID::D3_6: dName = "3-6 주사위"; break;
                default: dName = "미지의 주사위"; break;
                }
                PrintLeftLine(" - " + dName + " x" + std::to_string(slot.count), UI_WIDTH, CYAN);
            }
        }
    }
    PrintBottom(UI_WIDTH);
}

void Renderer::RenderAreaChoices(const std::vector<std::string>& choices, const std::unordered_map<std::string, std::string>& displayMap) {

    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 탐험 지역 선택 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);

    PrintLeftLine("어디로 이동하시겠습니까?", UI_WIDTH, CYAN);
    PrintDivider(UI_WIDTH);

    for (int i = 0; i < choices.size(); ++i) {
        std::string engName = choices[i];
        std::string korName = engName;

        auto it = displayMap.find(engName);
        if (it != displayMap.end()) {
            korName = it->second;
        }

        std::string choiceText = " [" + std::to_string(i + 1) + "] " + korName;
        PrintLeftLine(choiceText, UI_WIDTH, YELLOW);
    }

    PrintBottom(UI_WIDTH);
}
void Renderer::RenderRestMenu() {
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 캠프파이어 (휴식소) ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[1] 주사위 강화 (티켓 소모)", UI_WIDTH, YELLOW);
    PrintLeftLine("[2] 체력 회복   (주사위 굴리기)", UI_WIDTH, BRIGHT_GREEN);
    PrintLeftLine("[0] 마을로 돌아가기", UI_WIDTH, WHITE);
    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > 옵션을 선택 : " << RESET;
}

void Renderer::RenderDiceUpgradeList(const std::vector<DiceSlot>& storage) {
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 강화할 주사위 선택 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);

    if (storage.empty()) {
        PrintLeftLine(" (강화할 수 있는 주사위가 없습니다.)", UI_WIDTH, GRAY);
    }
    else {
        for (int i = 0; i < storage.size(); i++) {
            std::string diceInfo = "[" + std::to_string(i + 1) + "] " +
                storage[i].dice->DiceIdToString() +
                " (" + std::to_string(storage[i].count) + "개)";
            PrintLeftLine(diceInfo, UI_WIDTH, CYAN);
        }
    }
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[0] 취소하고 돌아가기", UI_WIDTH, WHITE);
    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > 주사위 번호 입력 : " << RESET;
}

void Renderer::RenderHealResult(int healValue, int prevHP, int curHP) {
    Clear();
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 회복 결과 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);
    std::string msg = "체력이 " + std::to_string(healValue) + "만큼 회복되었습니다!";
    PrintCenterLine(msg, UI_WIDTH, BRIGHT_GREEN);
    std::string hpMsg = std::to_string(prevHP) + " -> " + std::to_string(curHP);
    PrintCenterLine(hpMsg, UI_WIDTH, WHITE);
    PrintBottom(UI_WIDTH);
    Delay(2000);
}

void Renderer::RenderUpgradeResult(UpgradeStatus status, int prevLevel, int curLevel) {
    Clear();
    PrintTop(UI_WIDTH);
    if (status == UpgradeStatus::Success) {
        PrintCenterLine("[ 강화 성공! ]", UI_WIDTH, YELLOW);
        PrintDivider(UI_WIDTH);
        std::string msg = "강화 레벨: " + std::to_string(prevLevel) + " -> " + std::to_string(curLevel);
        PrintCenterLine(msg, UI_WIDTH, WHITE);
    }
    else {
        PrintCenterLine("[ 강화 실패 ]", UI_WIDTH, RED);
        PrintDivider(UI_WIDTH);
        PrintCenterLine("이미 최대 강화 레벨입니다.", UI_WIDTH, WHITE);
    }
    PrintBottom(UI_WIDTH);
    Delay(2000);
}

void Renderer::RenderTicketInsufficient()
{
    Clear();
    PrintCenterLine("티켓이 부족합니다.", UI_WIDTH, WHITE);
    Delay(2000);
}

void Renderer::RenderDiceUpgradeOption() {
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 강화 옵션 선택 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);

    std::string message = "[1] 최소값 +1 증가 [2] 최대값 +1 증가";
    PrintLeftLine(message, UI_WIDTH, CYAN);
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[0] 취소하고 돌아가기", UI_WIDTH, WHITE);
    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > 강화 옵션 선택 : " << RESET;
}

void Renderer::RenderShopItemList(const std::vector<BaseItem*>& itemLists) {
    PrintTop(UI_WIDTH);
    PrintCenterLine("[ 만물상 상점 ]", UI_WIDTH, WHITE);
    PrintDivider(UI_WIDTH);
    PrintLeftLine("[0] 마을로 돌아가기", UI_WIDTH, RED);
    PrintDivider(UI_WIDTH);

    for (int i = 0; i < itemLists.size(); i++) {
        std::string info = "[" + std::to_string(i + 1) + "] " +
            itemLists[i]->GetName() + " | " +
            itemLists[i]->GetTypeToString(itemLists[i]->GetType()) + " | " +
            std::to_string(itemLists[i]->GetPrice()) + "G";

        PrintLeftLine(info, UI_WIDTH, YELLOW);
    }

    PrintBottom(UI_WIDTH);
    std::cout << BRIGHT_GREEN << " > 구매할 아이템 번호 입력 : " << RESET;
}

void Renderer::RenderBuyResult(BuyStatus status, BaseItem* item, int playerGold) {
    Clear();
    PrintTop(UI_WIDTH);

    std::string message = "";
    std::string color = WHITE;

    switch (status) {
    case BuyStatus::Success:
        message = "[" + item->GetName() + "] 구매 성공!";
        color = BRIGHT_GREEN;
        break;
    case BuyStatus::InsufficientGold:
    {
        int shortGold = item->GetPrice() - playerGold;
        message = "골드가 [" + std::to_string(shortGold) + "G] 부족합니다.";
        delete item;
        color = RED;
    }
    break;
    case BuyStatus::Possessed:
        message = "이미 보유 중인 아이템입니다.";
        delete item;
        color = YELLOW;
        break;
    default:
        message = "구매할 수 없습니다.";
        color = RED;
        break;
    }

    PrintCenterLine(message, UI_WIDTH, color);
    PrintBottom(UI_WIDTH);
    Delay(2000);
}

void Renderer::Clear()
{
    system("cls");
}

void Renderer::Delay(int ms) {
    Sleep(ms);
}

void Renderer::PrintTyping(const std::string& text, int speed) {
    for (int i = 0; i < text.length(); i++) {
        if ((unsigned char)text[i] >= 0x80) {
            std::cout << text[i] << text[i + 1] << std::flush;
            i++;
        }
        else {
            std::cout << text[i] << std::flush;
        }
        Sleep(speed);
    }
    std::cout << std::endl;
}
