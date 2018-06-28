#include "Utils.h"
#include <ctime>
#include <cstdlib>

Utils::Utils()
{}

Utils::~Utils()
{}

// -------------------
// Author: Rony Hanna
// Description: Function that generates a random number between 2 float values
// -------------------
float Utils::RandomNumBetweenTwo(float _min, float _max)
{
	float result = (float)rand() / (double)RAND_MAX;
	return _min + result * (_max - _min);
}