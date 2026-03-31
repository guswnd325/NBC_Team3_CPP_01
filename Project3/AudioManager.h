#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <iostream>

#pragma comment(lib, "winmm.lib")

// 오디오 포맷 WAV 통일, 다른 확장자의 경우 인터넷의 컨버터를 사용해 변환해주시기 바랍니다.
// MP3 to WAV : https://www.freeconvert.com/mp3-to-wav 
// OGG to WAV : https://www.freeconvert.com/ko/ogg-to-wav

constexpr int defaultVolume = 100;

static const std::string bgmPaths[] =
{
    "Main_Theme.wav",
    "Shop.wav",
    "jane.wav"

    "Forest.wav",
    "Desert.wav",
    "Cave.wav",
    "Temple.wav",
    "Jungle.wav",
    "Mine.wav",
    "Ocean.wav",
    "Alley.wav",
    "Mountain.wav",
    "Nest.wav",
};

enum class BGMList
{
    Main_Theme,
    Shop,
    Jane,

    Forest,
    Desert,
    Cave,
    Temple,
    Jungle,
    Mine,
    Ocean,
    Alley,
    Mountain,
    Nest
};

static const std::string sfxPaths[] =
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
    static void PlaySFX(SFXList index);
    static void PlayBGM(BGMList index, bool repeat = true);
    static void PlayAreaBGM(std::string areaName);

public:
    AudioManager() { bgmVolume = 100; }
    void PauseMusic();
    void ResumeMusic();
    void StopMusic();
    static void SetVolume(int volume);
public:
    static AudioManager& GetInstance();

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
};