#include <iostream>

#include "GameManager.h"

using namespace std;

int main(void)
{
	GameManager::GetInstance()->Run();

	return 0;
}