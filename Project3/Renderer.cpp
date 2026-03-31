#include <iostream>
#include <iomanip>
#include "Renderer.h"
#include "CombatManager.h"
#include "BaseItem.h"

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


    system("mode con cols=100 lines=55");
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
        // UI_WIDTH보다 약간 넉넉하게 지워줍니다.
        std::cout << std::string(Renderer::UI_WIDTH + 5, ' ') << std::flush;
    }
}

// 간단한 한 줄 로그 출력 (기존 유지)
void Renderer::RenderLog(const std::string& message) {
    MoveCursor(0, Renderer::ZONE_LOG_Y);
    PrintTop(Renderer::UI_WIDTH);
    PrintLeftLine(" [LOG] " + message, Renderer::UI_WIDTH, GRAY);
    PrintBottom(Renderer::UI_WIDTH);
}

// 시스템 로그 추가 (기존 로직 유지)
void Renderer::AddSystemLog(std::string msg, std::string color) {
    systemLogs.push_back({msg, color});

    // 설정한 최대 로그 줄 수(4줄) 초과 시 삭제
    if (systemLogs.size() > MAX_MENU_LOGS) {
        systemLogs.erase(systemLogs.begin());
    }
}

void Renderer::ClearSystemLogs() {
    systemLogs.clear();
}

// 1. 시각적 길이 계산 (한글 2칸, 영문 1칸)
int Renderer::GetVisualLength(const std::string& str) {
    int length = 0;
    for (int i = 0; i < (int)str.length(); i++) {
        // ANSI ESCAPE CODE (\033[...m) 건너뛰기
        if (str[i] == '\033' && i + 1 < str.length() && str[i + 1] == '[') {
            while (i < str.length() && str[i] != 'm') i++;
            continue;
        }

        // 한글(멀티바이트) 처리 (2칸)
        if ((unsigned char)str[i] >= 0x80) {
            length += 2;
            i++;
        }
        // 일반 문자 (1칸)
        else {
            length += 1;
        }
    }
    return length;
}

// 2. 상단 테두리
void Renderer::PrintTop(int width) {
    // 특수문자 2칸을 제외한 나머지를 '-'로 채웁니다.
    std::cout << GOLD << "┌" << std::string(std::max(0, width - 2), '-') << "┐" << RESET << std::endl;
}

// 3. 중간 구분선
void Renderer::PrintDivider(int width) {
    std::cout << GOLD << "├" << std::string(std::max(0, width - 2), '-') << "┤" << RESET << std::endl;
}

// 4. 하단 테두리
void Renderer::PrintBottom(int width) {
    std::cout << GOLD << "└" << std::string(std::max(0, width - 2), '-') << "┘" << RESET << std::endl;
}

// 5. 중앙 정렬 라인
void Renderer::PrintCenterLine(const std::string& text, int width, std::string color) {
    int vLen = GetVisualLength(text);
    int totalPadding = width - 2 - vLen; // 양 끝 테두리 제외 가용 공간

    if (totalPadding < 0) totalPadding = 0;

    int padding = totalPadding / 2;
    int extra = totalPadding % 2;

    std::cout << GOLD << "│" << RESET
        << std::string(padding, ' ') << color << text << RESET
        << std::string(padding + extra, ' ') << GOLD << "│" << RESET << std::endl;
}

// 6. 왼쪽 정렬 라인
void Renderer::PrintLeftLine(const std::string& text, int width, std::string color) {
    int vLen = GetVisualLength(text);
    // "│  " (공백 포함 앞부분) + 글자 + 남은 공백 + "│"
    // 테두리(2) + 앞공백(2) = 4칸을 기본으로 제외합니다.
    int padding = width - 2 - vLen - 2;

    if (padding < 0) padding = 0;

    std::cout << GOLD << "│  " << RESET
        << color << text << RESET
        << std::string(padding, ' ') << GOLD << "│" << RESET << std::endl;
}

void Renderer::RenderSplitScreen(const std::vector<std::string>& leftContent,
    const std::vector<std::string>& diceFrame,
    const std::string& title,
    bool isBattleMode,
    std::string color)
{
    // 1. 화면 초기화 및 상단 타이틀
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine( "[ " + title + " ]", Renderer::UI_WIDTH, color);
    PrintDivider(Renderer::UI_WIDTH);

    // 2. 중앙 분할 구역 (고정 높이 사용으로 화면 떨림 방지)
    for (int i = 0; i < Renderer::CONTENT_HEIGHT; i++) {
        std::cout << GOLD << "│" << RESET;

        // [LEFT] 왼쪽 영역 (60칸)
        std::string lLine = (i < (int)leftContent.size()) ? leftContent[i] : "";
        int lLen = GetVisualLength(lLine);
        std::cout << " " << lLine << std::string(std::max(0, Renderer::LEFT_WIDTH - lLen - 1), ' ');

        // [CENTER DIVIDER] 중앙 세로선
        std::cout << GOLD << "│" << RESET;

        // [RIGHT] 오른쪽 영역 (37칸)
        std::string rLine = (i < (int)diceFrame.size()) ? diceFrame[i] : "";
        int rLen = GetVisualLength(rLine);
        std::cout << " " << rLine << std::string(std::max(0, Renderer::RIGHT_WIDTH - rLen - 1), ' ');

        std::cout << GOLD << "│" << RESET << std::endl;
    }

    // 3. 하단 로그 구역 구분선
    PrintDivider(Renderer::UI_WIDTH);

    // 4. 하단 로그 출력 (전투/시스템 모드 스위칭)
    if (isBattleMode) {
        // 1. 헤더 출력
        PrintLeftLine("[ 실시간 전투 로그 ]", Renderer::UI_WIDTH, GRAY);

        // 2. 개별 로그 출력 (각 로그가 가진 고유 색상 적용)
        for (const auto& log : battleLogs) {
            // log.color 변수를 PrintLeftLine의 세 번째 인자로 넘겨줍니다.
            PrintLeftLine(" >> " + log.message, Renderer::UI_WIDTH, log.color);
        }

        // 3. 빈 줄 채우기
        int remainingLines = MAX_LOGS - (int)battleLogs.size();
        for (int i = 0; i < remainingLines; i++) {
            PrintLeftLine("", Renderer::UI_WIDTH);
        }
    }
    else {
        // 시스템 메시지 영역 (보통은 흰색이나 회색으로 통일)
        PrintLeftLine("[ 시스템 메시지 ]", Renderer::UI_WIDTH, GRAY);
        for (const auto& log : systemLogs) {
            PrintLeftLine(" >> " + log.message, Renderer::UI_WIDTH, log.color);
        }

        int remainingLines = MAX_MENU_LOGS - (int)systemLogs.size();
        for (int i = 0; i < remainingLines; i++) {
            PrintLeftLine("", Renderer::UI_WIDTH);
        }
    }
    PrintBottom(Renderer::UI_WIDTH);
}

void Renderer::RenderSplitScreenSub(const std::vector<std::string>& leftContent,
    const std::vector<std::string>& diceFrame,
    const std::string& title,
    bool isBattleMode,
    std::string color)
{
    // 1. 화면 초기화 및 상단 타이틀
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ " + title + " ]", Renderer::UI_WIDTH, color);
    PrintDivider(Renderer::UI_WIDTH);

    // 2. 중앙 내용 구역 (고정 높이 CONTENT_HEIGHT 사용)
    for (int i = 0; i < Renderer::CONTENT_HEIGHT; i++) {
       
        std::cout << GOLD << "│" << RESET;

        std::string lLine = (i < (int)leftContent.size()) ? leftContent[i] : "";

        int lLen = GetVisualLength(lLine);

        std::cout << " " << lLine;

        int remainingSpace = Renderer::UI_WIDTH - 1 - 1 - lLen - 1;

        if (remainingSpace > 0) {
            std::cout << std::string(remainingSpace, ' ');
        }

        std::cout << GOLD << "│" << RESET << std::endl;
    }

   
    // 5. 최하단 테두리
    PrintBottom(Renderer::UI_WIDTH);
}
void Renderer::RenderMenu(const std::vector<std::string>& diceFrame) {
    std::vector<std::string> menuContent;
    int LW = Renderer::LEFT_WIDTH; // 60

    // 중앙 정렬용 람다 함수
    auto center = [&](std::string text) {
        int len = GetVisualLength(text);
        int space = (LW - len) / 2;
        return (space > 0) ? std::string(space, ' ') + text : text;
        };

    menuContent.push_back("");
    // 1. 타이틀 로고 중앙 정렬
    menuContent.push_back("");
    menuContent.push_back(center(std::string(BRIGHT_WHITE) + R"(      _______                                           )" + RESET));
    menuContent.push_back(center(std::string(BRIGHT_WHITE) + R"(     /\      \     _____  _____ _____ ______            )" + RESET));
    menuContent.push_back(center(std::string(BRIGHT_WHITE) + R"(    /  \   o  \   |  __ \|_   _/ ____|  ____|           )" + RESET));
    menuContent.push_back(center(std::string(BRIGHT_WHITE) + R"(   /    \______\  | |  | | | || |    | |__              )" + RESET));
    menuContent.push_back(center(std::string(BRIGHT_WHITE) + R"(   \    /      /  | |  | | | || |    |  __|             )" + RESET));
    menuContent.push_back(center(std::string(BRIGHT_WHITE) + R"(    \  /   o  /   | |__| |_| || |____| |____            )" + RESET));
    menuContent.push_back(center(std::string(BRIGHT_WHITE) + R"(     \/______/    |_____/|_____\_____|______|           )" + RESET));
    menuContent.push_back(center(R"(                                _  ____ ____ _  _ ___ )"));
    menuContent.push_back(center(R"(                                 /_\ | __ |___ |\ |  |    )"));
    menuContent.push_back(center(R"(                                /   \|__] |___ | \|  |    )"));
    menuContent.push_back(center(R"(                               )"));
    menuContent.push_back("");
    menuContent.push_back(""); // 로고와 버튼 사이 간격 추가

    std::string startBtn = std::string(BRIGHT_GREEN) + "[1] 게임 시작" + RESET;
    std::string exitBtn = std::string(RED) + "[2] 게임 종료" + RESET;
    menuContent.push_back(center(startBtn + "          " + exitBtn));


    std::vector<std::string> guideContent;
    int RW = 35; // 오른쪽 영역의 대략적인 폭 (환경에 따라 30~40 사이로 조절하세요)

    // 오른쪽 전용 중앙 정렬 람다
    auto centerRight = [&](std::string text) {
        int len = GetVisualLength(text);
        int space = (RW - len) / 2;
        return (space > 0) ? std::string(space, ' ') + text : text;
        };

    // 2. 오른쪽 영역 (게임 가이드 구성)
    guideContent.push_back("");
    guideContent.push_back(centerRight(std::string(BRIGHT_YELLOW) + "* 게임 가이드 *" + RESET));
    guideContent.push_back(centerRight(std::string(GRAY) + "----------------------------" + RESET));
    guideContent.push_back("");

    // 가이드 항목들 (해시태그 강조)
    guideContent.push_back(" " + std::string(BRIGHT_CYAN) + "탐사 |" + RESET + " 골드/경험치/주사위 획득");
    guideContent.push_back(" " + std::string(BRIGHT_GREEN) + "해금 |" + RESET + " 레벨업 시 새 지역 오픈");
    guideContent.push_back(" " + std::string(BRIGHT_YELLOW) + "강화 |" + RESET + " 주사위 눈 상향");
    guideContent.push_back(" " + std::string(BRIGHT_MAGENTA) + "회복 |" + RESET + " 주사위로 HP 회복");
    guideContent.push_back(" " + std::string(BRIGHT_RED) + "전투 |" + RESET + " 주사위 합 + 스탯 대결");
    guideContent.push_back(" " + std::string(BRIGHT_WHITE) + "보상 | " + RESET + "일반(안정) vs 고급(주사위)");

    guideContent.push_back("");
    guideContent.push_back(std::string(GRAY) + "   ----------------------------" + RESET);
    guideContent.push_back("   당신의 운을 믿고 나아가십시오");

    // 3. 엔진 호출 (diceFrame 대신 guideContent 전달)
    RenderSplitScreen(menuContent, guideContent, "DICE AGENT : THE ROGUELIKE", false);

    // 4. 입력 위치
    MoveCursor(0, 48);
    std::cout << BRIGHT_GREEN << " > 선택 : " << RESET;
}

void Renderer::RenderCreatePlayer(const std::vector<std::string>& diceFrame) {
    // 1. 왼쪽 영역에 들어갈 캐릭터 선택 정보 준비
    std::vector<std::string> charSelectionInfo;

    charSelectionInfo.push_back(" 세상을 구할 마지막 희망이여,");
    charSelectionInfo.push_back(" 당신의 운명을 결정할 집행자를 선택하십시오.");
    charSelectionInfo.push_back("");
    charSelectionInfo.push_back(" ------------------------------------------");
    charSelectionInfo.push_back("");

    // [1] 전사 - 안정적이고 단단한 느낌 (YELLOW)
    charSelectionInfo.push_back(std::string("  ") + YELLOW + "[1] 전  사" + RESET + "  (안정형: 주사위 1~6 (3개))");
    charSelectionInfo.push_back("      - 높은 안정성과 체력을 가진 표준 직업");
    charSelectionInfo.push_back("");

    // [2] 마법사 - 지적인 느낌 (CYAN)
    charSelectionInfo.push_back(std::string("  ") + CYAN + "[2] 마법사" + RESET + "  (표준형: 주사위 2~6 (2개))");
    charSelectionInfo.push_back("      - 강력한 한 방과 지능적인 플레이");
    charSelectionInfo.push_back("");

    // [3] 모험가 - 위험한 도박꾼 느낌 (RED)
    charSelectionInfo.push_back(std::string("  ") + RED + "[3] 도박꾼" + RESET + "  (도박형: 주사위 1~24 (1개))");
    charSelectionInfo.push_back("      - 운에 모든 것을 맡기는 극단적 재미");
    charSelectionInfo.push_back("");

    charSelectionInfo.push_back(" ------------------------------------------");

    std::vector<std::string> destinyDice = {
        "",
        std::string(BRIGHT_YELLOW) + "         < 운명의 주사위 >      " + RESET,
        "",
        std::string(WHITE) + "              _______          " + RESET,
        std::string(WHITE) + "             /      /|         " + RESET,
        std::string(WHITE) + "            /   o  / |         " + RESET,
        std::string(WHITE) + "           /______/  |         " + RESET,
        std::string(WHITE) + "           |      | o|         " + RESET,
        std::string(WHITE) + "           |  o   |  /         " + RESET,
        std::string(WHITE) + "           |______| /          " + RESET,
        "",
        std::string(GRAY) + "      당신의 행운을 믿으십시오.  " + RESET
    };

    // 3. 공용 엔진 호출 (제목: 운명의 집행자 선택)
    RenderSplitScreen(charSelectionInfo, destinyDice, "운명의 집행자 선택", false);

    // 4. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 집행자 번호 입력 : " << RESET;
}

void Renderer::RenderMainMenu(const std::vector<std::string>& diceFrame) {
    std::vector<std::string> townContent;
    int LW = Renderer::LEFT_WIDTH; // 60

    // 왼쪽 영역 중앙 정렬용 람다
    auto center = [&](std::string text) {
        int len = GetVisualLength(text);
        int space = (LW - len) / 2;
        if (space < 0) space = 0;
        return std::string(space, ' ') + text;
        };

    // 1. 왼쪽 영역 (마을 정보 및 메뉴)
    townContent.push_back("");
    townContent.push_back(center(std::string(BRIGHT_YELLOW) + "      _ ^ _      " + RESET));
    townContent.push_back(center(std::string(BRIGHT_YELLOW) + "     / |_| \\     " + RESET));
    townContent.push_back(center(std::string(BRIGHT_YELLOW) + "    |_______|    " + RESET));
    townContent.push_back("");

    townContent.push_back(center("모닥불 소리와 새들의 지저귐이 들리는"));
    townContent.push_back(center("평화로운 마을 " + std::string(BRIGHT_WHITE) + "'다이스톤'" + RESET + "입니다."));
    townContent.push_back("");
    townContent.push_back(center(std::string(GRAY) + "------------------------------------------" + RESET));
    townContent.push_back("");

    // 마을 메뉴 항목
    townContent.push_back(center(std::string(BRIGHT_GREEN) + "[1] 탐  사 (지역 조사)" + RESET));
    townContent.push_back(center(std::string(BRIGHT_YELLOW) + "  [2] 상  점 (아이템 구매)" + RESET));
    townContent.push_back(center(std::string(BRIGHT_CYAN) + " [3] 장비창 (인벤토리)  " + RESET));
    townContent.push_back(center(std::string(BRIGHT_MAGENTA) + "   [4] 휴  식 (회복 및 강화)" + RESET));

    townContent.push_back("");
    townContent.push_back(center(std::string(GRAY) + "------------------------------------------" + RESET));


    // 2. 오른쪽 영역 (게임 가이드 구성 - 중앙 정렬)
    std::vector<std::string> guideContent;
    int RW = 35; // 오른쪽 영역 폭 기준

    auto centerRight = [&](std::string text) {
        int len = GetVisualLength(text);
        int space = (RW - len) / 2;
        return (space > 0) ? std::string(space, ' ') + text : text;
        };

    guideContent.push_back("");
    guideContent.push_back(centerRight(std::string(BRIGHT_YELLOW) + "* 게임 가이드 *" + RESET));
    guideContent.push_back(centerRight(std::string(GRAY) + "--------------------------" + RESET));
    guideContent.push_back("");

    // 가이드 항목 (색상별 포인트)
    guideContent.push_back(" " + std::string(BRIGHT_CYAN) + "탐사 |" + RESET + " 골드/경험치/주사위 획득");
    guideContent.push_back(" " + std::string(BRIGHT_GREEN) + "해금 |" + RESET + " 레벨업 시 새 지역 오픈");
    guideContent.push_back(" " + std::string(BRIGHT_YELLOW) + "강화 |" + RESET + " 주사위 눈 상향");
    guideContent.push_back(" " + std::string(BRIGHT_MAGENTA) + "회복 |" + RESET + " 주사위로 HP 회복");
    guideContent.push_back(" " + std::string(BRIGHT_RED) + "전투 |" + RESET + " 주사위 합 + 스탯 대결");
    guideContent.push_back(" " + std::string(BRIGHT_WHITE) + "보상 | " + RESET + "일반(안정) vs 고급(주사위)");

    guideContent.push_back("");
    guideContent.push_back(centerRight(std::string(GRAY) + "--------------------------" + RESET));
    guideContent.push_back(centerRight("마을에서 전열을 가다듬으세요."));


    // 3. 공용 엔진 호출 (diceFrame 대신 guideContent 전달)
    RenderSplitScreen(townContent, guideContent, "평화로운 마을 : 다이스톤", false);

    // 4. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 행동 선택 : " << RESET;
}

void Renderer::RenderBattleAction(Monster* monster, Character* player, const std::vector<std::string>& diceFrame) {
    std::vector<std::string> battleContent;
    std::string divider = std::string(GRAY) + "--------------------------------------------------------" + RESET;

    // [상단] 몬스터 정보 (현재 HP / 최대 HP 표시)
    std::string mName = std::string(BRIGHT_RED) + " [ MONSTER ] " + monster->GetName() + RESET;

    // 헤더의 GetMaxHp()를 호출하여 출력
    std::string mStats = " 체력: " + std::string(RED) + std::to_string(monster->GetHP()) + " / " + std::to_string(monster->GetMaxHp()) + RESET +
        " | 공격력: " + std::string(RED) + std::to_string(monster->GetAtk()) + RESET +
        " | 방어력: " + std::string(BRIGHT_CYAN) + std::to_string(monster->GetDef()) + RESET;

    battleContent.push_back("");
    battleContent.push_back(mName);
    battleContent.push_back(mStats);
    battleContent.push_back(divider);

    // --- [동적 HP바 계산] ---
    int currentHp = monster->GetHP();
    int maxHp = monster->GetMaxHp();

    int barWidth = 15;
    int filledWidth = 0;

    if (maxHp > 0) {
        if (currentHp <= 0)
        {
            filledWidth = 0;
        }
        else
        {
            filledWidth = std::max(1,(currentHp * barWidth) / maxHp);
        }
    }
    // 안전장치
    if (filledWidth < 0) filledWidth = 0;
    if (filledWidth > barWidth) filledWidth = barWidth;

   
    std::string hpBar = ""; 

    for (int i = 0; i < barWidth; i++) {
        if (i < filledWidth) {
            // 찬 부분: 빨간색 적용
            hpBar += std::string(RED) + "=";
        }
        else {
            // 빈 부분: 회색 적용
            hpBar += std::string(GRAY) + "-";
        }
    }
    // 마지막에 모든 색상 초기화 (매우 중요!)
    hpBar += RESET;
    // ------------------------------------------------
    battleContent.push_back("");

    const std::vector<std::string>& visual = monster->GetVisual();
    for (int i = 0; i < 7; i++) {
        std::string line = " ";
        if (i < (int)visual.size()) {
            line += std::string(WHITE) + visual[i] + RESET;
        }
        else {
            line += "             ";
        }

        if (i == 3) {
            // 퍼센트 계산
            int percent = (maxHp > 0) ? (currentHp * 100 / maxHp) : 0;
            // 시각적 보정을 위해 뒤에 공백을 좀 더 줍니다.
            line += "  [HP] [" + hpBar + "] " + std::to_string(percent) + "%    ";
        }
        battleContent.push_back(line);
    }

    battleContent.push_back(divider);

    // [하단] 플레이어 정보 (기존 동일)
    std::string pName = std::string(BRIGHT_CYAN) + " [ PLAYER ] " + player->GetName() + RESET;
    std::string pStats = " 체력: " + std::string(BRIGHT_GREEN) + std::to_string(player->GetHP()) + RESET + " / 100" +
        " | 공격력: " + std::string(BRIGHT_RED) + std::to_string(player->GetAtk()) + RESET +
        " | 방어력: " + std::string(BRIGHT_CYAN) + std::to_string(player->GetDef()) + RESET;

    battleContent.push_back(pName);
    battleContent.push_back(pStats);
    battleContent.push_back("");

    std::string actions = std::string("  ") + BRIGHT_YELLOW + "          [1] 전 투" + RESET +
        "                " + BRIGHT_RED + "[2] 도 망" + RESET;
    battleContent.push_back(actions);

    RenderSplitScreen(battleContent, diceFrame, "탐사 : " + CombatManager::GetCurrentAreaName(), true);

    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 행동 선택 : " << RESET;
}

void Renderer::RenderRewardSelect(const std::vector<std::string>& diceFrame) {
    // 1. 왼쪽 영역: 승리 메시지 및 선택지 구성
    std::vector<std::string> rewardContent;
    std::string divider = std::string(GRAY) + " --------------------------------------------------------" + RESET;

    rewardContent.push_back("");
    // 승리 문구 - 화려한 노란색/금색 강조
    rewardContent.push_back(std::string(BRIGHT_YELLOW) + "  [ 승 리 ]" + RESET);
    rewardContent.push_back("  전투에서 승리하여 값진 전리품을 발견했습니다!");
    rewardContent.push_back("  어떤 방식으로 보상을 챙기시겠습니까?");
    rewardContent.push_back("");
    rewardContent.push_back(divider);
    rewardContent.push_back("");

    // [1] 일반 보상 - 하늘색 (안정, 휴식 느낌)
    rewardContent.push_back(std::string(BRIGHT_CYAN) + "  [1] 일반 보상 (안전)" + RESET);
    rewardContent.push_back(std::string(GRAY) + "      - 휴식권 1회 + 확정 골드 획득" + RESET);
    rewardContent.push_back("");

    // [2] 리스크 보상 - 주황/빨강 계열 (도전, 위험 느낌)
    rewardContent.push_back(std::string(BRIGHT_RED) + "  [2] 리스크 보상 (도전!)" + RESET);
    rewardContent.push_back(std::string(GRAY) + "      - 주사위를 굴려 더 큰 보상에 도전" + RESET);
    rewardContent.push_back("");

    rewardContent.push_back(divider);
    rewardContent.push_back("  " + std::string(BRIGHT_WHITE) + "당신의 운을 시험해 보십시오." + RESET);

    // 2. 공용 엔진 호출
    // 오른쪽(diceFrame)에는 현재 대기 중인 주사위나 보물상자 아트가 들어갑니다.
    RenderSplitScreen(rewardContent, diceFrame, "보상 선택", false);

    // 3. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 보상을 선택해라 : " << RESET;
}

void Renderer::RenderAreaChoices(const std::vector<std::string>& choices,
    const std::unordered_map<std::string, std::string>& displayMap,
    const std::vector<std::string>& diceFrame)
{
    // 1. 왼쪽 영역: 지역 리스트 구성
    std::vector<std::string> areaContent;
    std::string divider = std::string(GRAY) + " ------------------------------------------" + RESET;

    areaContent.push_back("");
    // 제목: 푸른색 계열로 탐험/지도 느낌 강조
    areaContent.push_back(std::string(BRIGHT_CYAN) + "  [ 탐험할 지역을 선택하십시오 ]" + RESET);
    areaContent.push_back(divider);
    areaContent.push_back("");

    for (int i = 0; i < (int)choices.size(); ++i) {
        // 한국어 이름이 있으면 가져오고, 없으면 영문 이름 사용
        std::string korName = displayMap.count(choices[i]) ? displayMap.at(choices[i]) : choices[i];

        // 번호는 초록색, 지역 이름은 밝은 노란색으로 강조
        std::string idx = std::string(BRIGHT_GREEN) + "[" + std::to_string(i + 1) + "]" + RESET;
        std::string areaName = std::string(BRIGHT_YELLOW) + korName + RESET;

        areaContent.push_back("    " + idx + " " + areaName);
        areaContent.push_back(""); // 가독성을 위해 한 줄 띔
    }

    areaContent.push_back(divider);
    // 마을 돌아가기 옵션
    areaContent.push_back("  " + std::string(WHITE) + "[0] 마을로 돌아가기" + RESET);

    // 2. 공용 엔진 호출
    // 오른쪽(diceFrame)에는 지도나 나침반 등이 들어갑니다.
    RenderSplitScreen(areaContent, diceFrame, "탐사 : 지역 선택", false);

    // 3. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 지역 번호 입력 : " << RESET;
}

void Renderer::RenderRestMenu(int restTicketCount, const std::vector<std::string>& diceFrame) {
    std::vector<std::string> restContent;
    int LW = Renderer::LEFT_WIDTH;

    // 중앙 정렬 람다
    auto center = [&](std::string text) {
        int len = GetVisualLength(text);
        int space = (LW - len) / 2;
        return (space > 0) ? std::string(space, ' ') + text : text;
        };

    restContent.push_back("");
    // 1. 모닥불 ASCII (중앙 정렬 유지)
    restContent.push_back(center(std::string(RED) + "(  )  " + YELLOW + "(  )  " + RED + "(  )"));
    restContent.push_back(center(std::string(YELLOW) + " )  (  )  (  ("));
    restContent.push_back(center(std::string(GRAY) + "[____________]"));
    restContent.push_back("");

    // 2. 설명 문구 및 휴식권 상태 표시
    restContent.push_back(center("타오르는 불꽃이 지친 몸을 달래줍니다."));

    // --- 휴식권 개수 표시 추가 ---
    std::string ticketInfo = "보유 휴식권 : " + std::string(BRIGHT_MAGENTA) + std::to_string(restTicketCount) + RESET + " 장";
    restContent.push_back(center(ticketInfo));
    // ----------------------------

    restContent.push_back("");
    restContent.push_back(center(std::string(GRAY) + "------------------------------------------" + RESET));
    restContent.push_back("");

    // 3. 메뉴 항목
    // [1] 주사위 강화
    restContent.push_back("  " + std::string(YELLOW) + "[1] 주사위 강화" + RESET + " (휴식권 1개 소모)");
    restContent.push_back(std::string(GRAY) + "      - 주사위의 잠재력을 이끌어냅니다." + RESET);
    restContent.push_back("");

    // [2] 체력 회복
    restContent.push_back("  " + std::string(BRIGHT_MAGENTA) + "[2] 체력 회복" + RESET + "   (휴식권 1개 소모)");
    restContent.push_back(std::string(GRAY) + "      - 따뜻한 온기로 체력을 보충합니다." + RESET);
    restContent.push_back("");

    // [0] 돌아가기
    restContent.push_back("  " + std::string(WHITE) + "[0] 마을로 돌아가기" + RESET);
    restContent.push_back("");
    restContent.push_back(center(std::string(GRAY) + "------------------------------------------" + RESET));

    // 4. 엔진 호출
    RenderSplitScreen(restContent, diceFrame, "캠프파이어 : 휴식 공간", false);

    // 5. 입력 위치
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 행동 선택 : " << RESET;
}

void Renderer::RenderDiceUpgradeList(const std::vector<DiceSlot>& storage, const std::vector<std::string>& diceFrame) {
    // 1. 왼쪽 영역: 주사위 강화 목록 데이터 구성
    std::vector<std::string> upgradeContent;
    std::string divider = std::string(GRAY) + " ------------------------------------------" + RESET;

    upgradeContent.push_back("");
    // 제목: 강화소의 신비로운 보라색 테마
    upgradeContent.push_back(std::string(BRIGHT_MAGENTA) + "  [ 주사위 강화소 ]" + RESET);
    upgradeContent.push_back("  어떤 주사위의 운명을 강화하시겠습니까?");
    upgradeContent.push_back(divider);
    upgradeContent.push_back("");

    if (storage.empty()) {
        upgradeContent.push_back(std::string(DARK_GRAY) + "    (강화할 수 있는 주사위가 없습니다.)" + RESET);
    }
    else {
        for (int i = 0; i < (int)storage.size(); i++) {
            // 최대 8개까지 출력
            if (i >= 8) {
                upgradeContent.push_back(std::string(GRAY) + "      ..." + RESET);
                break;
            }

            // 번호는 초록색, 이름은 노란색, 개수는 흰색으로 시각적 분리
            std::string idx = std::string(BRIGHT_GREEN) + "[" + std::to_string(i + 1) + "]" + RESET;
            std::string diceName = std::string(BRIGHT_YELLOW) + storage[i].dice->DiceIdToString() + RESET;
            std::string diceCount = std::string(WHITE) + " (" + std::to_string(storage[i].count) + "개)" + RESET;

            // 들여쓰기를 맞춰서 리스트 형식으로 저장
            upgradeContent.push_back("    " + idx + " " + diceName + diceCount);
        }
    }

    upgradeContent.push_back("");
    upgradeContent.push_back(divider);
    // 취소 옵션 강조
    upgradeContent.push_back("  " + std::string(WHITE) + "[0] 취소하고 돌아가기 (CANCEL)" + RESET);

    // 2. 공용 엔진 호출
    RenderSplitScreen(upgradeContent, diceFrame, "주사위 재련소", false);

    // 3. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 강화할 주사위 번호 입력 : " << RESET;
}

void Renderer::RenderHealResult(int healValue, int prevHP, int curHP, int maxHP, const std::vector<std::string>& diceFrame) {
    // 1. 왼쪽 영역: 치료 결과 정보 구성
    std::vector<std::string> healContent;
    std::string divider = std::string(GRAY) + " ------------------------------------------" + RESET;

    healContent.push_back("");
    healContent.push_back(std::string(BRIGHT_GREEN) + "  [ 휴식 및 치료 완료 ]" + RESET);
    healContent.push_back(divider);
    healContent.push_back("");

    if (prevHP >= maxHP) {
        healContent.push_back(std::string(BRIGHT_CYAN) + "  이미 최상의 컨디션입니다!" + RESET);
        healContent.push_back("");
        healContent.push_back("  현재 HP: " + std::string(BRIGHT_WHITE) + std::to_string(maxHP) + RESET + " / " + std::to_string(maxHP));
    }
    else {
        healContent.push_back("  따뜻한 휴식으로 체력을 회복했습니다.");
        healContent.push_back("");
        // 주사위 결과값(회복량) 강조
        healContent.push_back("  체력 [ " + std::string(BRIGHT_GREEN) + "+" + std::to_string(healValue) + RESET + " ] 회복 완료!");
        healContent.push_back("");

        // HP 변화 시각화
        std::string hpChange = "  HP 변화: " + std::to_string(prevHP) + " -> " + std::string(BRIGHT_GREEN) + std::to_string(curHP) + RESET;
        healContent.push_back(hpChange);
        healContent.push_back("  최대 HP: " + std::to_string(maxHP));
    }

    healContent.push_back("");
    healContent.push_back(divider);
    healContent.push_back(std::string(DARK_GRAY) + "  잠시 후 메뉴로 돌아갑니다..." + RESET);

    // 2. 오른쪽 영역: 전달받은 주사위 프레임 사용
    // 기존 healArt 대신 매개변수로 받은 diceFrame을 그대로 사용합니다.

    // 3. 공용 엔진 호출 (제목: 치료 결과)
    RenderSplitScreen(healContent, diceFrame, "! 경고 !", false, RED);

    // 4. 결과 확인을 위한 딜레이
    Delay(1500);
}

void Renderer::RenderUpgradeResult(UpgradeStatus status, int prevLevel, int curLevel) {
    // 1. 왼쪽 영역: 강화 결과 메시지 구성
    std::vector<std::string> upgradeContent;
    std::string title;
    std::string titleColor;
    std::string divider = std::string(GRAY) + " ------------------------------------------" + RESET;

    upgradeContent.push_back("");

    if (status == UpgradeStatus::Success) {
        title = "UPGRADE SUCCESS";
        titleColor = BRIGHT_GREEN;

        upgradeContent.push_back(std::string(BRIGHT_GREEN) + "  [ 강 화 성 공 ]" + RESET);
        upgradeContent.push_back(divider);
        upgradeContent.push_back("");
        upgradeContent.push_back("  주사위의 " + std::string(BRIGHT_YELLOW) + "잠재력이 해방" + RESET + "되었습니다!");
        upgradeContent.push_back("");
        upgradeContent.push_back("  강화 등급 변화:");
        // 레벨 변화 강조 (이전 레벨은 회색, 현재 레벨은 밝은 하늘색)
        upgradeContent.push_back("  [ " + std::string(GRAY) + "Lv." + std::to_string(prevLevel) + RESET +
            " ] >>> [ " + std::string(BRIGHT_CYAN) + "Lv." + std::to_string(curLevel) + RESET + " ]");
        upgradeContent.push_back("");
        upgradeContent.push_back("  이제 더 강력한 운명을 개척할 수 있습니다.");
    }
    else {
        title = "UPGRADE FAILED";
        titleColor = BRIGHT_RED;

        upgradeContent.push_back(std::string(BRIGHT_RED) + "  [ 강화 불가 ]" + RESET);
        upgradeContent.push_back(divider);
        upgradeContent.push_back("");
        upgradeContent.push_back("  이 주사위는 이미 " + std::string(BRIGHT_RED) + "한계치" + RESET + "에 도달했습니다.");
        upgradeContent.push_back("");
        // MAX 레벨 강조
        upgradeContent.push_back("  현재 등급: [ " + std::string(BRIGHT_YELLOW) + "Lv." + std::to_string(prevLevel) + RESET + " ] " +
            std::string(BRIGHT_RED) + "(MAX)" + RESET);
        upgradeContent.push_back("");
        upgradeContent.push_back("  다른 주사위를 강화해 보십시오.");
    }

    upgradeContent.push_back("");
    upgradeContent.push_back(divider);
    upgradeContent.push_back(std::string(DARK_GRAY) + "  대장장이가 장비를 정리합니다..." + RESET);

    // 2. 오른쪽 영역: 대장장이의 망치/모루 ASCII 아트 (금속 느낌 색상 추가)
    std::vector<std::string> upgradeArt = {
        "",
        std::string(GRAY) + "            _ .--.            " + RESET,
        std::string(GRAY) + "           ( `    )   .-.     " + RESET,
        std::string(GRAY) + "         .-'      `-'   )    " + RESET,
        std::string(GRAY) + "         (      _      -'     " + RESET,
        std::string(WHITE) + "          |    | |    |      " + RESET,
        std::string(WHITE) + "         _|    | |    |_     " + RESET,
        std::string(BRIGHT_WHITE) + "        [      |_|      ]    " + RESET,
        std::string(GRAY) + "         '--------------'    " + RESET,
        "",
        // 13
        std::string(BRIGHT_YELLOW) + "         [ 르탄이의 모루 ]  " + RESET
    };

    // 3. 공용 엔진 호출
    RenderSplitScreen(upgradeContent, upgradeArt, title, false);

    // 4. 결과 확인을 위한 딜레이
    Delay(1500);
}
void Renderer::RenderTicketInsufficient() {
    // 1. 왼쪽 영역: 경고 메시지 구성
    std::vector<std::string> alertContent;
    std::string divider = std::string(GRAY) + " ------------------------------------------" + RESET;

    alertContent.push_back("");
    // [출입 제한] 제목을 밝은 빨간색으로 강조
    alertContent.push_back(std::string(BRIGHT_RED) + "  [ 출입 제한 ]" + RESET);
    alertContent.push_back(divider);
    alertContent.push_back("");

    // 핵심 아이템인 [휴식권]은 노란색으로 강조
    alertContent.push_back("  필요한 [" + std::string(BRIGHT_YELLOW) + " 휴식권 " + RESET + "]이 부족합니다.");
    alertContent.push_back("");
    alertContent.push_back("  현재 보유하신 티켓으로는");
    alertContent.push_back("  이 작업을 수행할 수 없습니다.");
    alertContent.push_back("");
    alertContent.push_back("  마을에서 준비를 더 마친 뒤에 다시 오십시오.");
    alertContent.push_back("");
    alertContent.push_back(divider);

    // 안내 문구는 흐릿하게 처리
    alertContent.push_back(std::string(DARK_GRAY) + "  잠시 후 이전 화면으로 돌아갑니다..." + RESET);

    // 2. 오른쪽 영역: 자물쇠/X 아트 (색상 추가)
    std::vector<std::string> alertArt = {
        "",
        std::string(GRAY) + "            .--------.       " + RESET,
        std::string(GRAY) + "           /          \\      " + RESET,
        std::string(GRAY) + "          |   ______   |     " + RESET,
        std::string(GRAY) + "          |  |      |  |     " + RESET,
        "          |  |  " + std::string(BRIGHT_RED) + "[X]" + RESET + " |  |     ",
        std::string(GRAY) + "          |__|______|__|     " + RESET,
        "          |  " + std::string(RED) + "[ 잠금 ]" + RESET + "  |     ",
        std::string(GRAY) + "          '------------'     " + RESET,
        "",
        std::string(BRIGHT_RED) + "          휴식 티켓 부족   " + RESET
    };

    // 3. 공용 엔진 호출 (제목: ! ACCESS DENIED !)
    // 제목 자체에도 빨간색을 입히면 더 경고 효과가 큽니다.
    RenderSplitScreen(alertContent, alertArt, "캠프파이어 : 휴식 공간", false, CYAN);

    // 4. 경고를 인지할 수 있도록 딜레이
    Delay(1500);
}

void Renderer::RenderDiceUpgradeOption(const std::vector<std::string>& diceFrame) {
    // 1. 왼쪽 영역: 강화 경로 및 상세 설명 구성
    std::vector<std::string> upgradeOptionContent;
    std::string divider = std::string(GRAY) + " ------------------------------------------" + RESET;

    upgradeOptionContent.push_back("");
    upgradeOptionContent.push_back(std::string(BRIGHT_YELLOW) + "  [ 주사위 강화 경로 선택 ]" + RESET);
    upgradeOptionContent.push_back("  어떤 방향으로 주사위를 개조하시겠습니까?");
    upgradeOptionContent.push_back(divider);
    upgradeOptionContent.push_back("");

    // [1] 최소값 강화 - 안정적인 느낌 (CYAN)
    upgradeOptionContent.push_back(std::string(BRIGHT_CYAN) + "  [1] 최소값 +1 증가 (MIN UP)" + RESET);
    upgradeOptionContent.push_back(std::string(GRAY) + "      - 저점을 높여 안정적인 성능을 보장합니다." + RESET);
    upgradeOptionContent.push_back("");

    // [2] 최대값 강화 - 공격적인 느낌 (MAGENTA / PURPLE)
    upgradeOptionContent.push_back(std::string(BRIGHT_MAGENTA) + "  [2] 최대값 +1 증가 (MAX UP)" + RESET);
    upgradeOptionContent.push_back(std::string(GRAY) + "      - 고점을 높여 폭발적인 데미지를 노립니다." + RESET);
    upgradeOptionContent.push_back("");

    upgradeOptionContent.push_back(divider);

    // 취소 버튼 - 화이트/회색 계열
    upgradeOptionContent.push_back("  " + std::string(WHITE) + "[0] 취소하고 돌아가기" + RESET);

    // 2. 공용 엔진 호출
    RenderSplitScreen(upgradeOptionContent, diceFrame, "주사위 강화", false);

    // 3. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 옵션 선택 : " << RESET;
}
void Renderer::RenderShopItemList(const std::vector<BaseItem*>& itemLists, int playerGold, const std::vector<std::string>& diceFrame) {
    std::vector<std::string> shopContent;

    // --- [1. 규격 설정] ---
    const int W_IDX = 8;
    const int W_NAME = 26;
    const int W_TYPE = 12;
    const int W_PRICE = 12;
    const int W_STAT = 34;

    auto CenterText = [&](std::string text, int width) {
        int vLen = GetVisualLength(text);
        int totalPad = std::max(0, width - vLen);
        int leftPad = totalPad / 2;
        return std::string(leftPad, ' ') + text + std::string(totalPad - leftPad, ' ');
        };

    auto LeftText = [&](std::string text, int width) {
        int vLen = GetVisualLength(text);
        return text + std::string(std::max(0, width - vLen), ' ');
        };

    auto RightText = [&](std::string text, int width) {
        int vLen = GetVisualLength(text);
        int totalPad = std::max(0, width - vLen - 1);
        return std::string(totalPad, ' ') + text + " ";
        };

    // --- [2. 상단 정보 및 헤더] ---
    std::string horizontalBar = " ----------------------------------------------------------------------------------------------";
    shopContent.push_back("");
    shopContent.push_back("  보유 골드 : " + std::string(BRIGHT_YELLOW) + std::to_string(playerGold) + " G" + RESET);
    shopContent.push_back(std::string(GRAY) + horizontalBar + RESET);

    // [수정 포인트] "아이템 스탯" 텍스트만 CenterText 적용
    std::string headerLine = CYAN + std::string("  [##] ") + LeftText("아이템 이름", W_NAME) + GRAY + "|" +
        CYAN + CenterText("타입", W_TYPE) + GRAY + "|" +
        CYAN + CenterText("가격", W_PRICE) + GRAY + "|" +
        CYAN + CenterText("아이템 스탯", W_STAT) + RESET; // 여기를 CenterText로 변경

    shopContent.push_back(headerLine);
    shopContent.push_back(std::string(GRAY) + horizontalBar + RESET);

    // --- [3. 아이템 리스트 생성] ---
    for (int i = 0; i < (int)itemLists.size(); i++) {
        if (i >= 10) break;
        BaseItem* item = itemLists[i];
        StatDelta statDelta = item->GetStatDelta();

        // [A] 번호 & 이름
        std::string numStr = (i + 1 < 10) ? " " + std::to_string(i + 1) : std::to_string(i + 1);
        std::string indexPart = "  [" + std::string(BRIGHT_GREEN) + numStr + RESET + "] ";
        std::string namePart = LeftText(item->GetName(), W_NAME);

        // [B] 타입 & 가격
        std::string typePart = BRIGHT_CYAN + CenterText(item->GetTypeToString(item->GetType()), W_TYPE) + RESET;
        std::string pricePart = YELLOW + RightText(std::to_string(item->GetPrice()) + "G", W_PRICE) + RESET;

        // [C] 스탯 동적 정렬 (0 제외 + 왼쪽부터 채우기)
        std::vector<std::string> activeStats;
        if (statDelta.atk > 0) {
            activeStats.push_back(std::string(BRIGHT_RED) + "공격력 +" + std::to_string(statDelta.atk) + RESET);
        }
        if (statDelta.def > 0) {
            activeStats.push_back(std::string(BRIGHT_CYAN) + "방어력 +" + std::to_string(statDelta.def) + RESET);
        }

        std::string statLine = "";
        for (size_t j = 0; j < activeStats.size(); ++j) {
            statLine += activeStats[j];
            if (j < activeStats.size() - 1) statLine += "  ";
        }

        // 데이터 스탯 영역은 왼쪽 정렬 유지 (앞 여백 2칸)
        std::string finalStatPart = "  " + statLine;
        int currentVisualLen = GetVisualLength(finalStatPart);
        finalStatPart += std::string(std::max(0, W_STAT - currentVisualLen), ' ');

        // [D] 한 줄 조립
        std::string line = indexPart + namePart + std::string(GRAY) + "|" + RESET
            + typePart + std::string(GRAY) + "|" + RESET
            + pricePart + std::string(GRAY) + "|" + RESET
            + finalStatPart;

        shopContent.push_back(line);
    }

    shopContent.push_back(std::string(GRAY) + horizontalBar + RESET);
    shopContent.push_back("  [0] " + std::string(WHITE) + "마을로 돌아가기" + RESET);
    shopContent.push_back("");

    std::vector<std::string> infoContent;
    RenderSplitScreenSub(shopContent, infoContent, "만물상 상점", false);

    MoveCursor(0, 23);
    std::cout << BRIGHT_GREEN << " > 구매 아이템 번호 입력 : " << RESET;
}

void Renderer::RenderBuyResult(BuyStatus status, BaseItem* item, int playerGold) {
    std::vector<std::string> buyContent;
    std::vector<std::string> resultArt;
    std::string title = "만물상 상점";
    std::string color = WHITE; // 기본값

    buyContent.push_back("");

    switch (status) {
    case BuyStatus::Success:
        color = BRIGHT_GREEN;
        // 제목과 핵심 메시지에 초록색 적용
        buyContent.push_back(std::string(color) + " [ 구매 성공! ]" + RESET);
        buyContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        buyContent.push_back("");
        if (item) {
            buyContent.push_back(" [" + std::string(BRIGHT_YELLOW) + item->GetName() + RESET + "] (을)를");
        }
        buyContent.push_back(" 가방에 안전하게 넣었습니다.");
        buyContent.push_back("");
        buyContent.push_back(" 남은 골드: " + std::string(BRIGHT_YELLOW) + std::to_string(playerGold) + " G" + RESET);

        // 돈주머니 아트는 노란색으로 강조
        resultArt = { "",
            std::string(BRIGHT_YELLOW) + "              .----. " + RESET,
            std::string(BRIGHT_YELLOW) + "             |  $$  |" + RESET,
            std::string(BRIGHT_YELLOW) + "            '--[  ]--'" + RESET,
            std::string(BRIGHT_YELLOW) + "             (  $   )" + RESET,
            std::string(BRIGHT_YELLOW) + "              '----' " + RESET,
            "", "              고맙다! " };
        break;

    case BuyStatus::InsufficientGold:
        color = RED;
        buyContent.push_back(std::string(color) + " !! 골드 부족 !!" + RESET);
        buyContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        buyContent.push_back("");
        buyContent.push_back(" 지갑이 가벼워 물건을 살 수 없습니다.");
        if (item) {
            buyContent.push_back(" 필요 골드: " + std::string(BRIGHT_RED) + std::to_string(item->GetPrice()) + " G" + RESET);
        }
        buyContent.push_back(" 현재 골드: " + std::string(BRIGHT_YELLOW) + std::to_string(playerGold) + " G" + RESET);

        resultArt = { "",
            std::string(DARK_GRAY) + "              .----. " + RESET,
            std::string(DARK_GRAY) + "             | EMPTY|" + RESET,
            std::string(DARK_GRAY) + "            '--[  ]--'" + RESET,
            std::string(DARK_GRAY) + "             (      )" + RESET,
            std::string(DARK_GRAY) + "              '----' " + RESET,
            "", std::string(RED) + "          돈이 부족합니다. " + RESET };
        break;

    case BuyStatus::Possessed:
        color = RED;
        buyContent.push_back(std::string(color) + " !! 중복 소유 !!" + RESET);
        buyContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        buyContent.push_back("");
        buyContent.push_back(" 이미 동일한 아이템을 가지고 있습니다.");
        buyContent.push_back(" 중복 구매는 허용되지 않습니다.");

        resultArt = { "",
            std::string(BRIGHT_YELLOW) + "             .----. " + RESET,
            std::string(BRIGHT_YELLOW) + "            |  !!  |" + RESET,
            std::string(BRIGHT_YELLOW) + "           '--[  ]--'" + RESET,
            std::string(BRIGHT_YELLOW) + "          (  ALREADY )" + RESET,
            std::string(BRIGHT_YELLOW) + "             '----' " + RESET,
            "", "  이미 아이템를 보유한거 같은데?" };
        break;

    default:
        color = RED;
        buyContent.push_back(std::string(color) + " !! 구 매 실 패 !!" + RESET);
        buyContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        buyContent.push_back("");
        buyContent.push_back(" 거래 도중 문제가 발생했습니다.");
        resultArt = { "",
            std::string(RED) + "      XXXXX   " + RESET,
            std::string(RED) + "      X   X   " + RESET,
            std::string(RED) + "      XXXXX   " + RESET,
            "", "   ERROR...   " };
        break;
    }

    buyContent.push_back("");
    buyContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
    buyContent.push_back(" 잠시 후 상점으로 돌아갑니다...");

    // 2. 공용 엔진 호출
    RenderSplitScreen(buyContent, resultArt, title, false);

    // 3. 결과 확인 딜레이
    Delay(1500);
}

void Renderer::RenderInventory(int level, int CurExp, int MaxLevelExp, int restTicketCount, BaseItem* slots[], const std::vector<ItemSlot>& gearStorage,
    const std::vector<DiceSlot>& diceStorage, const std::vector<std::string>& diceFrame)
{
    // 1. 왼쪽 영역 (invContent): 상태, 장착 장비, 주사위 정보
    std::vector<std::string> invContent;
    std::string divider = std::string(GRAY) + " ------------------------------------------" + RESET;

    invContent.push_back("");
    // [ CHARACTER STATUS ]
    invContent.push_back(std::string(BRIGHT_YELLOW) + "  [ 캐릭터 스탯 ]" + RESET);
    invContent.push_back("  - 현재 레벨 : " + std::string(BRIGHT_WHITE) + "LV. " + std::to_string(level) + RESET);
    invContent.push_back("  - 경 험 치  : " + std::string(BRIGHT_CYAN) + std::to_string(CurExp) + RESET + " / " + std::to_string(MaxLevelExp));

    // 휴식 티켓 정보 추가 (분홍색/밝은 자색 계열로 눈에 띄게 배치)
    invContent.push_back("  - 휴식 티켓 : " + std::string(BRIGHT_MAGENTA) + std::to_string(restTicketCount) + " 장" + RESET);

    invContent.push_back(divider);

    // [섹션 1] 현재 장착 장비
    invContent.push_back(std::string(BRIGHT_CYAN) + "  [ 현재 장착 장비 ]" + RESET);
    const char* slotNames[] = { "무  기", "헬  멧", "갑  옷", "신  발", "장신구" };
    for (int i = 0; i < 5; i++) {
        std::string line = "  - " + std::string(slotNames[i]) + " : ";
        if (slots[i] != nullptr) {
            line += std::string(BRIGHT_YELLOW) + slots[i]->GetName() + RESET;
            StatDelta delta = slots[i]->GetStatDelta();
            std::string statInfo = "";

            if (delta.atk != 0 && delta.def == 0) {
                statInfo = " (" + std::string(RED) + "공격력 +" + std::to_string(delta.atk) + RESET + ")";
            }
            else if (delta.def != 0 && delta.atk == 0) {
                statInfo = " (" + std::string(BRIGHT_CYAN) + "방어력 +" + std::to_string(delta.def) + RESET + ")";
            }
            else if (delta.atk != 0 && delta.def != 0) {
                statInfo = " (" + std::string(RED) + "공격력+" + std::to_string(delta.atk) + RESET +
                    " " + std::string(BRIGHT_CYAN) + "방어력+" + std::to_string(delta.def) + RESET + ")";
            }
            line += statInfo;
        }
        else {
            line += std::string(DARK_GRAY) + "---" + RESET;
        }
        invContent.push_back(line);
    }
    invContent.push_back(divider);

    // [섹션 2] 소지 주사위
    invContent.push_back(std::string(BRIGHT_YELLOW) + "  [ 소지 중인 주사위 ]" + RESET);
    if (diceStorage.empty()) {
        invContent.push_back(std::string(DARK_GRAY) + "    (비어 있음)" + RESET);
    }
    else {
        std::string dRow = "    ";
        for (const auto& ds : diceStorage) {
            dRow += std::string(BRIGHT_YELLOW) + ds.dice->DiceIdToString() + RESET + "(" + std::to_string(ds.count) + ")";
        }
        invContent.push_back(dRow);
    }
    invContent.push_back(divider);
    invContent.push_back("  " + std::string(WHITE) + "[0] 마을로 돌아가기" + RESET);

    // 2. 오른쪽 영역 (rightContent): 소지 장비 목록
    std::vector<std::string> rightContent;
    rightContent.push_back("");
    rightContent.push_back(std::string(BRIGHT_GREEN) + "       [ 집행자의 소지 장비 ]" + RESET);
    rightContent.push_back(std::string(GRAY) + "    ----------------------------" + RESET);
    rightContent.push_back("");

    if (gearStorage.empty()) {
        rightContent.push_back(std::string(DARK_GRAY) + "     (소지품이 비어 있습니다.)" + RESET);
    }
    else {
        for (int i = 0; i < (int)gearStorage.size(); i++) {
            if (i >= 12) {
                rightContent.push_back(std::string(GRAY) + "      ..." + RESET);
                break;
            }
            std::string idx = std::string(BRIGHT_GREEN) + "[" + std::to_string(i + 1) + "] " + RESET;
            std::string itemName = gearStorage[i].item->GetName();
            std::string count = std::string(GRAY) + " x" + std::to_string(gearStorage[i].count) + RESET;

            rightContent.push_back("    " + idx + itemName + count);
        }
    }

    // 3. 공용 엔진 호출
    RenderSplitScreen(invContent, rightContent, "집행자의 가방", false);

    // 4. 입력 위치 고정
    MoveCursor(0, 48);
    std::cout << BRIGHT_GREEN << " > 장비 번호 선택 : " << RESET;
}

void Renderer::RenderEquipResult(const EquipResult& result) {
    std::vector<std::string> equipContent;
    std::vector<std::string> equipArt;
    std::string title = "집행자의 가방";

    // 안전한 아이템 이름 처리
    std::string itemName = (result.item != nullptr) ? result.item->GetName() : "알 수 없는 아이템";

    equipContent.push_back("");

    switch (result.status) {
    case EquipStatus::Equip:
        // [장착 성공] - BRIGHT_GREEN
        equipContent.push_back(std::string(BRIGHT_GREEN) + " [ 장착 성공 ]" + RESET);
        equipContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        equipContent.push_back("");
        equipContent.push_back(" 새로운 장비를 몸에 맞췄습니다.");
        equipContent.push_back("");
        equipContent.push_back(" 현재 착용: " + std::string(BRIGHT_YELLOW) + "[" + itemName + "]" + RESET);

        break;

    case EquipStatus::Changed:
        // [장비 교체] - CYAN
        equipContent.push_back(std::string(CYAN) + " [ 장비 교체 완료 ]" + RESET);
        equipContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        equipContent.push_back("");
        equipContent.push_back(" 기존 장비를 해제하고 교체했습니다.");
        equipContent.push_back("");
        equipContent.push_back(std::string(GRAY) + " 이전: [" + result.prevItem + "]" + RESET);
        equipContent.push_back(std::string(YELLOW) + "  | " + RESET);
        equipContent.push_back(std::string(YELLOW) + "  V " + RESET);
        equipContent.push_back(" 현재: " + std::string(BRIGHT_CYAN) + "[" + itemName + "]" + RESET);


        break;

    case EquipStatus::Overlap:
        // [중복 경고] - RED
        equipContent.push_back(std::string(RED) + " !! 착용 중 !!" + RESET);
        equipContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        equipContent.push_back("");
        equipContent.push_back(" 동일한 장비를 중복해서 낄 수 없습니다.");
        equipContent.push_back("");
        equipContent.push_back(" 대상: " + std::string(BRIGHT_RED) + "[" + itemName + "]" + RESET);

        break;

    default:
        // [기타 오류] - GRAY
        equipContent.push_back(std::string(GRAY) + " !! 장 착 불 가 !!" + RESET);
        equipContent.push_back(" ------------------------------------------");
        equipContent.push_back("");
        equipContent.push_back(" 장착 과정에서 알 수 없는 오류 발생.");

        break;
    }

    equipContent.push_back("");
    equipContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
    equipContent.push_back(std::string(DARK_GRAY) + " 잠시 후 소지품 창으로 돌아갑니다..." + RESET);

    // 2. 공용 엔진 호출
    RenderSplitScreen(equipContent, equipArt, title, false);

    // 3. 결과 확인 딜레이
    Delay(1500);
}
void Renderer::Clear() {

    system("cls");
}

void Renderer::Delay(int ms) {
    Sleep(ms);
}

void Renderer::PrintTyping(const std::string& text, int speed) {
    for (int i = 0; i < (int)text.length(); i++) {
        // 1. 한글 처리 (2바이트 문자 감지)
        if ((unsigned char)text[i] >= 0x80) {
            if (i + 1 < (int)text.length()) {
                std::cout << text[i] << text[i + 1] << std::flush;
                i++;
            }
        }
        // 2. 일반 영문/기호 처리
        else {
            std::cout << text[i] << std::flush;
        }

        // 3. 딜레이 적용
        if (speed > 0) Sleep(speed);
    }
    // 타이핑 종료 후 줄바꿈은 호출하는 쪽에서 제어하도록 하는 것이 UI 배치에 유리할 수 있습니다.
    std::cout << std::endl;
}
