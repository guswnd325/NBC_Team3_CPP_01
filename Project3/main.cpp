#include <iostream>
#include "GameManager.h"
#include "AudioManager.h"

using namespace std;

int main(void)
{
	AudioManager::GetInstance().PlayBGM(BGMList::Test);
	GameManager::GetInstance().Run();
	return 0;
}