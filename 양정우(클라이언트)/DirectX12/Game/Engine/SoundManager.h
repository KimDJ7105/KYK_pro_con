#pragma once
#include <memory>

enum Sounds
{
	BGM_MAIN_LOBBY,
    BGM_IN_GAME,
    BGM_SECOND_WAVE,

	SELECT_WEAPON,
	WEAPON_SELECTED,

    CRUSHER_MOVING,

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
    void Update();

    void soundPlay(int _type, const Vec3& position);
    void soundStop(int enumSounds);

    void UpdateListener(const Vec3& l_Position, const Vec3& l_Velocity, const Vec3& l_Forward, const Vec3& l_Up);
    void SetSoundProperties(int _type, float minDistance, float maxDistance);
};
