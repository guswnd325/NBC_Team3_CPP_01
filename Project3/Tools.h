#pragma once

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

};
