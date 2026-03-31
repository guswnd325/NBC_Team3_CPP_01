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
    "Main_Theme.wav",
    "Shop.wav"
};

enum class BGMList
{
    Main_Theme,
    Shop
};

const std::string sfxPaths[] =
{
    "dice_roll.wav",
    "Button_Lite.wav",
    "Button_Click.wav",
    "Buy_Item.wav",
    "Equip.wav",
    "Error.wav",
    "FootStep.wav",
    "Game_Complete.wav",
    "Go_Exploer.wav",
    "Heal.wav",
    "Hit.wav",
    "LevelUp.wav",
    "Reward_Fail.wav",
    "Upgrade_Dice.wav",
};

enum class SFXList
{
    Dice_Roll,

    Button_Lite,
    Button_Click,
    Buy_Item,
    Equip,
    Error,
    FootStep,
    Game_Complete,
    Go_Exploer,
    Heal,
    Hit,
    LevelUp,
    Reward_Fail,
    Upgrade_Dice,
    
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