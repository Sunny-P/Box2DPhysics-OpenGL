#pragma once
#include <fmod.hpp>

#include <vector>
#include <stdlib.h>
#include <time.h>

class Audio {
public:
	Audio();
	~Audio();

	void Update();
	
	void playRandomBackground();
	void playSFX();
	void playMenuClick();

protected:
	bool Init();

	FMOD_RESULT result;
	FMOD::System* audioSystem;
	FMOD::Channel* channel;

	int playIndex;

	FMOD::Sound* Bone_Dance;
	FMOD::Sound* Deadlands;
	FMOD::Sound* Distant_Land;
	FMOD::Sound* Stagnant;
	FMOD::Sound* Morytania;
	std::vector<FMOD::Sound*> backgroundMusicVec;

	FMOD::Sound* creak;
	FMOD::Sound* backwardSoul;
	std::vector<FMOD::Sound*> sfxVec;
	FMOD::Sound* menuClick;

	void audioPlaySound(FMOD::Sound * _track, FMOD::Channel* _channel, float volume);
};