#include "Renderer.h"
#include <iostream>

#define GOLD    "\033[38;2;218;165;32m"
#define BRIGHT_GREEN "\033[92m"
#define WHITE   "\033[37m"
#define RED     "\033[31m"
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"

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

void Renderer::RenderMenu()
{

    std::cout << GOLD << "┌───────────────────────────┐" << RESET << std::endl;
    std::cout << GOLD << "│        " << WHITE << "Dice Agent" << GOLD << "         │" << RESET << std::endl;
    std::cout << GOLD << "├───────────────────────────┤" << RESET << std::endl;
    std::cout << GOLD << "│  " << YELLOW << "[1]" << WHITE << " START GAME           " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│  " << YELLOW << "[2]" << RED << " EXIT                 " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "└───────────────────────────┘" << RESET << std::endl;
    std::cout << BRIGHT_GREEN << " > INPUT : " << RESET;

}

void Renderer::RenderCreatePlayer()
{
    std::cout << GOLD << "┌───────────────────────────────────────────┐" << RESET << std::endl;
    std::cout << GOLD << "│         " << WHITE << "◈ 캐릭터 집행자 선택 ◈" << GOLD << "            │" << std::endl;
    std::cout << GOLD << "│───────────────────────────────────────────│" << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[1] 전  사  (안정형: 주사위 2 ~ 4)       " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[2] 마법사  (표준형: 주사위 3 ~ 5)       " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[3] 모험가  (도박형: 주사위 1 ~ 8)       " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "└───────────────────────────────────────────┘" << RESET << std::endl;
    std::cout << BRIGHT_GREEN << " > 집행자를 선택해라 : " << RESET;
}

void Renderer::RenderMainMenu()
{
    std::cout << GOLD << "┌───────────────────────────────────────────┐" << RESET << std::endl;
    std::cout << GOLD << "│            " << WHITE << "◈ 현재 행동 선택 ◈" << GOLD << "             │" << RESET << std::endl;
    std::cout << GOLD << "├───────────────────────────────────────────┤" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[1] 탐  사 (새로운 지역을 조사합니다)    " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[2] 상  점 (아이템을 구매하거나 팝니다)  " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[3] 휴  식 (체력을 회복하고 재정비합니다)" << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "└───────────────────────────────────────────┘" << RESET << std::endl;
    std::cout << BRIGHT_GREEN << " > 행동을 선택해라 : " << RESET;
}

void Renderer::RenderRewardSelect() {
    std::cout << GOLD << "┌───────────────────────────────┐" << RESET << std::endl;
    std::cout << GOLD << "│         " << WHITE << "◈ 보상 선택 ◈          " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "├───────────────────────────────┤" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[1] 일반 보상                 " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│      " << WHITE << "(휴식권 1회 + 골드)        " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[2] 리스크 보상               " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│      " << WHITE << "(주사위로 고보상 도전)     " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "└───────────────────────────────┘" << RESET << std::endl;
    std::cout << BRIGHT_GREEN << " > 보상을 선택해라 : " << RESET;
}
void Renderer::RenderBattleAction() {
    std::cout << GOLD << "┌───────────────────────────┐" << RESET << std::endl;
    std::cout << GOLD << "│       " << WHITE << "◈ 행동 선택 ◈        " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "├───────────────────────────┤" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[1] 전투                  " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "│  " << WHITE << "[2] 도망                  " << GOLD << "│" << RESET << std::endl;
    std::cout << GOLD << "└───────────────────────────┘" << RESET << std::endl;
	std::cout << BRIGHT_GREEN << " > 행동을 선택해라 : " << RESET;

}
void Renderer::Clear()
{
    system("cls");
}
