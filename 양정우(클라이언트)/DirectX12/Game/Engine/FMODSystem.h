#pragma once
#include <memory>


class FMODSystem
{
	DECLARE_SINGLE(FMODSystem);

public:
	void Init();
	void Update();

	std::shared_ptr<FMOD::System> GetSystem() { return _system; }

	void Set3DListenerAttributes(int listenerIndex, const FMOD_VECTOR& position, const FMOD_VECTOR& velocity, const FMOD_VECTOR& forward, const FMOD_VECTOR& up);


private:
	std::shared_ptr<FMOD::System> _system;
};

//============================================================

enum Sounds
{
	BGM_SPACE,
	BGM_DESERT,
	WEAPON_ASSULT_FIRE,
	ENV_HIT_ENEMY,
	ENV_EAT_ITEM,
	SOUND_TYPE_COUNT
};
class SoundManager
{
	DECLARE_SINGLE(SoundManager);

private:
	FMOD::System* m_soundSystem;
	FMOD::Sound* m_sound[SOUND_TYPE_COUNT];
	FMOD::Channel* m_channel[SOUND_TYPE_COUNT];

public:
	void Init();
	void soundPlay(int enumSounds);
	void soundStop(int enumSounds);
};
