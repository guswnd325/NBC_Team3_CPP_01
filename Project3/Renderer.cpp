#include "Renderer.h"
#include "BaseItem.h"
#include <iostream>
#include <iomanip>

#define GOLD    "\033[38;2;218;165;32m"
#define BRIGHT_GREEN "\033[92m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"
#define CYAN        "\033[36m"
#define GRAY        "\033[90m"

Renderer::Renderer() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    // 콘솔 창 크기 고정 (UI_WIDTH에 맞춰 설정 추천)
    // system("mode con cols=110 lines=35"); 
}

Renderer::~Renderer() {}

Renderer& Renderer::GetInstance() {
    static Renderer instance;
    return instance;
}

void Renderer::MoveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

void Renderer::ClearZone(int startY, int height) {
    for (int i = 0; i < height; i++) {
        MoveCursor(0, startY + i);
        std::cout << std::string(Renderer::UI_WIDTH + 2, ' ') << std::flush;
    }
}

void Renderer::RenderLog(const std::string& message) {
    MoveCursor(0, Renderer::ZONE_LOG_Y);
    PrintTop(Renderer::UI_WIDTH);
    PrintLeftLine(" [LOG] " + message, Renderer::UI_WIDTH, GRAY);
    PrintBottom(Renderer::UI_WIDTH);
}

void Renderer::AddSystemLog(std::string msg) {
    systemLogs.push_back(msg);

    // 설정한 최대 로그 줄 수(예: 4줄)를 넘으면 가장 오래된 것 삭제
    if (systemLogs.size() > 4) {
        systemLogs.erase(systemLogs.begin());
    }
}

void Renderer::ClearSystemLogs() {
    systemLogs.clear();
}
int Renderer::GetVisualLength(const std::string& str) {
    int length = 0;
    for (int i = 0; i < str.length(); i++) {
        // 멀티바이트(한글) 체크
        if ((unsigned char)str[i] >= 0x80) {
            length += 2;
            i += 1; // EUC-KR 기준 (UTF-8 환경이면 시스템에 따라 조정 필요)
        }
        else {
            length += 1;
        }
    }
    return length;
}

// --- 테두리 및 라인 출력 (Renderer 멤버 함수로 통합) ---

void Renderer::PrintTop(int width) {
    std::cout << GOLD << "┌" << std::string(width - 2, '-') << "┐" << RESET << std::endl;
}

void Renderer::PrintDivider(int width) {
    std::cout << GOLD << "├" << std::string(width - 2, '-') << "┤" << RESET << std::endl;
}

void Renderer::PrintBottom(int width) {
    std::cout << GOLD << "└" << std::string(width - 2, '-') << "┘" << RESET << std::endl;
}

void Renderer::PrintCenterLine(const std::string& text, int width, std::string color) {
    int vLen = GetVisualLength(text);
    int padding = (width - 2 - vLen) / 2;
    int extra = (width - 2 - vLen) % 2;

    if (padding < 0) padding = 0;

    std::cout << GOLD << "│" << RESET << std::string(padding, ' ') << color << text << RESET << std::string(padding + extra, ' ') << GOLD << "│" << RESET << std::endl;
}

void Renderer::PrintLeftLine(const std::string& text, int width, std::string color) {
    int vLen = GetVisualLength(text);
    // "│  " (3칸) + 글자 + "  │" 형태를 맞추기 위한 패딩 계산
    int padding = width - 2 - vLen - 2;

    if (padding < 0) padding = 0;

    std::cout << GOLD << "│  " << RESET << color << text << RESET << std::string(padding, ' ') << GOLD << "│" << RESET << std::endl;
}
void Renderer::RenderMenu() {
    // 1. 전체 화면 초기화 및 시작 위치 고정
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);

    // 타이틀 ASCII ART
    PrintCenterLine(R"(      .-------.    ____  _ ____ ____      )", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(R"(     /   o   /|   |  _ \(_) ___|  __|      )", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(R"(    /  o    / o|  | | | | | |   | |__       )", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(R"(   /   o   / o |  | |_| | | |___|  __|      )", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(R"(  '-------' o /   |____/|_|\____|____|      )", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(R"(  | o     |  /      _   ____ ____ _  _ ___ )", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(R"(  |    o  | /      /_\ | __ |___ |\ |  |   )", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(R"(  | o     |/      /_  \|__] |___ | \|  |   )", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(R"(  '-------'                                 )", Renderer::UI_WIDTH, WHITE);

    PrintCenterLine("DICE ADVENTURE : THE ROGUELIKE", Renderer::UI_WIDTH, CYAN);

    for (int i = 0; i < 3; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // --- [하단 메뉴 선택지 구역] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("[1] 게임 시작          [2] 게임 종료", Renderer::UI_WIDTH, YELLOW);

    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [ 시스템 메시지 ]", Renderer::UI_WIDTH, GRAY);

    for (const auto& log : systemLogs) {
        PrintLeftLine(" >> " + log, Renderer::UI_WIDTH, WHITE);
    }
    // 로그 빈 줄 채우기 (높이 유지)
    for (int i = 0; i < (MAX_MENU_LOGS - (int)systemLogs.size()); i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }
    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > InPut : " << RESET;
}
void Renderer::RenderCreatePlayer() {
    // 1. 화면 전체 초기화
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [거대 박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 운명의 집행자 선택 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // 2. 상단 안내 구역 (질문을 먼저 던져서 동기 부여)
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("세계를 구할 마지막 희망이여,", Renderer::UI_WIDTH, GRAY);
    PrintCenterLine("당신의 운명을 결정할 집행자를 선택하십시오.", Renderer::UI_WIDTH, CYAN);
    PrintLeftLine("", Renderer::UI_WIDTH);

    PrintDivider(Renderer::UI_WIDTH); // 영역 구분

    // 3. 캐릭터 선택 목록 (중앙 집중 배치)
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintLeftLine("  [1] 전  사  (안정형: 주사위 1 ~ 6  (3개))", Renderer::UI_WIDTH, YELLOW);
    PrintLeftLine("", Renderer::UI_WIDTH); // 캐릭터 사이 간격 추가로 가독성 확보
    PrintLeftLine("  [2] 마법사  (표준형: 주사위 2 ~ 6  (2개))", Renderer::UI_WIDTH, CYAN);
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintLeftLine("  [3] 모험가  (도박형: 주사위 1 ~ 24 (1개))", Renderer::UI_WIDTH, RED);

    for (int i = 0; i < 3; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [ 시스템 메시지 ]", Renderer::UI_WIDTH, GRAY);

    for (const auto& log : systemLogs) {
        PrintLeftLine(" >> " + log, Renderer::UI_WIDTH, WHITE);
    }
    // 로그 빈 줄 채우기 (높이 유지)
    for (int i = 0; i < (MAX_MENU_LOGS - (int)systemLogs.size()); i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }
    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 5. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 집행자 번호 입력 : " << RESET;
}

void Renderer::RenderMainMenu() {
    // 1. 화면 초기화 및 시작 위치 고정
    ClearZone(Renderer::ZONE_SCREEN_Y, 30);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 평화로운 마을 ]", Renderer::UI_WIDTH, CYAN);
    PrintDivider(Renderer::UI_WIDTH);

    // --- [상단: 마을 정보/풍경 구역] ---
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("   _ ^ _   ", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine("  / |_| \  ", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine(" |_______| ", Renderer::UI_WIDTH, WHITE); // 간단한 집 모양 ASCII
    PrintCenterLine("모닥불 소리와 새들의 지저귐이 들리는 평화로운 마을입니다.", Renderer::UI_WIDTH, GRAY);

    // --- [중간: 빈 공간/로그 구역 테두리 유지] ---
    // 로그가 찍히지 않아도 테두리(│)를 유지하며 하단까지 박스를 늘립니다.
    // 이 구간이 아까 어색해 보였던 그 빈 공간을 메워주는 역할입니다.
    for (int i = 0; i < 8; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }
   

    // --- [하단: 행동 선택지 구역] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine("[1] 탐  사 (지역 조사)    [2] 상  점 (아이템 매매)", Renderer::UI_WIDTH, YELLOW);
    PrintLeftLine("[3] 장비창 (인벤토리)     [4] 휴  식 (회복 및 강화)", Renderer::UI_WIDTH, YELLOW);

    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [ 시스템 메시지 ]", Renderer::UI_WIDTH, GRAY);

    for (const auto& log : systemLogs) {
        PrintLeftLine(" >> " + log, Renderer::UI_WIDTH, WHITE);
    }
    // 로그 빈 줄 채우기 (높이 유지)
    for (int i = 0; i < (MAX_MENU_LOGS - (int)systemLogs.size()); i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }
    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 입력 위치 고정 (박스 바깥 하단)
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 행동을 선택해라 : " << RESET;
}

void Renderer::RenderBattleAction(Monster* monster, Character* player) { // player 인자 추가
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("!!! BATTLE IN PROGRESS !!!", Renderer::UI_WIDTH, RED);
    PrintDivider(Renderer::UI_WIDTH);

    // 2. 상단: 몬스터 정보
    std::string mStats = " [ ENEMY ] " + monster->GetName() + " | HP: " + std::to_string(monster->GetHP());
    PrintCenterLine(mStats, Renderer::UI_WIDTH, YELLOW);

    PrintDivider(Renderer::UI_WIDTH);
    // 1. 몬스터 이미지 구역 (8줄 고정)
    const std::vector<std::string>& visual = monster->GetVisual();
    for (int i = 0; i < 8; i++) {
        if (i < (int)visual.size()) {
            int vLen = GetVisualLength(visual[i]);
            int leftPad = (Renderer::UI_WIDTH - 2 - vLen) / 2;
            int rightPad = Renderer::UI_WIDTH - 2 - vLen - leftPad;
            std::cout << GOLD << "│" << RESET << std::string(leftPad, ' ') << WHITE << visual[i] << RESET
                << std::string(rightPad, ' ') << GOLD << "│" << RESET << std::endl;
        }
        else {
            PrintLeftLine("", Renderer::UI_WIDTH);
        }
    }


    // 3. 중앙: 플레이어 상태 정보 (새로 추가)
    PrintDivider(Renderer::UI_WIDTH);
    std::string pStats = " [ PLAYER ] " + player->GetName() + " | HP: " + std::to_string(player->GetHP()) + " / " + "100";
    PrintCenterLine(pStats, Renderer::UI_WIDTH, BRIGHT_GREEN);

    // 4. 누적 전투 로그 구역
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [ 실시간 전투 로그 ]", Renderer::UI_WIDTH, GRAY);
    for (const auto& log : battleLogs) {
        PrintLeftLine(" >> " + log, Renderer::UI_WIDTH, WHITE);
    }
    for (int i = 0; i < (MAX_LOGS - (int)battleLogs.size()); i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // 5. 하단 선택지
    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("[1] 전 투            [2] 도 망", Renderer::UI_WIDTH, YELLOW);
    PrintBottom(Renderer::UI_WIDTH);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 행동 선택 : " << RESET;
}

void Renderer::RenderRewardSelect() {
    // 1. 전체 구역 초기화 (이전 전투 흔적 제거)
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ VICTORY ]", Renderer::UI_WIDTH, YELLOW);
    PrintDivider(Renderer::UI_WIDTH);

    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("전투에서 승리하여 값진 전리품을 발견했습니다!", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine("어떤 방식으로 보상을 챙기시겠습니까?", Renderer::UI_WIDTH, WHITE);
    PrintLeftLine("", Renderer::UI_WIDTH);

    for (int i = 0; i < 6; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [1] 일반 보상 (휴식권 1회 + 골드)", Renderer::UI_WIDTH, BRIGHT_GREEN);
    PrintLeftLine(" [2] 리스크 보상 (주사위로 고보상 도전)", Renderer::UI_WIDTH, RED);

    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintBottom(Renderer::UI_WIDTH);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 보상을 선택해라 : " << RESET;
}

void Renderer::RenderStatus(Character* player) {
    if (!player) return;

    ClearZone(Renderer::ZONE_SCREEN_Y, 16);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    std::string topInfo = " [ PLAYER STATUS ]           REST TICKET : " + std::to_string(player->GetRestTicket());
    PrintLeftLine(topInfo, Renderer::UI_WIDTH, WHITE);

    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" 닉네임 : " + player->GetName(), Renderer::UI_WIDTH, YELLOW);
    PrintLeftLine(" HP     : " + std::to_string(player->GetHP()) + " / " + std::to_string(MAX_HP), Renderer::UI_WIDTH, RED);
    PrintLeftLine(" ATK    : " + std::to_string(player->GetAtk()) + " | DEF : " + std::to_string(player->GetDef()), Renderer::UI_WIDTH, WHITE);
    PrintLeftLine(" 소지금 : " + std::to_string(player->GetGold()) + " G", Renderer::UI_WIDTH, GOLD);

    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [ 소지한 주사위 ]", Renderer::UI_WIDTH, CYAN);

    Inventory* inv = player->GetInventory();
    if (inv) {
        const auto& diceList = inv->GetDiceStorege();
        if (diceList.empty()) {
            PrintLeftLine("   (소지 중인 주사위가 없습니다.)", Renderer::UI_WIDTH, GRAY);
        }
        else {
            int count = 0;
            for (const auto& slot : diceList) {
                if (count >= 3) { PrintLeftLine("   ...", Renderer::UI_WIDTH, GRAY); break; }
                if (!slot.dice) continue;
                PrintLeftLine(" - " + slot.dice->DiceIdToString() + " x" + std::to_string(slot.count), Renderer::UI_WIDTH, CYAN);
                count++;
            }
        }
    }
    PrintBottom(Renderer::UI_WIDTH);
}

void Renderer::RenderAreaChoices(const std::vector<std::string>& choices, const std::unordered_map<std::string, std::string>& displayMap) {
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [상단] 타이틀
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 탐험 지역 선택 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // [중앙] 메인 콘텐츠 (지역 리스트)
    PrintLeftLine("", Renderer::UI_WIDTH);
    for (int i = 0; i < (int)choices.size(); ++i) {
        std::string korName = displayMap.count(choices[i]) ? displayMap.at(choices[i]) : choices[i];
        PrintLeftLine("    [" + std::to_string(i + 1) + "] " + korName, Renderer::UI_WIDTH, YELLOW);
    }

    // 콘텐츠 영역 높이 고정 (예: 6줄)
    for (int i = 0; i < (6 - (int)choices.size()); i++) PrintLeftLine("", Renderer::UI_WIDTH);

    // [하단] 로그 구역 (이미지 속 전투 로그와 동일한 구조)
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [ 시스템 메시지 ]", Renderer::UI_WIDTH, GRAY);

    for (const auto& log : systemLogs) {
        PrintLeftLine(" >> " + log, Renderer::UI_WIDTH, WHITE);
    }
    // 로그 빈 줄 채우기 (높이 유지)
    for (int i = 0; i < (MAX_MENU_LOGS - (int)systemLogs.size()); i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // [최하단] 조작 가이드
    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("[0] 마을로 돌아가기", Renderer::UI_WIDTH, RED);
    PrintBottom(Renderer::UI_WIDTH);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 지역 번호 입력 : " << RESET;
}

void Renderer::RenderRestMenu() {
    // 1. 화면 전체 구역 초기화 (충분한 높이 확보)
    ClearZone(Renderer::ZONE_SCREEN_Y, 30);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 캠프파이어 (휴식소) ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // --- [중앙: 캠프파이어 연출 구역] ---
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("  (  )  (  )  (  )  ", Renderer::UI_WIDTH, RED);
    PrintCenterLine("   )  (  )  (  (    ", Renderer::UI_WIDTH, YELLOW);
    PrintCenterLine("  [__________]  ", Renderer::UI_WIDTH, WHITE);
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("타오르는 불꽃이 지친 몸을 달래줍니다.", Renderer::UI_WIDTH, GRAY);

    // --- [중간: 빈 공간 테두리 유지] ---
    // 로그 구역만큼 박스를 아래로 늘려줍니다. (약 10줄 정도)
    for (int i = 0; i < 10; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // --- [하단: 휴식 옵션 선택지 구역] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [1] 주사위 강화 (티켓 소모)    [2] 체력 회복 (티켓 소모)", Renderer::UI_WIDTH, YELLOW);
    PrintLeftLine(" [0] 마을로 돌아가기", Renderer::UI_WIDTH, WHITE);

    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 2. 입력 위치 고정 (박스 외부 하단)
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 옵션을 선택 : " << RESET;
}

void Renderer::RenderDiceUpgradeList(const std::vector<DiceSlot>& storage) {
    // 1. 화면 전체 초기화 (잔상 완벽 제거)
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [거대 박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 주사위 강화소 ]", Renderer::UI_WIDTH, YELLOW);
    PrintDivider(Renderer::UI_WIDTH);

    // 2. 상단 안내 및 질문 (가독성을 위해 위로 올림)
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("어떤 주사위의 운명을 강화하시겠습니까?", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine("강화할 주사위를 신중하게 선택하십시오.", Renderer::UI_WIDTH, GRAY);
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintDivider(Renderer::UI_WIDTH);

    // 3. 주사위 목록 출력
    PrintLeftLine("", Renderer::UI_WIDTH);
    if (storage.empty()) {
        PrintCenterLine("(강화할 수 있는 주사위가 없습니다.)", Renderer::UI_WIDTH, GRAY);
        for (int i = 0; i < 7; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    }
    else {
        for (int i = 0; i < (int)storage.size(); i++) {
            // 최대 8개 출력 제한
            if (i >= 8) {
                PrintLeftLine("      ...", Renderer::UI_WIDTH, GRAY);
                break;
            }

            // [번호] 주사위이름 (보유량) 형식
            std::string diceName = storage[i].dice->DiceIdToString();
            std::string diceCount = " (" + std::to_string(storage[i].count) + "개)";

            // 들여쓰기를 넣어 리스트처럼 보이게 함
            std::string diceInfo = "    [" + std::to_string(i + 1) + "] " + diceName + diceCount;
            PrintLeftLine(diceInfo, Renderer::UI_WIDTH, CYAN);
        }

        // 목록 높이 고정 (박스가 찌그러지지 않게)
        int remaining = 8 - (int)storage.size();
        for (int i = 0; i < (remaining > 0 ? remaining : 0); i++) {
            PrintLeftLine("", Renderer::UI_WIDTH);
        }
    }

    // 4. 하단 취소 메뉴 및 마감
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine("  [0] 취소하고 돌아가기", Renderer::UI_WIDTH, RED);
    PrintBottom(Renderer::UI_WIDTH); // [박스 끝 - 여기서 딱 한 번 닫음]

    // 5. 입력 위치 고정 (박스 외부 하단)
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 강화할 주사위 번호 입력 : " << RESET;
}

void Renderer::RenderHealResult(int healValue, int prevHP, int curHP, int maxHP) {
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 휴식 및 치료 완료 ]", Renderer::UI_WIDTH, BRIGHT_GREEN);
    PrintDivider(Renderer::UI_WIDTH);

    PrintLeftLine("", Renderer::UI_WIDTH);
    if (prevHP >= maxHP) {
        PrintCenterLine("이미 최상의 컨디션입니다!", Renderer::UI_WIDTH, YELLOW);
        PrintCenterLine("현재 HP: " + std::to_string(maxHP) + " / " + std::to_string(maxHP), Renderer::UI_WIDTH, WHITE);
    }
    else {
        PrintCenterLine("따뜻한 휴식으로 체력을 회복했습니다.", Renderer::UI_WIDTH, WHITE);
        PrintLeftLine("", Renderer::UI_WIDTH);

        std::string msg = "체력 [ +" + std::to_string(healValue) + " ] 회복 완료!";
        PrintCenterLine(msg, Renderer::UI_WIDTH, BRIGHT_GREEN);

        std::string hpMsg = "HP 변화: " + std::to_string(prevHP) + " - " + std::to_string(curHP) + " / " + std::to_string(maxHP);
        PrintCenterLine(hpMsg, Renderer::UI_WIDTH, WHITE);
    }

    // 높이 유지를 위한 공백 (전투 화면 등과 높이를 맞춤)
    for (int i = 0; i < 10; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("잠시 후 메뉴로 돌아갑니다...", Renderer::UI_WIDTH, GRAY);
    PrintBottom(Renderer::UI_WIDTH);

    Delay(1500);
}

void Renderer::RenderUpgradeResult(UpgradeStatus status, int prevLevel, int curLevel) {
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    if (status == UpgradeStatus::Success) {
        PrintCenterLine("[ 강 화 성 공 ]", Renderer::UI_WIDTH, YELLOW);
        PrintDivider(Renderer::UI_WIDTH);
        PrintLeftLine("", Renderer::UI_WIDTH);
        PrintCenterLine("주사위의 잠재력이 해방되었습니다!", Renderer::UI_WIDTH, BRIGHT_GREEN);
        PrintLeftLine("", Renderer::UI_WIDTH);

        std::string msg = "강화 등급: [ Lv." + std::to_string(prevLevel) + " ] >>> [ Lv." + std::to_string(curLevel) + " ]";
        PrintCenterLine(msg, Renderer::UI_WIDTH, WHITE);
    }
    else {
        PrintCenterLine("[ 강화 불가 ]", Renderer::UI_WIDTH, RED);
        PrintDivider(Renderer::UI_WIDTH);
        PrintLeftLine("", Renderer::UI_WIDTH);
        PrintCenterLine("이 주사위는 이미 한계치에 도달했습니다.", Renderer::UI_WIDTH, WHITE);
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    for (int i = 0; i < 11; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("대장장이가 장비를 정리합니다...", Renderer::UI_WIDTH, GRAY);
    PrintBottom(Renderer::UI_WIDTH);

    Delay(1500);
}

void Renderer::RenderTicketInsufficient() {
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("! 출입 제한 !", Renderer::UI_WIDTH, RED);
    PrintDivider(Renderer::UI_WIDTH);

    for (int i = 0; i < 6; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintCenterLine("필요한 [ 휴식권 ]이 부족합니다.", Renderer::UI_WIDTH, RED);
    PrintCenterLine("마을에서 준비를 더 마친 뒤에 다시 오십시오.", Renderer::UI_WIDTH, WHITE);

    for (int i = 0; i < 7; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintBottom(Renderer::UI_WIDTH);
    Delay(1500);
}

void Renderer::RenderDiceUpgradeOption() {
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 강화 경로 선택 ]", Renderer::UI_WIDTH, CYAN);
    PrintDivider(Renderer::UI_WIDTH);

    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("어떤 방향으로 주사위를 개조하시겠습니까?", Renderer::UI_WIDTH, WHITE);
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintDivider(Renderer::UI_WIDTH);

    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintLeftLine("  [1] 최소값 +1 증가 - 안정적인 최소 성능을 보장합니다.", Renderer::UI_WIDTH, YELLOW);
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintLeftLine("  [2] 최대값 +1 증가 - 더 높은 고점의 데미지를 노립니다.", Renderer::UI_WIDTH, RED);
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintLeftLine("  [0] 취소하고 돌아가기", Renderer::UI_WIDTH, GRAY);

    for (int i = 0; i < 6; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintBottom(Renderer::UI_WIDTH);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 옵션 선택 입력 : " << RESET;
}

void Renderer::RenderShopItemList(const std::vector<BaseItem*>& itemLists, int playerGold) {
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 만물상 상점 ]", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine("보유 골드: " + std::to_string(playerGold) + " G", Renderer::UI_WIDTH, GOLD);
    PrintDivider(Renderer::UI_WIDTH);

    // 헤더 부분도 동일한 간격으로 설정
    // [번호] 7칸, 이름 22칸, 타입 12칸, 가격순
    PrintLeftLine("  [번호]  아이템 이름           |    타입    |   가격", Renderer::UI_WIDTH, CYAN);
    PrintLeftLine("  -------------------------------------------------------------------------------------", Renderer::UI_WIDTH, GRAY);

    int maxDisplay = 10;
    for (int i = 0; i < (int)itemLists.size(); i++) {
        if (i >= maxDisplay) break;

        std::string name = itemLists[i]->GetName();
        std::string type = itemLists[i]->GetTypeToString(itemLists[i]->GetType());
        std::string price = std::to_string(itemLists[i]->GetPrice()) + " G";

        // 1. 이름 칸 정렬 (한글 폭 계산)
        int nameW = GetVisualLength(name);
        int nameTarget = 22; // 이름 칸 총 너비
        std::string namePadding = (nameTarget > nameW) ? std::string(nameTarget - nameW, ' ') : "";

        // 2. 타입 칸 정렬
        int typeW = GetVisualLength(type);
        int typeTarget = 10; // 타입 칸 총 너비
        std::string typePadding = (typeTarget > typeW) ? std::string(typeTarget - typeW, ' ') : "";

        // 3. 인덱스 번호 보정 ( [ 1] 형태 )
        std::string idxStr = (i + 1 < 10) ? " " + std::to_string(i + 1) : std::to_string(i + 1);

        // 최종 조립 (중간에 | 구분선 삽입)
        std::string info = "   [" + idxStr + "]  " + name + namePadding + " | "
            + type + typePadding + " | " + price;

        PrintLeftLine(info, Renderer::UI_WIDTH, WHITE);
    }

    // 빈 줄 채우기
    for (int i = 0; i < (maxDisplay - (int)itemLists.size()); i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [0] 마을로 돌아가기", Renderer::UI_WIDTH, RED);
    PrintBottom(Renderer::UI_WIDTH);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 구매 아이템 번호 입력 : " << RESET;
}

void Renderer::RenderBuyResult(BuyStatus status, BaseItem* item, int playerGold) {
    // 1. 화면 전체 초기화 (잔상 제거 및 일관된 배경 확보)
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 상점 이용 결과 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // 2. 결과 메시지 설정
    std::string message = "";
    std::string color = WHITE;
    std::string subMessage = "";

    switch (status) {
    case BuyStatus::Success:
        message = "[ 구매 성공! ]";
        if (item) subMessage = "[" + item->GetName() + "] (을)를 가방에 넣었습니다.";
        color = BRIGHT_GREEN;
        break;
    case BuyStatus::InsufficientGold:
        message = "!! 골드 부족 !!";
        if (item) subMessage = "필요 골드: " + std::to_string(item->GetPrice()) + " G (현재: " + std::to_string(playerGold) + " G)";
        color = RED;
        break;
    case BuyStatus::Possessed:
        message = "!! 중복 소유 !!";
        subMessage = "이미 가방에 동일한 아이템이 있습니다.";
        color = YELLOW;
        break;
    default:
        message = "!! 구매 실패 !!";
        subMessage = "알 수 없는 이유로 거래가 중단되었습니다.";
        color = RED;
        break;
    }

    // 3. 중앙 배치 연출
    for (int i = 0; i < 6; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintCenterLine(message, Renderer::UI_WIDTH, color);
    if (!subMessage.empty()) {
        PrintCenterLine(subMessage, Renderer::UI_WIDTH, WHITE);
    }

    // 높이 유지를 위한 하단 공백
    for (int i = 0; i < 7; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("잠시 후 상점 목록으로 돌아갑니다...", Renderer::UI_WIDTH, GRAY);
    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 4. 짧은 대기 후 복귀
    Delay(1500);
}

void Renderer::RenderInventory(BaseItem* slots[], const std::vector<ItemSlot>& gearStorage, const std::vector<DiceSlot>& diceStorage) {
    // 1. 화면 전체 구역 초기화 (인벤토리는 정보가 많으므로 높게!)
    ClearZone(Renderer::ZONE_SCREEN_Y, 35);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 인벤토리 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // --- [섹션 1: 장착 슬롯] ---
    PrintLeftLine(" [ 현재 장착 장비 ]", Renderer::UI_WIDTH, CYAN);
    const char* slotNames[] = { "무  기", "헬  멧", "갑  옷", "신  발", "장신구" };
    for (int i = 0; i < 5; i++) {
        std::string itemName = (slots[i] != nullptr) ? slots[i]->GetName() : "---";
        PrintLeftLine("  - " + std::string(slotNames[i]) + " : " + itemName, Renderer::UI_WIDTH, (slots[i] ? YELLOW : GRAY));
    }

    PrintDivider(Renderer::UI_WIDTH);

    // --- [섹션 2: 소지 장비 리스트] ---
    PrintLeftLine(" [ 소지 중인 장비 ]", Renderer::UI_WIDTH, CYAN);
    if (gearStorage.empty()) {
        PrintLeftLine("    (비어 있음)", Renderer::UI_WIDTH, GRAY);
    }
    else {
        for (int i = 0; i < std::min((int)gearStorage.size(), 6); i++) {
            std::string info = "  [" + std::to_string(i + 1) + "] " +
                gearStorage[i].item->GetName() +
                " (" + std::to_string(gearStorage[i].count) + "개)";
            PrintLeftLine(info, Renderer::UI_WIDTH, WHITE);
        }
    }

    PrintDivider(Renderer::UI_WIDTH);

    // --- [섹션 3: 소지 주사위 리스트] ---
    PrintLeftLine(" [ 소지 중인 주사위 ]", Renderer::UI_WIDTH, BRIGHT_GREEN);
    if (diceStorage.empty()) {
        PrintLeftLine("    (비어 있음)", Renderer::UI_WIDTH, GRAY);
    }
    else {
        std::string diceRow = "  ";
        for (const auto& ds : diceStorage) {
            diceRow += ds.dice->DiceIdToString() + "(" + std::to_string(ds.count) + ")  ";
        }
        PrintLeftLine(diceRow, Renderer::UI_WIDTH, WHITE);
    }

    // --- [중간: 박스 하단까지 연장] ---
    // 빈 공간을 테두리로 채워 "하단 메뉴"까지 연결합니다.
    int remaining = 4; // 남은 공간에 따라 조절 가능
    for (int i = 0; i < remaining; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // --- [하단: 조작 안내 구역] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [0] 마을로 돌아가기", Renderer::UI_WIDTH, RED);
    PrintCenterLine("장착할 장비의 번호를 입력해 주세요.", Renderer::UI_WIDTH, GRAY);

    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 2. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 장비 번호 선택 : " << RESET;
}

void Renderer::RenderEquipResult(const EquipResult& result) {
    // 1. 화면 전체 초기화 (잔상 및 쪼개진 박스 방지)
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 장비 변경 알림 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // 2. 메시지 및 색상 설정
    std::string mainMsg = "";
    std::string subMsg = "";
    std::string color = YELLOW;

    // 안전한 아이템 이름 처리
    std::string itemName = (result.item != nullptr) ? result.item->GetName() : "알 수 없는 아이템";

    switch (result.status) {
    case EquipStatus::Equip:
        mainMsg = "[ 장착 성공 ]";
        subMsg = "[" + itemName + "] (을)를 성공적으로 착용했습니다.";
        color = BRIGHT_GREEN;
        break;
    case EquipStatus::Changed:
        mainMsg = "[ 장비 교체 완료 ]";
        // 이전 장비와 새 장비의 대비를 시각적으로 표현
        subMsg = "[" + result.prevItem + "]  -->  [" + itemName + "]";
        color = CYAN;
        break;
    case EquipStatus::Overlap:
        mainMsg = "!! 이미 착용 중 !!";
        subMsg = "동일한 장비 [" + itemName + "] (을)를 이미 사용하고 있습니다.";
        color = RED;
        break;
    default:
        mainMsg = "!! 장착 불가 !!";
        subMsg = "해당 아이템을 장착하는 과정에서 오류가 발생했습니다.";
        color = GRAY;
        break;
    }

    // 3. 중앙 연출 구역
    for (int i = 0; i < 6; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintCenterLine(mainMsg, Renderer::UI_WIDTH, color);
    PrintLeftLine("", Renderer::UI_WIDTH); // 메시지 사이 간격
    PrintCenterLine(subMsg, Renderer::UI_WIDTH, WHITE);

    // 높이 유지를 위한 하단 공백
    for (int i = 0; i < 7; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("잠시 후 소지품 창으로 돌아갑니다...", Renderer::UI_WIDTH, GRAY);
    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 4. 결과 확인 대기
    Delay(1500);
}
void Renderer::Clear() { system("cls"); }
void Renderer::Delay(int ms) { Sleep(ms); }

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
