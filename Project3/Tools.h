#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "AudioManager.h"

enum class InputStatus
{
	Fail,
	IndexOver,
	Success,
	Exit
};

struct InputResult
{
	InputStatus status;
	int value;
};

template <typename T>
class Tools
{
public:
	static T Clamp(T origin, T min, T max)
	{ 
		if (origin < min) return min;
		else if (origin > max) return max;
		else return origin;
	}

	// 문자열로 받은 값들이 숫자인지 확인하는 함수
	static bool IsDigit(const std::string & x)
	{
		for (int i = 0; i < x.size(); i++)
		{
			if (!isdigit(x[i]))
			{
				return false;
			}
		}
		return true;
	}

	static InputResult Input(int minLimit, int maxLimit)
	{


		InputResult result;
		int input;
		std::cin >> input;
		AudioManager::PlaySFX(SFXList::Button_Click);
		
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			result.status = InputStatus::Fail;
			return result;
		}
		else if (input == 0)
		{
			result.status = InputStatus::Exit;
			return result;
		}
		else if (!(input >= minLimit && input <= maxLimit))
		{
			result.status = InputStatus::IndexOver;
			return result;
		}
		
		else
		{
			result.status = InputStatus::Success;
			result.value = input;
			return result;
		}
	}


};
