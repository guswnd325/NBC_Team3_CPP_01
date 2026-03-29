#pragma once
#include <iostream>
#include <vector>
#include <sstream>

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

	static std::vector<T> Split(std::string input, char delimiter) {
		std::vector<std::string> answer;
		std::string temp;
		std::stringstream ss(input);

		while (std::getline(ss, temp, delimiter)) {
			answer.push_back(temp);
		}

		return answer;
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

};
