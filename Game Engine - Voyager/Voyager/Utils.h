#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

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

	float RandomNumBetweenTwo(float _min, float _max);

private:
	Utils();
};

#endif // !__UTILS_H__