#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <iostream>
#include "Tools.h"

#pragma comment(lib, "winmm.lib")

// 오디오 포맷 WAV 통일, 다른 확장자의 경우 인터넷의 컨버터를 사용해 변환해주시기 바랍니다.
// MP3 to WAV : https://www.freeconvert.com/mp3-to-wav 
// OGG to WAV : https://www.freeconvert.com/ko/ogg-to-wav

const std::string bgmPaths[] =
{
    "Background Test.wav",
};

enum class BGMList
{
    Test,
};

const std::string sfxPaths[] =
{
    "SFX Test.wav",
    "dice_roll.wav",
};

enum class SFXList
{
    Test,
    dice_roll,
};

class AudioManager
{
private:
    int bgmVolume;
public:
    void PlaySFX(SFXList index);
public:
    AudioManager() { bgmVolume = 100; }
    void PlayBGM(BGMList index, bool repeat = false);
    void PauseMusic();
    void ResumeMusic();
    void StopMusic();
    void SetVolume(int volume);
public:
    static AudioManager& GetInstance();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};