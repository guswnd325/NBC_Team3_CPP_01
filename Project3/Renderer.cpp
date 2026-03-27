#include "Renderer.h"
#include <iostream>

void Renderer::RenderMenu()
{
    std::cout << "====================" << std::endl;
    std::cout << "   나의 게임 제목   " << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "1. 게임 시작" << std::endl;
    std::cout << "2. 게임 종료" << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "입력 : ";
}

void Renderer::RenderCreatePlayer()
{
    std::cout << "====================" << std::endl;
    std::cout << "   나의 캐릭터 직업 정하기   " << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "1. 전사(주사위 저점높게, 고점낮게(2~4))" << std::endl;
    std::cout << "2. 마법사(그냥 3~5)" << std::endl;
    std::cout << "3. (주사위 고점높게(1~8))" << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "입력 : ";
}

void Renderer::Clear()
{
    system("cls");
}
