#include <iostream>
#include "GameManager.h"
#include "AudioManager.h"

using namespace std;

int main(void)
{
	AudioManager::GetInstance().PlayBGM(BGMList::Main_Theme);
	GameManager::GetInstance().Run();
	return 0;
}