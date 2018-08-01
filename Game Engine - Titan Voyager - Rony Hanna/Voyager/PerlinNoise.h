#pragma once
#ifndef __PERLIN_NOISE_H__
#define __PERLIN_NOISE_H__

#include <algorithm>
#include <random>

class PerlinNoise
{
public:
	PerlinNoise() { /* Do Nothing */ }
	~PerlinNoise() { /* Do Nothing */ }

	void SetSeed(int _seed);
	double noise(double x, double y, double z);
	double Octave(double x, double y, int octaves);
	inline double OctaveNoise(double x, double y, int octaves) { return Octave(x, y, octaves) * 0.5 + 0.5; }

private:
	int m_permutation[256];

	// -------------------
	// Author: Rony Hanna
	// Description: Private function that eases coordinate values and smooth final output
	// -------------------
	double Fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

	double Lerp(double t, double a, double b) { return a + t * (b - a); }
	double Grad(int hash, double x, double y, double z);
};

// Structure for storing R, G, B values 
struct RGB
{
	double r = 0.0;
	double g = 0.0;
	double b = 0.0;

	constexpr RGB() = default;

	constexpr RGB(double _rgb)
		: r(_rgb), g(_rgb), b(_rgb)
	{}

	constexpr RGB(double _r, double _g, double _b) :
		r(_r), g(_g), b(_b) {}
};

#endif // !__PERLIN_NOISE_H__