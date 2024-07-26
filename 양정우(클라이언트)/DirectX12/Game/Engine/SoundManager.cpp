#include "pch.h"
#include "SoundManager.h"
#include "Engine.h"

//============================================================


void SoundManager::Init()
{
    FMOD::System_Create(&m_soundSystem);
    m_soundSystem->init(256, FMOD_INIT_NORMAL, 0);

    //BGM
    m_soundSystem->createSound("..\\Resources\\Sound\\BGM\\Main_Lobby_Sound.mp3", FMOD_3D, 0, &m_sound[BGM_MAIN_LOBBY]);
    m_soundSystem->createSound("..\\Resources\\Sound\\BGM\\In_Game_BGM.mp3", FMOD_3D, 0, &m_sound[BGM_IN_GAME]);
    m_soundSystem->createSound("..\\Resources\\Sound\\BGM\\SecondWave_BGM.mp3", FMOD_3D, 0, &m_sound[BGM_SECOND_WAVE]);

    //UI
    m_soundSystem->createSound("..\\Resources\\Sound\\UI\\Lobby_Select_Weapon.mp3", FMOD_DEFAULT, 0, &m_sound[SELECT_WEAPON]);
    m_soundSystem->createSound("..\\Resources\\Sound\\UI\\Lobby_Weapon_Selected.mp3", FMOD_DEFAULT, 0, &m_sound[WEAPON_SELECTED]);

    //Trap
    m_soundSystem->createSound("..\\Resources\\Sound\\Trap\\Crusher_Moving.mp3", FMOD_3D, 0, &m_sound[CRUSHER_MOVING]);
}

void SoundManager::Update()
{
    m_soundSystem->update();
}

void SoundManager::soundPlay(int _type, const Vec3& position, bool loop)
{

    FMOD_VECTOR pos = FMOD_VECTOR(position.x, position.y, position.z);

    if (m_channel[_type])
    {
        m_channel[_type]->stop();
    }


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
        m_channel[_type] = channel;
        m_channel[_type]->set3DAttributes(&pos, nullptr);

        if (_type == Sounds::SELECT_WEAPON || _type == Sounds::WEAPON_SELECTED)
        {
            m_channel[_type]->setVolume(0.5f);
        }
        else if (_type == Sounds::BGM_MAIN_LOBBY)
        {
            m_channel[_type]->setVolume(0.3f);
        }
    }
    else
    {
        std::cerr << "Error playing sound: " << FMOD_ErrorString(result) << std::endl;
    }
    

    //m_soundSystem->update();
}

void SoundManager::soundStop(int _type)
{
    m_channel[_type]->stop();
    //m_soundSystem->update();
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

bool SoundManager::IsSoundPlaying(int _type)
{
    if (m_channel[_type])
    {
        bool isPlaying = false;
        m_channel[_type]->isPlaying(&isPlaying);
        return isPlaying;
    }
    return false;
}

void SoundManager::UpdateSoundPosition(int _type, const Vec3& position)
{
    if (m_channel[_type])
    {
        FMOD_VECTOR pos = { position.x, position.y, position.z };
        m_channel[_type]->set3DAttributes(&pos, nullptr);
    }
}
