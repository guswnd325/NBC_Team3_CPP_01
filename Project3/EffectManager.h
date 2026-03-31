#pragma once
#include <vector>
#include <string>
#include <Windows.h>

class EffectManager {
public:
    // 흔들림 + 색상 변경 히트 효과
    static void PlayMonsterHitEffect(const std::vector<std::string>& visual, int startX, int startY, DWORD frameDist = 50);

private:
    static void Gotoxy(int x, int y);
    static void SetColor(int color);
};