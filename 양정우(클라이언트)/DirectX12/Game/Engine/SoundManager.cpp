#include "pch.h"
#include "SoundManager.h"
#include "Engine.h"

//============================================================


void SoundManager::Init()
{
    FMOD::System_Create(&m_soundSystem);
    m_soundSystem->init(256, FMOD_INIT_NORMAL, 0);

    //BGM
    m_soundSystem->createSound("..\\Resources\\Sound\\BGM\\Main_Lobby_Sound.mp3", FMOD_LOOP_NORMAL, 0, &m_sound[BGM_MAIN_LOBBY]);
    m_soundSystem->createSound("..\\Resources\\Sound\\BGM\\In_Game_BGM.mp3", FMOD_LOOP_NORMAL, 0, &m_sound[BGM_IN_GAME]);
    m_soundSystem->createSound("..\\Resources\\Sound\\BGM\\SecondWave_BGM.mp3", FMOD_LOOP_NORMAL, 0, &m_sound[BGM_SECOND_WAVE]);

    //UI
    m_soundSystem->createSound("..\\Resources\\Sound\\UI\\Lobby_Select_Weapon.mp3", FMOD_DEFAULT, 0, &m_sound[SELECT_WEAPON]);
    m_soundSystem->createSound("..\\Resources\\Sound\\UI\\Lobby_Weapon_Selected.mp3", FMOD_DEFAULT, 0, &m_sound[WEAPON_SELECTED]);
    m_soundSystem->createSound("..\\Resources\\Sound\\UI\\Lobby_Game_Start.mp3", FMOD_DEFAULT, 0, &m_sound[GAME_START]);

    //Trap
    m_soundSystem->createSound("..\\Resources\\Sound\\Trap\\Crusher_Moving.mp3", FMOD_3D, 0, &m_sound[CRUSHER_MOVING]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Trap\\Laser_Cutter.mp3", FMOD_3D, 0, &m_sound[LASER_CUTTER]);

    //Weapon
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\AssaultRifle_Fire.mp3", FMOD_DEFAULT, 0, &m_sound[WEAPON_ASSAULT_RIFLE]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\Pistol_Fire.mp3", FMOD_DEFAULT, 0, &m_sound[WEAPON_PISTOL]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\Shotgun_Fire.mp3", FMOD_DEFAULT, 0, &m_sound[WEAPON_SHOTGUN]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\Sniper_Fire.mp3", FMOD_DEFAULT, 0, &m_sound[WEAPON_SNIPER]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\SubMachineGun_Fire.mp3", FMOD_DEFAULT, 0, &m_sound[WEAPON_SUB_MACHINE_GUN]);

    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\AssaultRifle_Fire.mp3", FMOD_3D, 0, &m_sound[OTHER_WEAPON_ASSAULT_RIFLE]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\Pistol_Fire.mp3", FMOD_3D, 0, &m_sound[OTHER_WEAPON_PISTOL]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\Shotgun_Fire.mp3", FMOD_3D, 0, &m_sound[OTHER_WEAPON_SHOTGUN]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\Sniper_Fire.mp3", FMOD_3D, 0, &m_sound[OTHER_WEAPON_SNIPER]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Weapon\\SubMachineGun_Fire.mp3", FMOD_3D, 0, &m_sound[OTHER_WEAPON_SUB_MACHINE_GUN]);

    //Player
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Death_Sound.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_DEATH]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Gun_Change.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_GUN_CHANGE]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Gun_Reload.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_GUN_RELOAD]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\HeadCore_Move.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_HEADCORE_MOVE]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Heal_Sound.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_HEAL]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Item_Pickup.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_ITEM_PICK]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Player_Run_Sound.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_RUN]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Player_Walk_Sound.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_WALK]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Revive_Sound.mp3", FMOD_DEFAULT, 0, &m_sound[PLAYER_REVIVE]);

    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Death_Sound.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_DEATH]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Gun_Change.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_GUN_CHANGE]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Gun_Reload.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_GUN_RELOAD]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\HeadCore_Move.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_HEADCORE_MOVE]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Heal_Sound.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_HEAL]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Item_Pickup.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_ITEM_PICK]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Player_Run_Sound.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_RUN]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Player_Walk_Sound.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_WALK]);
    m_soundSystem->createSound("..\\Resources\\Sound\\Player\\Revive_Sound.mp3", FMOD_3D, 0, &m_sound[OTHER_PLAYER_REVIVE]);
}

void SoundManager::Update()
{
    m_soundSystem->update();
}

void SoundManager::soundPlay(int _type, const Vec3& position, bool loop)
{

    FMOD_VECTOR pos = FMOD_VECTOR(position.x, position.y, position.z);


    if (loop)
    {
        m_sound[_type]->setMode(FMOD_LOOP_NORMAL);
    }
    else
    {
        m_sound[_type]->setMode(FMOD_LOOP_OFF);
    }



    FMOD::Channel* channel;
    FMOD_RESULT result = m_soundSystem->playSound(m_sound[_type], 0, false, &channel);

    if (result == FMOD_OK && channel)
    {
        channel->set3DAttributes(&pos, nullptr);
        

        // 사운드의 3D 속성을 설정
        SetSoundProperties(_type, 20.0f, 1500.0f);//->내가 임의로 적은 속성(10.0f, 1000.0f)

        if (_type == Sounds::SELECT_WEAPON || _type == Sounds::WEAPON_SELECTED)
        {
            channel->setVolume(0.3f);
        }
        else if (_type == Sounds::BGM_MAIN_LOBBY)
        {
            channel->setVolume(0.2f);
        }
        else if (_type == Sounds::CRUSHER_MOVING)
        {
            channel->setVolume(0.8f);
        }
        else if (_type == Sounds::PLAYER_GUN_CHANGE)
        {
            channel->setVolume(0.5f);
        }
        else if (_type == Sounds::WEAPON_ASSAULT_RIFLE
            || WEAPON_PISTOL
            || WEAPON_SHOTGUN
            || WEAPON_SNIPER
            || WEAPON_SUB_MACHINE_GUN)
        {
            channel->setVolume(0.3f);
        }

        m_channels[_type].push_back(channel); // 채널을 저장
    }
    else
    {
        std::cerr << "Error playing sound: " << FMOD_ErrorString(result) << std::endl;
    }
    

    //m_soundSystem->update();
}

void SoundManager::soundStop(int _type)
{
    for (auto& channel : m_channels[_type])
    {
        if (channel)
        {
            channel->stop();
        }
    }
    m_channels[_type].clear();
    //m_soundSystem->update();
}

void SoundManager::soundStopInstance(int _type, int instanceIndex)
{
    if (instanceIndex >= 0 && instanceIndex < m_channels[_type].size())
    {
        if (m_channels[_type][instanceIndex])
        {
            m_channels[_type][instanceIndex]->stop();
            m_channels[_type].erase(m_channels[_type].begin() + instanceIndex);
            //m_soundSystem->update(); // Ensure the system is updated
        }
    }
}

void SoundManager::StopAllSounds()
{
    for (auto& [type, channels] : m_channels)
    {
        for (auto& channel : channels)
        {
            if (channel)
            {
                channel->stop();
            }
        }
        channels.clear();
    }
    m_soundSystem->update(); // Ensure the system is updated
}

void SoundManager::UpdateListener(const Vec3& l_Position, const Vec3& l_Velocity, const Vec3& l_Forward, const Vec3& l_Up)
{

    FMOD_VECTOR listenerPosition = { l_Position.x, l_Position.y, l_Position.z };
    FMOD_VECTOR listenerVelocity = { l_Velocity.x, l_Velocity.y, l_Velocity.z };
    FMOD_VECTOR listenerForward = { l_Forward.x, l_Forward.y, l_Forward.z };
    FMOD_VECTOR listenerUp = { l_Up.x, l_Up.y, l_Up.z };


    //m_soundSystem->set3DListenerAttributes(0, &listenerPosition, &listenerVelocity, &listenerForward, &listenerUp);
    m_soundSystem->set3DListenerAttributes(0, &listenerPosition, nullptr, &listenerForward, &listenerUp);
}

void SoundManager::SetSoundProperties(int _type, float minDistance, float maxDistance)
{
    FMOD::Sound* sound = m_sound[_type];
    if (sound)
    {
        sound->set3DMinMaxDistance(minDistance, maxDistance);
    }
}

void SoundManager::SetChannelVolume(int _type, float volume)
{
    for (auto& channel : m_channels[_type])
    {
        if (channel)
        {
            channel->setVolume(volume);
        }
    }
}

bool SoundManager::IsSoundPlaying(int _type)
{
    for (auto& channel : m_channels[_type])
    {
        bool isPlaying = false;
        if (channel && channel->isPlaying(&isPlaying) == FMOD_OK && isPlaying)
        {
            return true;
        }
    }
    return false;
}

void SoundManager::UpdateSoundPosition(int _type, int instanceIndex, const Vec3& position)
{
    if (instanceIndex >= 0 && instanceIndex < m_channels[_type].size())
    {
        FMOD_VECTOR pos = { position.x, position.y, position.z };
        if (m_channels[_type][instanceIndex])
        {
            m_channels[_type][instanceIndex]->set3DAttributes(&pos, nullptr);
            m_soundSystem->update(); // Ensure the system is updated
        }
    }
}
