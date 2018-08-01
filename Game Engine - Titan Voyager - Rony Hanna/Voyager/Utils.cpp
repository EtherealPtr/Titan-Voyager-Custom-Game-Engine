#include "Utils.h"
#include <ctime>

Utils::Utils()
{
	m_mtRandom.seed(time(0));
}

Utils::~Utils()
{}

// -------------------
// Author: Rony Hanna
// Description: Function that generates a random number between 1 and 100
// -------------------
int Utils::RandomNumBetween1And100()
{
	std::uniform_int_distribution<int> randomNum(1, 100);
	return randomNum(m_mtRandom);
}

// -------------------
// Author: Rony Hanna
// Description: Function that generates a random number between 2 float values
// -------------------
float Utils::RandomNumBetweenTwo(float _min, float _max)
{
	std::uniform_real_distribution<float> randomNum(_min, _max);
	return randomNum(m_mtRandom);
}

