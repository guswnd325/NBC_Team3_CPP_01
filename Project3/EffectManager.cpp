#include "EffectManager.h"
#include "Renderer.h"
#include <iostream>

#define HIT_COLOR "\033[91m"
#define RESET_COLOR "\033[0m"

void EffectManager::Gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void EffectManager::SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}



void EffectManager::PlayMonsterHitEffect(const std::vector<std::string>& visual, int startX, int startY, DWORD frameDist) {
    const int RENDER_HEIGHT = 7;
    int offsets[] = { -2, 2, -1, 1, 0 };

    for (int i = 0; i < 5; i++) {
        
        std::string frameColor = (i < 4) ? HIT_COLOR : RESET_COLOR;

        for (int j = 0; j < RENDER_HEIGHT; j++) {
            Gotoxy(0, startY + j);

            std::cout << GOLD << "¦¢" << RESET; 

            if (j < (int)visual.size()) {
                int vLen = Renderer::GetInstance().GetVisualLength(visual[j]);
                int baseLeft = 1;
                int finalLeft = (baseLeft + offsets[i] >= 0) ? (baseLeft + offsets[i]) : 0;
                int finalRight = Renderer::LEFT_WIDTH - vLen - finalLeft;
                finalRight = (finalRight > 0) ? finalRight : 0;

                std::cout << std::string(finalLeft, ' ')
                    << frameColor << visual[j] << RESET_COLOR
                    << std::string(finalRight, ' ');
            }
            else {
                std::cout << std::string(Renderer::LEFT_WIDTH, ' ');
            }

            std::cout << GOLD << "¦¢" << RESET
                << std::string(Renderer::RIGHT_WIDTH, ' ')
                << GOLD << "¦¢" << RESET;
        }
        std::cout << std::flush;
        Sleep(frameDist);
    }
}