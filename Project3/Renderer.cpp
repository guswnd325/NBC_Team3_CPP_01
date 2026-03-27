#include "Renderer.h"
#include <iostream>

void Renderer::RenderMenu()
{
    
    std::cout << "┌───────────────────────────┐" << std::endl;
    std::cout << "│      MY AWESOME GAME      │" << std::endl;
    std::cout << "├───────────────────────────┤" << std::endl;
    std::cout << "│  [1] START GAME           │" << std::endl;
    std::cout << "│  [2] EXIT                 │" << std::endl;
    std::cout << "└───────────────────────────┘" << std::endl;
    std::cout << " > INPUT : " << std::endl;
   
}

void Renderer::RenderCreatePlayer()
{
    std::cout << "┌───────────────────────────────────────────┐" << std::endl;
    std::cout << "│         ◈ 캐릭터 집행자 선택 ◈            │" << std::endl;
    std::cout << "│───────────────────────────────────────────│" << std::endl;
    std::cout << "│  [1] 전  사  (안정형: 주사위 2 ~ 4)       │" << std::endl;
    std::cout << "│  [2] 마법사  (표준형: 주사위 3 ~ 5)       │" << std::endl;
    std::cout << "│  [3] 모험가  (도박형: 주사위 1 ~ 8)       │" << std::endl;
    std::cout << "└───────────────────────────────────────────┘" << std::endl;
    std::cout << " > 집행자를 선택해라 : ";
}

void Renderer::RenderMainMenu()
{
    std::cout << "┌───────────────────────────────────────────┐" << std::endl;
    std::cout << "│            ◈ 현재 행동 선택 ◈             │" << std::endl;
    std::cout << "├───────────────────────────────────────────┤" << std::endl;
    std::cout << "│  [1] 탐  사 (새로운 지역을 조사합니다)    │" << std::endl;
    std::cout << "│  [2] 상  점 (아이템을 구매하거나 팝니다)  │" << std::endl;
    std::cout << "│  [3] 휴  식 (체력을 회복하고 재정비합니다)│" << std::endl;
    std::cout << "└───────────────────────────────────────────┘" << std::endl;
    std::cout << " > 행동을 선택해라 : ";
}

void Renderer::Clear()
{
    system("cls");
}
