#include "AudioManager.h"

AudioManager& AudioManager::GetInstance()
{
	static AudioManager instance;
	return instance;
}

void AudioManager::PlaySFX(SFXList index)
{
	std::string fullPath = "./" + sfxPaths[(int)index];
	PlaySoundA(fullPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

void AudioManager::PlayBGM(BGMList index, bool repeat)
{
	mciSendStringA("close bgm", NULL, 0, NULL);

	std::string fullPath = "./" + bgmPaths[(int)index];
	std::string openCommand = "open \"" + fullPath + "\" type mpegvideo alias bgm";

	mciSendStringA(openCommand.c_str(), NULL, 0, NULL);
	
	if (repeat)
	{
		mciSendStringA("play bgm repeat", NULL, 0, NULL);
	}
	else
	{
		mciSendStringA("play bgm", NULL, 0, NULL);
	}
	SetVolume(bgmVolume);
}

void AudioManager::PauseMusic()
{
	//mciSendStringA("pause bgm", NULL, 0, NULL);
}

void AudioManager::ResumeMusic()
{
	//mciSendStringA("resume bgm", NULL, 0, NULL);
}

void AudioManager::StopMusic()
{
	mciSendStringA("stop bgm", NULL, 0, NULL);
	mciSendStringA("close bgm", NULL, 0, NULL);
}

void AudioManager::SetVolume(int volume)
{
	volume = Tools<int>::Clamp(volume, 0, 1000);

	std::string cmd = "setaudio bgm volume to " + std::to_string(volume);
	mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}
