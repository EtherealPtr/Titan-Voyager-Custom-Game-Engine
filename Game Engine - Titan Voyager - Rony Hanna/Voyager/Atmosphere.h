#pragma once
#ifndef __ATMOSPHERE_H__
#define __ATMOSPHERE_H__

class Atmosphere
{
public:
	Atmosphere();
	~Atmosphere();

	void Update(float dt);

	void SetFlashDuration(int duration)	{ m_flashDuration = duration; }
	void SetThunderFlash(bool flash)	{ m_thunderFlash = flash; }
	void SetFlashTimer(float timer)		{ m_flashTimer = timer; }
	void Restart();

	bool& GetThunderstorm()				{ return m_playThunderstorm; }
	int& GetFlashDuration()				{ return m_flashDuration; }
	bool& GetThunderFlash()				{ return m_thunderFlash; }
	float& GetFlashTimer()				{ return m_flashTimer; }
	float GetDayTime()					{ return m_dayTimer; }

private:
	float m_thunderstormTimer;
	bool m_activateThunderstorms, m_playThunderstorm, m_thunderFlash, m_nightMode;
	int m_flashDuration;
	float m_flashTimer;
	float m_dayTimer;

	// Private functions
	void Thunderstorm();
};

#endif // !__ATMOSPHERE_H__