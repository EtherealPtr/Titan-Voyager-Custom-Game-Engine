#include "Audio.h"
#include <stdio.h>

Audio::Audio()
{}

Audio::~Audio()
{}

bool Audio::Init()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&m_pAudioManager);

	if (result != FMOD_OK)
	{
		printf("FMOD ERROR: Unable to initialize FMOD library");
		return false;
	}
	
	result = m_pAudioManager->init(50, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
	{
		printf("FMOD ERROR: Unable to initialize FMOD library");
		return false;
	}

	return true;
}

bool Audio::LoadAudioFile(char* file, char* nameId)
{
	FMOD_RESULT result;

	FMOD::Sound* pSound;
	result = m_pAudioManager->createSound(file, FMOD_DEFAULT, nullptr, &pSound);

	if (result != FMOD_OK)
	{
		printf("FMOD ERROR: Unable to create sound, ensure file path is accurate");
		return false;
	}

	m_sounds[nameId] = pSound;
	
	//m_pBackgroundMusic->setMode(FMOD_LOOP_NORMAL);
	//m_AudioManager->playSound(m_JumpSound, 0, false, &m_FmodChannel);

	return true;
}

bool Audio::IsSoundPlaying()
{
	bool bIsPlaying;
	m_pAudioChannel->isPlaying(&bIsPlaying);
	return bIsPlaying;
}