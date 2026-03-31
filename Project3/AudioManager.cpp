#include "AudioManager.h"

AudioManager& AudioManager::GetInstance()
{
	static AudioManager instance;
	return instance;
}

void AudioManager::PlayAreaBGM(std::string areaName)
{
	if (areaName == "Forest")
		AudioManager::PlayBGM(BGMList::Forest);

	else if (areaName == "Mine")
		AudioManager::PlayBGM(BGMList::Mine);

	else if (areaName == "Mountain")
		AudioManager::PlayBGM(BGMList::Mountain);

	else if (areaName == "Desert")
		AudioManager::PlayBGM(BGMList::Desert);

	else if (areaName == "Cave")
		AudioManager::PlayBGM(BGMList::Cave);

	else if (areaName == "Alley")
		AudioManager::PlayBGM(BGMList::Alley);

	else if (areaName == "Jungle")
		AudioManager::PlayBGM(BGMList::Jungle);

	else if (areaName == "Temple")
		AudioManager::PlayBGM(BGMList::Temple);

	else if (areaName == "Ocean")
		AudioManager::PlayBGM(BGMList::Ocean);

	else if (areaName == "Nest")
		AudioManager::PlayBGM(BGMList::Nest);

}

void AudioManager::PlaySFX(SFXList index)
{
	
	std::string alias = "sfx_" + std::to_string((int)index);

	
	mciSendStringA(("stop " + alias).c_str(), NULL, 0, NULL);
	mciSendStringA(("close " + alias).c_str(), NULL, 0, NULL);

	
	std::string fullPath = "./" + sfxPaths[(int)index];

	
	std::string openCommand = "open \"" + fullPath + "\" type mpegvideo alias " + alias;
	mciSendStringA(openCommand.c_str(), NULL, 0, NULL);

	//소리 변경 시 해당 볼륨 값 변경
	int targetVolume = 150;
	std::string volCmd = "setaudio " + alias + " volume to " + std::to_string(targetVolume);
	mciSendStringA(volCmd.c_str(), NULL, 0, NULL);

	
	mciSendStringA(("play " + alias + " from 0").c_str(), NULL, 0, NULL);
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
	SetVolume(defaultVolume);
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
	std::string cmd = "setaudio bgm volume to " + std::to_string(volume);
	mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}
