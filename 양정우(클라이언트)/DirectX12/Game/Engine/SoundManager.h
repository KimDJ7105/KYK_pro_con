#pragma once
#include <memory>

enum Sounds
{
	MAIN_LOBBY,
	SELECT_WEAPON,
	WEAPON_SELECTED,
	SOUND_TYPE_COUNT
};

//============================================================


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
