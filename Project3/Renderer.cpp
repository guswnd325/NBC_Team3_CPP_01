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

    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("DICE ADVENTURE : THE ROGUELIKE", Renderer::UI_WIDTH, CYAN);

    for (int i = 0; i < 11; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // --- [하단 메뉴 선택지 구역] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("[1] START GAME          [2] EXIT", Renderer::UI_WIDTH, YELLOW);

    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > InPut : " << RESET;
}
void Renderer::RenderCreatePlayer() {
    // 1. 전체 구역 초기화 (상단부터 하단까지 넉넉하게)
    ClearZone(Renderer::ZONE_SCREEN_Y, 30);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 캐릭터 집행자 선택 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // 정보 출력 구역
    PrintLeftLine("[1] 전  사  (안정형: 주사위 1 ~ 6  (3개) )", Renderer::UI_WIDTH, YELLOW);
    PrintLeftLine("[2] 마법사  (표준형: 주사위 2 ~ 6  (2개) )", Renderer::UI_WIDTH, CYAN);
    PrintLeftLine("[3] 모험가  (도박형: 주사위 1 ~ 24 (1개) )", Renderer::UI_WIDTH, RED);

    for (int i = 0; i < 12; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // --- [하단 안내 및 입력 구역 구분] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("당신의 운명을 결정할 집행자를 선택하십시오.", Renderer::UI_WIDTH, WHITE);
    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 커서를 박스 바로 아래나 특정 위치로 옮겨서 입력 받기
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 집행자를 선택해라 : " << RESET;
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
    for (int i = 0; i < 12; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // --- [하단: 행동 선택지 구역] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine("[1] 탐  사 (지역 조사)    [2] 상  점 (아이템 매매)", Renderer::UI_WIDTH, YELLOW);
    PrintLeftLine("[3] 장비창 (인벤토리)    [4] 휴  식 (회복 및 강화)", Renderer::UI_WIDTH, YELLOW);

    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 입력 위치 고정 (박스 바깥 하단)
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 행동을 선택해라 : " << RESET;
}


void Renderer::RenderBattleStart(Monster* monster) {
    Clear();
    MoveCursor(0, ZONE_SCREEN_Y);

    PrintTop(UI_WIDTH);
    PrintCenterLine("!!! BATTLE START !!!", UI_WIDTH, RED);
    PrintDivider(UI_WIDTH);

    // 몬스터 이미지 영역 (8줄 고정)
    const auto& visual = monster->GetVisual();
    for (int i = 0; i < 8; i++) {
        if (i < visual.size()) {
            int vLen = GetVisualLength(visual[i]);
            int pad = (UI_WIDTH - 2 - vLen) / 2;
            std::cout << GOLD << "│" << RESET << std::string(pad, ' ') << WHITE << visual[i] << RESET << std::string(UI_WIDTH - 2 - vLen - pad, ' ') << GOLD << "│" << RESET << std::endl;
        }
        else {
            PrintLeftLine("", UI_WIDTH);
        }
    }

    PrintDivider(UI_WIDTH);
    std::string stats = " 이름: " + monster->GetName() + " | HP: " + std::to_string(monster->GetHP()) + " | ATK: " + std::to_string(monster->GetAtk());
    PrintCenterLine(stats, UI_WIDTH, YELLOW);

    // 하단부 공간 미리 확보 (박스 테두리 유지)
    for (int i = 0; i < 10; i++) PrintLeftLine("", UI_WIDTH);
    PrintBottom(UI_WIDTH);
}


void Renderer::RenderBattleAction(Monster* monster) {
    // 1. 화면 초기화 및 시작 위치 고정
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("!!! BATTLE IN PROGRESS !!!", Renderer::UI_WIDTH, RED);
    PrintDivider(Renderer::UI_WIDTH);

    // --- [중요: 이 부분이 몬스터를 다시 그리는 곳입니다] ---
    const std::vector<std::string>& visual = monster->GetVisual();
    int monsterBoxHeight = 8; // 몬스터가 차지할 고정 높이

    for (int i = 0; i < monsterBoxHeight; i++) {
        if (i < (int)visual.size()) {
            // 중앙 정렬 출력
            int vLen = GetVisualLength(visual[i]);
            int leftPad = (Renderer::UI_WIDTH - 2 - vLen) / 2;
            int rightPad = Renderer::UI_WIDTH - 2 - vLen - leftPad;

            std::cout << GOLD << "│" << RESET
                << std::string(leftPad, ' ')
                << WHITE << visual[i] << RESET
                << std::string(rightPad, ' ')
                << GOLD << "│" << RESET << std::endl;
        }
        else {
            // 이미지가 없는 줄도 테두리(│)는 그려야 박스가 안 깨집니다.
            PrintLeftLine("", Renderer::UI_WIDTH);
        }
    }
    // ---------------------------------------------------

    PrintDivider(Renderer::UI_WIDTH);
    // 몬스터 정보 (실시간 체력 등)
    std::string stats = " 이름: " + monster->GetName() +
        " | HP: " + std::to_string(monster->GetHP()) +
        " | ATK: " + std::to_string(monster->GetAtk());
    PrintCenterLine(stats, Renderer::UI_WIDTH, YELLOW);

    // --- [중간: 누적 전투 로그 구역] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine(" [ 실시간 전투 로그 ]", Renderer::UI_WIDTH, GRAY);

    // battleLogs 벡터에 담긴 내용을 순서대로 출력
    for (const auto& log : battleLogs) {
        PrintLeftLine(" >> " + log, Renderer::UI_WIDTH, WHITE);
    }

    // 로그 창 높이 고정 (나머지 빈 줄 채우기)
    int emptyLogs = MAX_LOGS - (int)battleLogs.size();
    for (int i = 0; i < emptyLogs; i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    // --- [하단: 선택지 구역] ---
    PrintDivider(Renderer::UI_WIDTH);
    PrintCenterLine("[1] 전 투           [2] 도 망", Renderer::UI_WIDTH, YELLOW);
    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 입력 위치 고정
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

    // [박스 시작]
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 탐험 지역 선택 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // 1. 안내 및 질문 구역 (상단으로 전진 배치)
    PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("안전한 길은 없습니다. 당신의 직감을 믿으세요.", Renderer::UI_WIDTH, GRAY);
    PrintLeftLine("", Renderer::UI_WIDTH);

    PrintDivider(Renderer::UI_WIDTH); // 구분을 위해 선 하나 추가
    PrintCenterLine("어디로 이동하시겠습니까?", Renderer::UI_WIDTH, CYAN); // 질문을 목록 위로!
    PrintLeftLine("", Renderer::UI_WIDTH);

    // 2. 지역 목록 구역
    for (int i = 0; i < (int)choices.size(); ++i) {
        std::string korName = displayMap.count(choices[i]) ? displayMap.at(choices[i]) : choices[i];
        std::string choiceText = "    [" + std::to_string(i + 1) + "] " + korName;
        PrintLeftLine(choiceText, Renderer::UI_WIDTH, YELLOW);
    }

    // 3. 하단 여백 및 박스 마감
    // 목록 뒤에 빈 공간을 충분히 주어 시각적으로 안정감을 줍니다.
    int contentLines = (int)choices.size() + 7;
    int targetHeight = 18;
    for (int i = 0; i < (targetHeight - contentLines); i++) {
        PrintLeftLine("", Renderer::UI_WIDTH);
    }

    PrintBottom(Renderer::UI_WIDTH); // [박스 끝]

    // 4. 입력창 (박스 바로 아래)
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 이동할 지역 번호 입력 : " << RESET;
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
    // 1. 상단 구역(ZONE_SCREEN_Y)에 강화 가능한 주사위 리스트 출력
    ClearZone(Renderer::ZONE_SCREEN_Y, 16);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 주사위 강화소 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    if (storage.empty()) {
        PrintLeftLine(" (강화할 수 있는 주사위가 없습니다.)", Renderer::UI_WIDTH, GRAY);
        for (int i = 0; i < 6; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    }
    else {
        for (int i = 0; i < storage.size(); i++) {
            // 최대 출력 개수 제한 (화면 크기에 맞춰 8개 정도로 제한 추천)
            if (i >= 8) { PrintLeftLine("  ...", Renderer::UI_WIDTH, GRAY); break; }

            std::string diceInfo = "[" + std::to_string(i + 1) + "] " +
                storage[i].dice->DiceIdToString() +
                " (" + std::to_string(storage[i].count) + "개)";
            PrintLeftLine(diceInfo, Renderer::UI_WIDTH, CYAN);
        }
        // 높이 유지를 위한 빈 줄 채우기
        int remaining = 8 - (int)storage.size();
        for (int i = 0; i < (remaining > 0 ? remaining : 0); i++) PrintLeftLine("", Renderer::UI_WIDTH);
    }

    PrintDivider(Renderer::UI_WIDTH);
    PrintLeftLine("[0] 취소하고 돌아가기", Renderer::UI_WIDTH, WHITE);
    PrintBottom(Renderer::UI_WIDTH);

    // 2. 하단 입력 구역 (ZONE_PLAYER_Y)
    ClearZone(Renderer::ZONE_PLAYER_Y, 8);
    MoveCursor(0, Renderer::ZONE_PLAYER_Y);
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("강화할 주사위를 신중하게 선택하십시오.", Renderer::UI_WIDTH, WHITE);
    PrintBottom(Renderer::UI_WIDTH);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 주사위 번호 입력 : " << RESET;
}

void Renderer::RenderHealResult(int healValue, int prevHP, int curHP, int maxHP) {
    // 결과를 상단 구역에만 덮어쓰기 (전체 화면이 깜빡이지 않음)
    ClearZone(Renderer::ZONE_SCREEN_Y, 16);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 회복 결과 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    for (int i = 0; i < 2; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    if (prevHP >= maxHP) {
        PrintCenterLine("이미 체력이 가득 차 있습니다!", Renderer::UI_WIDTH, YELLOW);
        PrintCenterLine("현재 HP: " + std::to_string(maxHP) + " / " + std::to_string(maxHP), Renderer::UI_WIDTH, WHITE);
    }
    else {
        std::string msg = "체력이 " + std::to_string(healValue) + "만큼 회복되었습니다!";
        PrintCenterLine(msg, Renderer::UI_WIDTH, BRIGHT_GREEN);

        // HP 게이지 바 느낌의 시각적 효과 추가 가능
        std::string hpMsg = "HP: " + std::to_string(prevHP) + " -> " + std::to_string(curHP);
        PrintCenterLine(hpMsg, Renderer::UI_WIDTH, WHITE);

        if (curHP >= maxHP) {
            PrintCenterLine("[ FULL HP !! ]", Renderer::UI_WIDTH, CYAN);
        }
    }

    for (int i = 0; i < 3; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    PrintBottom(Renderer::UI_WIDTH);

    Delay(1500); // 2000은 조금 길게 느껴질 수 있어 1500 추천
}

void Renderer::RenderUpgradeResult(UpgradeStatus status, int prevLevel, int curLevel) {
    // 상단 구역에 강화 결과 연출
    ClearZone(Renderer::ZONE_SCREEN_Y, 16);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);

    if (status == UpgradeStatus::Success) {
        PrintCenterLine("[ 강화 성공! ]", Renderer::UI_WIDTH, YELLOW);
        PrintDivider(Renderer::UI_WIDTH);
        for (int i = 0; i < 3; i++) PrintLeftLine("", Renderer::UI_WIDTH);

        std::string msg = "강화 레벨: [Lv." + std::to_string(prevLevel) + "] >>> [Lv." + std::to_string(curLevel) + "]";
        PrintCenterLine(msg, Renderer::UI_WIDTH, WHITE);
        PrintCenterLine("주사위가 더욱 강력해졌습니다!", Renderer::UI_WIDTH, BRIGHT_GREEN);
    }
    else {
        PrintCenterLine("[ 강화 불가 ]", Renderer::UI_WIDTH, RED);
        PrintDivider(Renderer::UI_WIDTH);
        for (int i = 0; i < 4; i++) PrintLeftLine("", Renderer::UI_WIDTH);

        PrintCenterLine("이미 최대 강화 레벨에 도달했습니다.", Renderer::UI_WIDTH, WHITE);
    }

    for (int i = 0; i < 3; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    PrintBottom(Renderer::UI_WIDTH);

    Delay(1500);
}
void Renderer::RenderTicketInsufficient() {
    // 로그 구역이나 상단 구역에 잠깐 띄우기
    ClearZone(Renderer::ZONE_SCREEN_Y, 16);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);
    PrintTop(Renderer::UI_WIDTH);
    for (int i = 0; i < 5; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine("! 티켓이 부족합니다 !", Renderer::UI_WIDTH, RED);
    PrintCenterLine("휴식권이 있어야 해당 행동을 할 수 있습니다.", Renderer::UI_WIDTH, WHITE);
    for (int i = 0; i < 6; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    PrintBottom(Renderer::UI_WIDTH);
    Delay(1500);
}

void Renderer::RenderDiceUpgradeOption() {
    // 하단 입력 구역에 강화 옵션 고정
    ClearZone(Renderer::ZONE_PLAYER_Y, 8);
    MoveCursor(0, Renderer::ZONE_PLAYER_Y);
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 강화 옵션 선택 ]", Renderer::UI_WIDTH, WHITE);
    PrintLeftLine(" [1] 최소값 +1 증가    [2] 최대값 +1 증가", Renderer::UI_WIDTH, CYAN);
    PrintLeftLine(" [0] 취소하고 돌아가기", Renderer::UI_WIDTH, WHITE);
    PrintBottom(Renderer::UI_WIDTH);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 옵션 선택 : " << RESET;
}

void Renderer::RenderShopItemList(const std::vector<BaseItem*>& itemLists, int playerGold) {
    // 1. 상단 구역: 상품 리스트 (정보가 많으므로 넉넉히 사용)
    ClearZone(Renderer::ZONE_SCREEN_Y, 20); // 상점은 조금 더 넓게 사용
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 만물상 상점 ]", Renderer::UI_WIDTH, WHITE);
    PrintCenterLine("보유 골드: " + std::to_string(playerGold) + " G", Renderer::UI_WIDTH, GOLD);
    PrintDivider(Renderer::UI_WIDTH);

    for (int i = 0; i < itemLists.size(); i++) {
        if (i >= 10) break; // 최대 10개 출력
        std::string name = itemLists[i]->GetName();
        std::string type = itemLists[i]->GetTypeToString(itemLists[i]->GetType());
        std::string price = std::to_string(itemLists[i]->GetPrice()) + " G";

        // 이름 정렬을 위한 여백 계산
        int nameW = GetVisualLength(name);
        std::string namePart = name + std::string(std::max(0, 20 - nameW), ' ');

        std::string idxStr = (i + 1 < 10) ? " " + std::to_string(i + 1) : std::to_string(i + 1);
        std::string info = "[" + idxStr + "] " + namePart + " | " + type + " | " + price;
        PrintLeftLine(info, Renderer::UI_WIDTH, WHITE);
    }

    for (int i = 0; i < (10 - (int)itemLists.size()); i++) PrintLeftLine("", Renderer::UI_WIDTH);
    PrintBottom(Renderer::UI_WIDTH);

    // 2. 하단 입력 구역
    ClearZone(Renderer::ZONE_PLAYER_Y, 8);
    MoveCursor(0, Renderer::ZONE_PLAYER_Y);
    PrintTop(Renderer::UI_WIDTH);
    PrintLeftLine("[0] 마을로 돌아가기", Renderer::UI_WIDTH, RED);
    PrintBottom(Renderer::UI_WIDTH);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 4);
    std::cout << BRIGHT_GREEN << " > 구매할 아이템 번호 : " << RESET;
}

void Renderer::RenderBuyResult(BuyStatus status, BaseItem* item, int playerGold) {
    ClearZone(Renderer::ZONE_SCREEN_Y, 16);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);
    PrintTop(Renderer::UI_WIDTH);

    std::string message = "";
    std::string color = WHITE;

    switch (status) {
    case BuyStatus::Success:
        message = " [" + item->GetName() + "] 구매 성공! ";
        color = BRIGHT_GREEN;
        break;
    case BuyStatus::InsufficientGold:
        message = "!! 골드가 부족합니다 !!";
        color = RED;
        break;
    case BuyStatus::Possessed:
        message = "!! 이미 보유 중인 아이템입니다 !!";
        color = YELLOW;
        break;
    default:
        message = "구매할 수 없습니다.";
        color = RED;
        break;
    }

    for (int i = 0; i < 5; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    PrintCenterLine(message, Renderer::UI_WIDTH, color);
    if (status == BuyStatus::InsufficientGold && item) {
        PrintCenterLine("필요 골드: " + std::to_string(item->GetPrice()) + " G", Renderer::UI_WIDTH, WHITE);
    }
    for (int i = 0; i < 5; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    PrintBottom(Renderer::UI_WIDTH);

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
    const char* slotNames[] = { "무기", "헬멧", "갑옷", "신발", "장신구" };
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
    // 1. 상단 화면 구역(ZONE_SCREEN_Y)만 갱신하여 결과 출력
    ClearZone(Renderer::ZONE_SCREEN_Y, 16);
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);

    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ 장비 변경 알림 ]", Renderer::UI_WIDTH, WHITE);
    PrintDivider(Renderer::UI_WIDTH);

    // 내용물 출력을 위한 여백
    for (int i = 0; i < 4; i++) PrintLeftLine("", Renderer::UI_WIDTH);

    std::string message = "";
    std::string color = YELLOW;

    // 안전한 아이템 이름 가져오기
    std::string itemName = (result.item != nullptr) ? result.item->GetName() : "알 수 없는 아이템";

    switch (result.status) {
    case EquipStatus::Equip:
        message = " [" + itemName + "] 장착 완료! ";
        color = BRIGHT_GREEN;
        break;
    case EquipStatus::Changed:
        message = " [" + result.prevItem + "] >> [" + itemName + "] ";
        PrintCenterLine(message, Renderer::UI_WIDTH, YELLOW);
        message = "장비가 성공적으로 교체되었습니다.";
        color = WHITE;
        break;
    case EquipStatus::Overlap:
        message = "!! 이미 [" + itemName + "]을(를) 착용 중입니다 !!";
        color = RED;
        break;
    default:
        message = "장착할 수 없는 아이템이거나 오류가 발생했습니다.";
        color = GRAY;
        break;
    }

    PrintCenterLine(message, Renderer::UI_WIDTH, color);

    // 하단 여백 채우기
    for (int i = 0; i < 4; i++) PrintLeftLine("", Renderer::UI_WIDTH);
    PrintBottom(Renderer::UI_WIDTH);

    // 결과 확인을 위한 대기 (1.5초가 적당합니다)
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
