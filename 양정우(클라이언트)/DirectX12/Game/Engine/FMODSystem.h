#pragma once
#include <memory>

enum Sounds
{
	MAIN_LOBBY,
	SELECT_WEAPON,
	WEAPON_SELECTED,
	SOUND_TYPE_COUNT
};

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


class SoundManager
{
	DECLARE_SINGLE(SoundManager);

private:
	std::shared_ptr<FMOD::System> m_soundSystem;
	std::array<std::shared_ptr<FMOD::Sound>, static_cast<size_t>(Sounds::SOUND_TYPE_COUNT)> m_sound;
	std::array<std::shared_ptr<FMOD::Channel>, static_cast<size_t>(Sounds::SOUND_TYPE_COUNT)> m_channel;
public:
	void Init();
	void soundPlay(Sounds soundType);
	void soundStop(Sounds soundType);
};
