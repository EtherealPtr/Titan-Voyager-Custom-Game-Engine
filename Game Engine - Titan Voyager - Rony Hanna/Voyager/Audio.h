#pragma once
#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Dependencies\FMOD\Include\fmod.hpp"
#include <vector>
#include <map>

class Audio
{
public:
	Audio();
	~Audio();

	bool Init();
	bool LoadAudioFile(char* file, char* nameId);

	FMOD::Channel* GetAudioChannel() { return m_pAudioChannel; }
	FMOD::System* GetAudioManager() { return m_pAudioManager; }
	std::map<char*, FMOD::Sound*> GetSoundsMap() { return m_sounds; }

private:
	FMOD::Channel* m_pAudioChannel;
	FMOD::System* m_pAudioManager;
	std::map<char*, FMOD::Sound*> m_sounds;
};

#endif // !__AUDIO_H__