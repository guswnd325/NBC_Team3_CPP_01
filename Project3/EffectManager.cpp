#include "EffectManager.h"
#include "Renderer.h"
#include <iostream>

void EffectManager::Gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void EffectManager::SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void EffectManager::PlayMonsterHitEffect(const std::vector<std::string>& visual, int startX, int startY, DWORD frameDist) {
    int offsets[] = { -1, 1, -1, 1, 0 };

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) { // Renderer와 동일하게 8줄 고정 루프
            Gotoxy(0, startY + j); 

            if (j < (int)visual.size()) {
                
                int vLen = Renderer::GetInstance().GetVisualLength(visual[j]);
                int leftPad = (Renderer::UI_WIDTH - 2 - vLen) / 2;
                int rightPad = Renderer::UI_WIDTH - 2 - vLen - leftPad;

                
                std::string color = (i < 4) ? "\033[31m" : "\033[0m";

                
                std::cout << "\033[38;2;218;165;32m" << "│" << "\033[0m"
                    << std::string(leftPad + offsets[i], ' ')
                    << color << visual[j] << "\033[0m"
                    << std::string(rightPad - offsets[i], ' ')
                    << "\033[38;2;218;165;32m" << "│" << "\033[0m" << std::flush;
            }
            else {
                
                std::cout << "\033[38;2;218;165;32m" << "│" << "\033[0m"
                    << std::string(Renderer::UI_WIDTH - 2, ' ')
                    << "\033[38;2;218;165;32m" << "│" << "\033[0m" << std::flush;
            }
        }
        Sleep(frameDist);
    }
}