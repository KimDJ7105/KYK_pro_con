#pragma once
#include <memory>
#include <vector>
#include <map>

enum Sounds
{
    //BGM
	BGM_MAIN_LOBBY,
    BGM_IN_GAME,
    BGM_SECOND_WAVE,

    //UI
	SELECT_WEAPON,
	WEAPON_SELECTED,
    GAME_START,

    //Trap
    CRUSHER_MOVING,
    LASER_CUTTER,

    //Weapon
    WEAPON_ASSAULT_RIFLE,
    WEAPON_PISTOL,
    WEAPON_SHOTGUN,
    WEAPON_SNIPER,
    WEAPON_SUB_MACHINE_GUN,

    OTHER_WEAPON_ASSAULT_RIFLE,
    OTHER_WEAPON_PISTOL,
    OTHER_WEAPON_SHOTGUN,
    OTHER_WEAPON_SNIPER,
    OTHER_WEAPON_SUB_MACHINE_GUN,

    //Player
    PLAYER_DEATH,
    PLAYER_GUN_CHANGE,
    PLAYER_GUN_RELOAD,
    PLAYER_HEADCORE_MOVE,
    PLAYER_HEAL,
    PLAYER_ITEM_PICK,
    PLAYER_RUN,
    PLAYER_WALK,
    PLAYER_REVIVE,

    OTHER_PLAYER_DEATH,
    OTHER_PLAYER_GUN_CHANGE,
    OTHER_PLAYER_GUN_RELOAD,
    OTHER_PLAYER_HEADCORE_MOVE,
    OTHER_PLAYER_HEAL,
    OTHER_PLAYER_ITEM_PICK,
    OTHER_PLAYER_RUN,
    OTHER_PLAYER_WALK,
    OTHER_PLAYER_REVIVE,

    PLAYER_HIT_01,
    PLAYER_HIT_02,
    PLAYER_HIT_03,


	SOUND_TYPE_COUNT
};

//============================================================


class SoundManager
{
	DECLARE_SINGLE(SoundManager);

private:
    FMOD::System* m_soundSystem;
    FMOD::Sound* m_sound[SOUND_TYPE_COUNT];
    std::map<int, std::vector<FMOD::Channel*>> m_channels;

public:
    void Init();
    void Update();

    void soundPlay(int _type, const Vec3& position, bool loop);

    void soundStop(int enumSounds);
    void soundStopInstance(int _type, int instanceIndex);
    void StopAllSounds();

    void UpdateListener(const Vec3& l_Position, const Vec3& l_Velocity, const Vec3& l_Forward, const Vec3& l_Up);
    void SetSoundProperties(int _type, float minDistance, float maxDistance);

    void SetChannelVolume(int _type, float volume);
    bool IsSoundPlaying(int _type);

    void UpdateSoundPosition(int _type, int instanceIndex, const Vec3& position);
};
