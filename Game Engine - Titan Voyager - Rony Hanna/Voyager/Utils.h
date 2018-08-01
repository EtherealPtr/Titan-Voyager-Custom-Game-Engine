#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <random>

class Utils
{
public:
	~Utils();

	static Utils& GetInstance()
	{
		static Utils instance;
		return instance;
	}

	Utils(Utils const&) = delete;
	void operator=(Utils const&) = delete;

	int RandomNumBetween1And100();
	float RandomNumBetweenTwo(float _min, float _max);

private:
	Utils();

	std::mt19937 m_mtRandom;
};

#endif // !__UTILS_H__