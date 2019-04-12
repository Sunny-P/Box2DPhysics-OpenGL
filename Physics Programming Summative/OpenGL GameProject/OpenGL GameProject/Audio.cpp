#pragma once
#include "Audio.h"
#include <iostream>

Audio::Audio()
{
	Init();

	/*result = audioSystem->createSound("Resources/Audio/Bone_Dance.ogg", FMOD_LOOP_NORMAL, 0, &Bone_Dance);
	backgroundMusicVec.push_back(Bone_Dance);
	result = audioSystem->createSound("Resources/Audio/Deadlands.ogg", FMOD_LOOP_NORMAL, 0, &Deadlands);
	backgroundMusicVec.push_back(Deadlands);
	result = audioSystem->createSound("Resources/Audio/Distant_Land.ogg", FMOD_LOOP_NORMAL, 0, &Distant_Land);
	backgroundMusicVec.push_back(Distant_Land);
	result = audioSystem->createSound("Resources/Audio/Stagnant.ogg", FMOD_LOOP_NORMAL, 0, &Stagnant);
	backgroundMusicVec.push_back(Stagnant);
	result = audioSystem->createSound("Resources/Audio/Morytania.ogg", FMOD_LOOP_NORMAL, 0, &Morytania);
	backgroundMusicVec.push_back(Morytania);

	result = audioSystem->createSound("Resources/Audio/BackwardsSouls.wav", FMOD_DEFAULT, 0, &backwardSoul);
	sfxVec.push_back(backwardSoul);
	result = audioSystem->createSound("Resources/Audio/Creak.wav", FMOD_DEFAULT, 0, &creak);
	sfxVec.push_back(creak);

	result = audioSystem->createSound("Resources/Audio/Creak.wav", FMOD_DEFAULT, 0, &menuClick);*/
}

Audio::~Audio()
{

}

void Audio::Update()
{
	audioSystem->update();
}

bool Audio::Init()
{
	result = FMOD::System_Create(&audioSystem);
	if (result != FMOD_OK)
	{
		return false;
	}

	result = audioSystem->init(100, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		return false;
	}

	return true;
}

void Audio::playRandomBackground()
{
	playIndex = rand() % backgroundMusicVec.size();
	audioPlaySound(backgroundMusicVec.at(playIndex), channel, 0.3f);
}

void Audio::playSFX()
{
	for (unsigned int i = 0; i < sfxVec.size(); ++i)
	{
		audioPlaySound(sfxVec.at(i), channel, 0.3f);
	}
}

void Audio::playMenuClick()
{
	audioPlaySound(menuClick, channel, 0.3f);
}

void Audio::audioPlaySound(FMOD::Sound * _track, FMOD::Channel * _channel, float volume)
{
	//std::cout << "Audio Play Sound" << std::endl;
	result = audioSystem->playSound(_track, 0, false, 0);
	result = channel->setVolume(volume);
}
