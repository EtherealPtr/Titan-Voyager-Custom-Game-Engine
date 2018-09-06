#pragma once
#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Dependencies\FMOD\Include\fmod.hpp"

class Audio
{
public:
	Audio();
	~Audio();

	bool Init();
	bool LoadAudioFile();

	FMOD::Channel* GetAudioChannel() { return m_pAudioChannel; }
	FMOD::System* GetAudioManager() { return m_pAudioManager; }
	FMOD::Sound* GetSound() { return m_pSound; }

private:
	FMOD::Channel* m_pAudioChannel;
	FMOD::System* m_pAudioManager;
	FMOD::Sound* m_pSound;
};

#endif // !__AUDIO_H__