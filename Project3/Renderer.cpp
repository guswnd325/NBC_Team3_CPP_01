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
void Renderer::AddSystemLog(std::string msg) {
    systemLogs.push_back(msg);

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
    bool isBattleMode)
{
    // 1. 화면 초기화 및 상단 타이틀
    Clear();
    MoveCursor(0, Renderer::ZONE_SCREEN_Y);
    PrintTop(Renderer::UI_WIDTH);
    PrintCenterLine("[ " + title + " ]", Renderer::UI_WIDTH, CYAN);
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
        PrintLeftLine("[ 실시간 전투 로그 ]", Renderer::UI_WIDTH, GRAY);
        for (const auto& log : battleLogs) {
            PrintLeftLine(" >> " + log, Renderer::UI_WIDTH, WHITE);
        }
        for (int i = 0; i < (MAX_LOGS - (int)battleLogs.size()); i++) PrintLeftLine("", Renderer::UI_WIDTH);
    }
    else {
        PrintLeftLine("[ 시스템 메시지 ]", Renderer::UI_WIDTH, GRAY);
        for (const auto& log : systemLogs) {
            PrintLeftLine(" >> " + log, Renderer::UI_WIDTH, WHITE);
        }
        for (int i = 0; i < (MAX_MENU_LOGS - (int)systemLogs.size()); i++) PrintLeftLine("", Renderer::UI_WIDTH);
    }

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
    menuContent.push_back(center(R"(      .-------.      ____  _ ____ _____      )"));
    menuContent.push_back(center(R"(     /   o   / |    |  _ \(_) ___||  __|     )"));
    menuContent.push_back(center(R"(     /  o    / o|    | | | | | |   | |__       )"));
    menuContent.push_back(center(R"(    /   o   / o |    | |_| | | |___|  __|      )"));
    menuContent.push_back(center(R"(    '-------' o /    |____/|_|\____|____|      )"));
    menuContent.push_back(center(R"( | o     |  /      _  ____ ____ _  _ ___ )"));
    menuContent.push_back(center(R"(  |    o  | /      /_\ | __ |___ |\ |  |    )"));
    menuContent.push_back(center(R"(  | o     |/      /_  \|__] |___ | \|  |    )"));
    menuContent.push_back(center(R"(  '-------'                                 )"));
    menuContent.push_back("");
    menuContent.push_back(""); // 로고와 버튼 사이 간격 추가

    // 2. 버튼 설정 및 중앙 정렬
    std::string startBtn = std::string(BRIGHT_GREEN) + "[1] 게임 시작" + RESET;
    std::string exitBtn = std::string(RED) + "[2] 게임 종료" + RESET;

    // 버튼 두 개를 합친 문자열을 만들고 통째로 중앙 정렬
    // "          " 부분을 조절하여 버튼 사이의 간격을 넓히거나 좁힐 수 있습니다.
    std::string buttons = startBtn + "          " + exitBtn;
    menuContent.push_back(center(buttons));

    // 3. 엔진 호출
    RenderSplitScreen(menuContent, diceFrame, "DICE ADVENTURE : THE ROGUELIKE", false);

    // 4. 입력 위치 및 콘솔 크기 대응
    // 하단 테두리 선(PrintBottom)이 겹치지 않게 MoveCursor 위치를 넉넉히 잡으세요.
    // system("mode con cols=120 lines=55") 기준 47~50이 적당합니다.
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
        "      < 운명의 주사위 >",
        "",
        "          _______",
        "         /      /|",
        "        /   o  / |",
        "       /______/  |",
        "       |      | o|",
        "       |  o   |  /",
        "       |______| /",
        "",
        "    당신의 행운을 믿으십시오."
    };

    // 3. 공용 엔진 호출 (제목: 운명의 집행자 선택)
    RenderSplitScreen(charSelectionInfo, destinyDice, "운명의 집행자 선택", false);

    // 4. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 집행자 번호 입력 : " << RESET;
}

void Renderer::RenderMainMenu(const std::vector<std::string>& diceFrame) {
    std::vector<std::string> townContent;
    int LW = Renderer::LEFT_WIDTH; // 기준 너비 (60)

    // 공백을 계산해서 넣어주는 람다 함수 (편의용)
    auto center = [&](std::string text) {
        int len = GetVisualLength(text);
        int space = (LW - len) / 2;
        if (space < 0) space = 0;
        return std::string(space, ' ') + text;
        };

    townContent.push_back("");
    // 집 모양 아트 (가운데 정렬)
    townContent.push_back(center("      _ ^ _      "));
    townContent.push_back(center("     / |_| \\     "));
    townContent.push_back(center("    |_______|    "));
    townContent.push_back("");

    // 설명 문구 (가운데 정렬)
    townContent.push_back(center("모닥불 소리와 새들의 지저귐이 들리는"));
    townContent.push_back(center("평화로운 마을 '다이스톤'입니다."));
    townContent.push_back("");
    townContent.push_back(center("------------------------------------------"));
    townContent.push_back("");
    townContent.push_back(center(std::string(BRIGHT_GREEN) + "[1] 탐  사 (지역 조사)" + RESET));
    townContent.push_back(center(std::string(YELLOW) + "  [2] 상  점 (아이템 구매)" + RESET));
    townContent.push_back(center(std::string(CYAN) + "[3] 장비창 (인벤토리)" + RESET));
    townContent.push_back(center(std::string(CYAN) + "   [4] 휴  식 (회복 및 강화)" + RESET));
    townContent.push_back("");
    townContent.push_back(center("------------------------------------------"));
    // 2. 공용 엔진 호출
    // 마을 화면에서도 오른쪽에 diceFrame(주사위나 마을 전용 로그)을 띄웁니다.
    RenderSplitScreen(townContent, diceFrame, "평화로운 마을", false);

    // 3. 입력 위치 고정 (UI 하단부)
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 행동을 선택해라 : " << RESET;
}

void Renderer::RenderBattleAction(Monster* monster, Character* player, const std::vector<std::string>& diceFrame) {
    // 1. 왼쪽 영역 데이터 구성 (몬스터 정보 + 이미지 + 플레이어 정보)
    std::vector<std::string> battleContent;

    // [상단] 몬스터 정보 (이름 및 HP)
    std::string mHealth = " HP: " + std::to_string(monster->GetHP());
    battleContent.push_back(" [ ENEMY ] " + monster->GetName());
    battleContent.push_back(mHealth);
    battleContent.push_back(" ------------------------------------------");

    // [중간] 몬스터 이미지 (원본 GetVisual() 활용)
    const std::vector<std::string>& visual = monster->GetVisual();
    for (int i = 0; i < 7; i++) { // 이미지 공간 7줄 할당
        if (i < (int)visual.size()) battleContent.push_back(" " + visual[i]);
        else battleContent.push_back("");
    }

    battleContent.push_back(" ------------------------------------------");

    // [하단] 플레이어 정보
    std::string pHealth = " HP: " + std::to_string(player->GetHP()) + " / 100";
    battleContent.push_back(" [ PLAYER ] " + player->GetName());
    battleContent.push_back(pHealth);
    battleContent.push_back("");
    battleContent.push_back("  [1] 전  투             [2] 도  망");

    // 2. 공용 엔진 호출 
    // - diceFrame: 현재 굴러가는 주사위나 결과값이 담김
    // - title: "!!! BATTLE IN PROGRESS !!!"
    // - isBattleMode: true (하단 로그창을 battleLogs로 스위칭)
    RenderSplitScreen(battleContent, diceFrame, "!!! BATTLE IN PROGRESS !!!", true);

    // 3. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 행동 선택 : " << RESET;
}

void Renderer::RenderRewardSelect(const std::vector<std::string>& diceFrame) {
    // 1. 왼쪽 영역: 승리 메시지 및 선택지 구성
    std::vector<std::string> rewardContent;

    rewardContent.push_back("");
    rewardContent.push_back(" [ VICTORY ]");
    rewardContent.push_back(" 전투에서 승리하여 값진 전리품을 발견했습니다!");
    rewardContent.push_back(" 어떤 방식으로 보상을 챙기시겠습니까?");
    rewardContent.push_back("");
    rewardContent.push_back(" ------------------------------------------");
    rewardContent.push_back("");
    rewardContent.push_back("  [1] 일반 보상 (안전)");
    rewardContent.push_back("      - 휴식권 1회 + 확정 골드 획득");
    rewardContent.push_back("");
    rewardContent.push_back("  [2] 리스크 보상 (도전!)");
    rewardContent.push_back("      - 주사위를 굴려 더 큰 보상에 도전");
    rewardContent.push_back("");
    rewardContent.push_back(" ------------------------------------------");
    rewardContent.push_back("  당신의 운을 시험해 보십시오.");

    // 2. 공용 엔진 호출
    // 오른쪽(diceFrame)에는 현재 대기 중인 주사위나 보물상자 아트가 들어갑니다.
    RenderSplitScreen(rewardContent, diceFrame, "전리품 획득", false);

    // 3. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 보상을 선택해라 : " << RESET;
}

void Renderer::RenderStatus(Character* player, const std::vector<std::string>& diceFrame) {
    if (!player) return;

    // 1. 왼쪽 영역: 플레이어 상세 정보 구성
    std::vector<std::string> statusContent;

    statusContent.push_back(" [ PLAYER STATUS ]");
    statusContent.push_back(" 닉네임 : " + player->GetName());
    statusContent.push_back(" HP     : " + std::to_string(player->GetHP()) + " / " + std::to_string(MAX_HP));
    statusContent.push_back(" ATK    : " + std::to_string(player->GetAtk()) + " | DEF : " + std::to_string(player->GetDef()));
    statusContent.push_back(" 소지금 : " + std::to_string(player->GetGold()) + " G");
    statusContent.push_back(" 휴식권 : " + std::to_string(player->GetRestTicket()) + " 장");
    statusContent.push_back(" ------------------------------------------");
    statusContent.push_back(" [ 소지한 주사위 ]");

    Inventory* inv = player->GetInventory();
    if (inv) {
        const auto& diceList = inv->GetDiceStorege();
        if (diceList.empty()) {
            statusContent.push_back("   (소지 중인 주사위가 없습니다.)");
        }
        else {
            int count = 0;
            for (const auto& slot : diceList) {
                if (count >= 5) { // 2분할 높이에 맞춰 5개까지 표시
                    statusContent.push_back("   ...외 다수");
                    break;
                }
                if (!slot.dice) continue;
                statusContent.push_back(" - " + slot.dice->DiceIdToString() + " x" + std::to_string(slot.count));
                count++;
            }
        }
    }
    statusContent.push_back(" ------------------------------------------");

    // 2. 공용 엔진 호출 (제목: 캐릭터 상태창)
    RenderSplitScreen(statusContent, diceFrame, "CHARACTER STATUS", false);

    // 3. 입력 위치 (상태창은 보통 확인 후 아무 키나 눌러 넘어가므로 하단 고정)
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << YELLOW << " [ 아무 키나 누르면 돌아갑니다 ] " << RESET;
}

void Renderer::RenderAreaChoices(const std::vector<std::string>& choices,
    const std::unordered_map<std::string, std::string>& displayMap,
    const std::vector<std::string>& diceFrame)
{
    // 1. 왼쪽 영역: 지역 리스트 구성
    std::vector<std::string> areaContent;

    areaContent.push_back("");
    areaContent.push_back(" [ 탐험할 지역을 선택하십시오 ]");
    areaContent.push_back(" ------------------------------------------");
    areaContent.push_back("");

    for (int i = 0; i < (int)choices.size(); ++i) {
        // 한국어 이름이 있으면 가져오고, 없으면 영문 이름 사용
        std::string korName = displayMap.count(choices[i]) ? displayMap.at(choices[i]) : choices[i];
        areaContent.push_back("   [" + std::to_string(i + 1) + "] " + korName);
        areaContent.push_back(""); // 가독성을 위해 한 줄 띔
    }

    areaContent.push_back(" ------------------------------------------");
    areaContent.push_back("  [0] 마을로 돌아가기 (RETURN)");

    // 2. 공용 엔진 호출
    // 오른쪽(diceFrame)에는 지도나 나침반, 혹은 현재 선택 중인 연출용 프레임이 들어갑니다.
    RenderSplitScreen(areaContent, diceFrame, "WORLD MAP : EXPLORATION", false);

    // 3. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 지역 번호 입력 : " << RESET;
}

void Renderer::RenderRestMenu(const std::vector<std::string>& diceFrame) {
    std::vector<std::string> restContent;
    int LW = Renderer::LEFT_WIDTH;

    // 중앙 정렬 람다 (GetVisualLength 활용)
    auto center = [&](std::string text) {
        int len = GetVisualLength(text);
        int space = (LW - len) / 2;
        return (space > 0) ? std::string(space, ' ') + text : text;
        };

    restContent.push_back("");
    // 1. 모닥불 ASCII (빨간색과 노란색 조합으로 불꽃 느낌 극대화)
    restContent.push_back(center(std::string(RED) + "(  )  " + YELLOW + "(  )  " + RED + "(  )"));
    restContent.push_back(center(std::string(YELLOW) + " )  (  )  (  ("));
    restContent.push_back(center(std::string(GRAY) + "[____________]"));
    restContent.push_back("");

    // 2. 설명 문구 (포근한 느낌의 WHITE/GRAY)
    restContent.push_back(center("타오르는 불꽃이 지친 몸을 달래줍니다."));
    restContent.push_back(center(std::string(GRAY) + "이곳에서 전열을 가다듬으십시오." + RESET));
    restContent.push_back("");
    restContent.push_back(center(std::string(GRAY) + "------------------------------------------" + RESET));
    restContent.push_back("");

    // 3. 메뉴 항목
    // [1] 주사위 강화: 강해지는 느낌 (YELLOW)
    restContent.push_back("  " + std::string(YELLOW) + "[1] 주사위 강화" + RESET + " (휴식권 소모)");
    restContent.push_back(std::string(GRAY) + "      - 주사위의 눈을 영구적으로 업그레이드" + RESET);
    restContent.push_back("");

    // [2] 체력 회복: 생명력의 느낌 (BRIGHT_MAGENTA 또는 RED)
    restContent.push_back("  " + std::string(BRIGHT_MAGENTA) + "[2] 체력 회복" + RESET + "   (휴식권 소모)");
    restContent.push_back(std::string(GRAY) + "      - 잃은 체력의 일정량을 회복" + RESET);
    restContent.push_back("");

    // [0] 돌아가기: 취소/퇴장 느낌 (WHITE/GRAY)
    restContent.push_back("  " + std::string(WHITE) + "[0] 마을로 돌아가기 (RETURN)" + RESET);
    restContent.push_back("");
    restContent.push_back(center(std::string(GRAY) + "------------------------------------------" + RESET));

    // 4. 엔진 호출
    RenderSplitScreen(restContent, diceFrame, "CAMPFIRE : REST AREA", false);

    // 5. 입력 위치
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 원하는 행동을 선택하십시오 : " << RESET;
}

void Renderer::RenderDiceUpgradeList(const std::vector<DiceSlot>& storage, const std::vector<std::string>& diceFrame) {
    // 1. 왼쪽 영역: 주사위 강화 목록 데이터 구성
    std::vector<std::string> upgradeContent;

    upgradeContent.push_back("");
    upgradeContent.push_back(" [ 주사위 강화소 ]");
    upgradeContent.push_back(" 어떤 주사위의 운명을 강화하시겠습니까?");
    upgradeContent.push_back(" ------------------------------------------");
    upgradeContent.push_back("");

    if (storage.empty()) {
        upgradeContent.push_back("  (강화할 수 있는 주사위가 없습니다.)");
    }
    else {
        for (int i = 0; i < (int)storage.size(); i++) {
            // 최대 8개까지 출력 (2분할 높이 제한에 맞춤)
            if (i >= 8) {
                upgradeContent.push_back("      ...");
                break;
            }

            std::string diceName = storage[i].dice->DiceIdToString();
            std::string diceCount = " (" + std::to_string(storage[i].count) + "개)";

            // 리스트 형식으로 저장
            upgradeContent.push_back("  [" + std::to_string(i + 1) + "] " + diceName + diceCount);
        }
    }

    upgradeContent.push_back("");
    upgradeContent.push_back(" ------------------------------------------");
    upgradeContent.push_back("  [0] 취소하고 돌아가기");

    // 2. 공용 엔진 호출
    // 오른쪽(diceFrame)에는 현재 강화 대기 중인 주사위의 '눈' 정보나 아트를 띄웁니다.
    RenderSplitScreen(upgradeContent, diceFrame, "DICE UPGRADE SHOP", false);

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
    RenderSplitScreen(healContent, diceFrame, "HEAL COMPLETED", false);

    // 4. 결과 확인을 위한 딜레이
    Delay(1500);
}

void Renderer::RenderUpgradeResult(UpgradeStatus status, int prevLevel, int curLevel) {
    // 1. 왼쪽 영역: 강화 결과 메시지 구성
    std::vector<std::string> upgradeContent;
    std::string title;
    std::string titleColor;

    upgradeContent.push_back("");

    if (status == UpgradeStatus::Success) {
        title = "UPGRADE SUCCESS";
        titleColor = BRIGHT_GREEN;

        upgradeContent.push_back(" [ 강 화 성 공 ]");
        upgradeContent.push_back(" ------------------------------------------");
        upgradeContent.push_back("");
        upgradeContent.push_back(" 주사위의 잠재력이 해방되었습니다!");
        upgradeContent.push_back("");
        upgradeContent.push_back(" 강화 등급 변화:");
        upgradeContent.push_back(" [ Lv." + std::to_string(prevLevel) + " ] >>> [ Lv." + std::to_string(curLevel) + " ]");
        upgradeContent.push_back("");
        upgradeContent.push_back(" 이제 더 강력한 운명을 개척할 수 있습니다.");
    }
    else {
        title = "UPGRADE FAILED";
        titleColor = RED;

        upgradeContent.push_back(" [ 강화 불가 ]");
        upgradeContent.push_back(" ------------------------------------------");
        upgradeContent.push_back("");
        upgradeContent.push_back(" 이 주사위는 이미 한계치에 도달했습니다.");
        upgradeContent.push_back("");
        upgradeContent.push_back(" 현재 등급: [ Lv." + std::to_string(prevLevel) + " ] (MAX)");
        upgradeContent.push_back("");
        upgradeContent.push_back(" 다른 주사위를 강화해 보십시오.");
    }

    upgradeContent.push_back("");
    upgradeContent.push_back(" ------------------------------------------");
    upgradeContent.push_back(" 대장장이가 장비를 정리합니다...");

    // 2. 오른쪽 영역: 대장장이의 망치 ASCII 아트
    std::vector<std::string> upgradeArt = {
        "",
        "      _ .--.           ",
        "     ( `    )   .-.    ",
        "    .-'      `-'   )   ",
        "   (      _      -'    ",
        "    |    | |    |      ",
        "   _|    | |    |_     ",
        "  [      |_|      ]    ",
        "   '--------------'    ",
        "",
        "    [ SMITH'S ANVIL ]  "
    };

    // 3. 공용 엔진 호출
    RenderSplitScreen(upgradeContent, upgradeArt, title, false);

    // 4. 결과 확인을 위한 딜레이
    Delay(1500);
}

void Renderer::RenderTicketInsufficient() {
    // 1. 왼쪽 영역: 경고 메시지 구성
    std::vector<std::string> alertContent;

    alertContent.push_back("");
    alertContent.push_back(" [ 출 입 제 한 ]");
    alertContent.push_back(" ------------------------------------------");
    alertContent.push_back("");
    alertContent.push_back(" 필요한 [ 휴식권 ]이 부족합니다.");
    alertContent.push_back("");
    alertContent.push_back(" 현재 보유하신 티켓으로는");
    alertContent.push_back(" 이 작업을 수행할 수 없습니다.");
    alertContent.push_back("");
    alertContent.push_back(" 마을에서 준비를 더 마친 뒤에 다시 오십시오.");
    alertContent.push_back("");
    alertContent.push_back(" ------------------------------------------");
    alertContent.push_back(" 잠시 후 이전 화면으로 돌아갑니다...");

    // 2. 오른쪽 영역: 부족함을 알리는 자물쇠/X 아트
    std::vector<std::string> alertArt = {
        "",
        "       .--------.       ",
        "      /          \\      ",
        "     |   ______   |     ",
        "     |  |      |  |     ",
        "     |  |  [X] |  |     ",
        "     |__|______|__|     ",
        "     | [LOCKED]   |     ",
        "     '------------'     ",
        "",
        "    INSUFFICIENT TICKET  "
    };

    // 3. 공용 엔진 호출 (제목: 자원 부족)
    RenderSplitScreen(alertContent, alertArt, "! ACCESS DENIED !", false);

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
    upgradeOptionContent.push_back("  " + std::string(WHITE) + "[0] 취소하고 돌아가기 (CANCEL)" + RESET);

    // 2. 공용 엔진 호출
    RenderSplitScreen(upgradeOptionContent, diceFrame, "DICE REFORGING", false);

    // 3. 입력 위치 고정
    MoveCursor(0, Renderer::ZONE_PLAYER_Y + 6);
    std::cout << BRIGHT_GREEN << " > 옵션 선택 입력 : " << RESET;
}

void Renderer::RenderShopItemList(const std::vector<BaseItem*>& itemLists, int playerGold, const std::vector<std::string>& diceFrame) {
    std::vector<std::string> shopContent;

    // 상단 정보: 보유 골드를 밝은 노란색으로 강조
    shopContent.push_back("  [ 만물상 상점 ]   보유: " + std::string(BRIGHT_YELLOW) + std::to_string(playerGold) + " G" + RESET);
    shopContent.push_back(std::string(GRAY) + " ------------------------------------------------" + RESET);

    // 헤더: CYAN 색상으로 가독성 상향
    shopContent.push_back(std::string(CYAN) + "  [#]  아이템 이름              |  타입  |  가격" + RESET);
    shopContent.push_back(std::string(GRAY) + "  -----------------------------------------------" + RESET);

    int maxDisplay = 10;
    for (int i = 0; i < (int)itemLists.size(); i++) {
        if (i >= maxDisplay) break;

        std::string name = itemLists[i]->GetName();
        std::string type = itemLists[i]->GetTypeToString(itemLists[i]->GetType());
        std::string price = std::to_string(itemLists[i]->GetPrice()) + "G";

        // 1. 이름 정렬 및 색상 (이름은 기본 WHITE)
        int nameW = GetVisualLength(name);
        int nameTarget = 24;
        std::string namePadding = (nameTarget > nameW) ? std::string(nameTarget - nameW, ' ') : "";
        if (nameW > nameTarget) name = name.substr(0, nameTarget - 2) + "..";

        // 2. 타입 정렬 및 색상 (타입은 밝은 푸른색)
        int typeW = GetVisualLength(type);
        int typeTarget = 8;
        std::string typePadding = (typeTarget > typeW) ? std::string(typeTarget - typeW, ' ') : "";
        std::string coloredType = std::string(BRIGHT_CYAN) + type + RESET;

        // 3. 가격 색상 (돈이니까 GOLD/YELLOW)
        std::string coloredPrice = std::string(YELLOW) + price + RESET;

        // 4. 인덱스 번호 (GREEN)
        std::string idxStr = (i + 1 < 10) ? " " + std::to_string(i + 1) : std::to_string(i + 1);
        std::string coloredIdx = std::string(BRIGHT_GREEN) + idxStr + RESET;

        // 한 줄 조립 (구분선 '|'은 GRAY로 차분하게)
        std::string line = "  [" + coloredIdx + "] " + name + namePadding + std::string(GRAY) + "| " + RESET
            + coloredType + typePadding + std::string(GRAY) + "| " + RESET + coloredPrice;

        shopContent.push_back(line);
    }

    while (shopContent.size() < 15) shopContent.push_back("");

    shopContent.push_back(std::string(GRAY) + " ------------------------------------------------" + RESET);
    shopContent.push_back("  [0] " + std::string(WHITE) + "마을로 돌아가기 (RETURN)" + RESET);

    RenderSplitScreen(shopContent, diceFrame, "GENERAL STORE", false);

    // 입력 위치 보정 (박스 하단 테두리 아래로)
    MoveCursor(0, 48);
    std::cout << BRIGHT_GREEN << " > 구매 아이템 번호 입력 : " << RESET;
}

void Renderer::RenderBuyResult(BuyStatus status, BaseItem* item, int playerGold) {
    std::vector<std::string> buyContent;
    std::vector<std::string> resultArt;
    std::string title = "TRANSACTION RESULT";
    std::string color = WHITE; // 기본값

    buyContent.push_back("");

    switch (status) {
    case BuyStatus::Success:
        color = BRIGHT_GREEN;
        // 제목과 핵심 메시지에 초록색 적용
        buyContent.push_back(std::string(color) + " [ 구 매 성 고 ! ]" + RESET);
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
            std::string(BRIGHT_YELLOW) + "      .----. " + RESET,
            std::string(BRIGHT_YELLOW) + "     |  $$  |" + RESET,
            std::string(BRIGHT_YELLOW) + "    '--[  ]--'" + RESET,
            std::string(BRIGHT_YELLOW) + "     (  $   )" + RESET,
            std::string(BRIGHT_YELLOW) + "      '----' " + RESET,
            "", "   THANK YOU! " };
        break;

    case BuyStatus::InsufficientGold:
        color = BRIGHT_RED;
        buyContent.push_back(std::string(color) + " !! 골 드 부 족 !!" + RESET);
        buyContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        buyContent.push_back("");
        buyContent.push_back(" 지갑이 가벼워 물건을 살 수 없습니다.");
        if (item) {
            buyContent.push_back(" 필요 골드: " + std::string(BRIGHT_RED) + std::to_string(item->GetPrice()) + " G" + RESET);
        }
        buyContent.push_back(" 현재 골드: " + std::string(BRIGHT_YELLOW) + std::to_string(playerGold) + " G" + RESET);

        resultArt = { "",
            std::string(DARK_GRAY) + "      .----. " + RESET,
            std::string(DARK_GRAY) + "     | EMPTY|" + RESET,
            std::string(DARK_GRAY) + "    '--[  ]--'" + RESET,
            std::string(DARK_GRAY) + "     (      )" + RESET,
            std::string(DARK_GRAY) + "      '----' " + RESET,
            "", std::string(RED) + "  너 돈 없어" + RESET };
        break;

    case BuyStatus::Possessed:
        color = BRIGHT_YELLOW;
        buyContent.push_back(std::string(color) + " !! 중 복 소 유 !!" + RESET);
        buyContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        buyContent.push_back("");
        buyContent.push_back(" 이미 동일한 아이템을 가지고 있습니다.");
        buyContent.push_back(" 중복 구매는 허용되지 않습니다.");

        resultArt = { "",
            std::string(BRIGHT_YELLOW) + "      .----. " + RESET,
            std::string(BRIGHT_YELLOW) + "     |  !!  |" + RESET,
            std::string(BRIGHT_YELLOW) + "    '--[  ]--'" + RESET,
            std::string(BRIGHT_YELLOW) + "     (  ALREADY )" + RESET,
            std::string(BRIGHT_YELLOW) + "      '----' " + RESET,
            "", "    DUPLICATED" };
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

void Renderer::RenderInventory(int level, BaseItem* slots[], const std::vector<ItemSlot>& gearStorage,
    const std::vector<DiceSlot>& diceStorage, const std::vector<std::string>& diceFrame)
{
    std::vector<std::string> invContent;
    // 구분선 길이를 통일하기 위해 변수로 관리
    std::string divider = std::string(GRAY) + " ------------------------------------------" + RESET;

    invContent.push_back("");
    // 모든 섹션 제목의 들여쓰기를 공백 2칸으로 통일
    invContent.push_back(std::string(BRIGHT_YELLOW) + "  [ CHARACTER STATUS ]" + RESET);
    invContent.push_back("  - 현재 레벨 : " + std::string(BRIGHT_WHITE) + "LV. " + std::to_string(level) + RESET);
    invContent.push_back(divider);

    // [섹션 1] 현재 장착 장비
    invContent.push_back(std::string(BRIGHT_CYAN) + "  [ 현재 장착 장비 ]" + RESET);
    // 한글 글자 수를 맞추기 위해 공백을 제거하거나 통일 (무기, 헬멧... 2글자로 통일)
    const char* slotNames[] = { "무기", "헬멧", "갑옷", "신발", "반지" };

    for (int i = 0; i < 5; i++) {
        std::string line = "  - " + std::string(slotNames[i]) + "   : "; // 공백 3칸으로 콜론 위치 고정
        if (slots[i] != nullptr) {
            line += std::string(BRIGHT_YELLOW) + slots[i]->GetName() + RESET;
        }
        else {
            line += std::string(DARK_GRAY) + "---" + RESET;
        }
        invContent.push_back(line);
    }
    invContent.push_back(divider);

    // [섹션 2] 소지 장비
    invContent.push_back(std::string(BRIGHT_GREEN) + "  [ 소지 중인 장비 ]" + RESET);
    if (gearStorage.empty()) {
        invContent.push_back(std::string(DARK_GRAY) + "    (비어 있음)" + RESET);
    }
    else {
        for (int i = 0; i < std::min((int)gearStorage.size(), 5); i++) {
            std::string idx = std::string(BRIGHT_GREEN) + "[" + std::to_string(i + 1) + "] " + RESET;
            std::string count = std::string(GRAY) + " (x" + std::to_string(gearStorage[i].count) + ")" + RESET;
            invContent.push_back("    " + idx + gearStorage[i].item->GetName() + count);
        }
    }
    invContent.push_back(divider);

    // [섹션 3] 소지 주사위
    invContent.push_back(std::string(BRIGHT_YELLOW) + "  [ 소지 중인 주사위 ]" + RESET);
    if (diceStorage.empty()) {
        invContent.push_back(std::string(DARK_GRAY) + "    (비어 있음)" + RESET);
    }
    else {
        std::string dRow = "    ";
        for (const auto& ds : diceStorage) {
            dRow += std::string(BRIGHT_YELLOW) + ds.dice->DiceIdToString() + RESET +
                "(" + std::to_string(ds.count) + ") ";
        }
        invContent.push_back(dRow);
    }
    invContent.push_back(divider);

    // 종료 메뉴
    invContent.push_back("");
    invContent.push_back("  " + std::string(WHITE) + "[0] 마을로 돌아가기 (RETURN)" + RESET);

    RenderSplitScreen(invContent, diceFrame, "ADVENTURER'S BACKPACK", false);

    MoveCursor(0, 48);
    std::cout << BRIGHT_GREEN << " > 장비 번호 선택 : " << RESET;
}

void Renderer::RenderEquipResult(const EquipResult& result) {
    std::vector<std::string> equipContent;
    std::vector<std::string> equipArt;
    std::string title = "EQUIPMENT NOTIFICATION";

    // 안전한 아이템 이름 처리
    std::string itemName = (result.item != nullptr) ? result.item->GetName() : "알 수 없는 아이템";

    equipContent.push_back("");

    switch (result.status) {
    case EquipStatus::Equip:
        // [장착 성공] - BRIGHT_GREEN
        equipContent.push_back(std::string(BRIGHT_GREEN) + " [ 장 착 성 공 ]" + RESET);
        equipContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        equipContent.push_back("");
        equipContent.push_back(" 새로운 장비를 몸에 맞췄습니다.");
        equipContent.push_back("");
        equipContent.push_back(" 현재 착용: " + std::string(BRIGHT_YELLOW) + "[" + itemName + "]" + RESET);

        break;

    case EquipStatus::Changed:
        // [장비 교체] - CYAN
        equipContent.push_back(std::string(CYAN) + " [ 장 비 교 체 완 료 ]" + RESET);
        equipContent.push_back(std::string(GRAY) + " ------------------------------------------" + RESET);
        equipContent.push_back("");
        equipContent.push_back(" 기존 장비를 해제하고 교체했습니다.");
        equipContent.push_back("");
        equipContent.push_back(std::string(GRAY) + " 이전: [" + result.prevItem + "]" + RESET);
        equipContent.push_back(std::string(YELLOW) + "  V " + RESET);
        equipContent.push_back(" 현재: " + std::string(BRIGHT_CYAN) + "[" + itemName + "]" + RESET);

      
        break;

    case EquipStatus::Overlap:
        // [중복 경고] - RED
        equipContent.push_back(std::string(RED) + " !! 이 미 착 용 중 !!" + RESET);
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
