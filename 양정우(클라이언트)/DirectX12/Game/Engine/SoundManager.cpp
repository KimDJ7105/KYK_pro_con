#include "pch.h"
#include "SoundManager.h"
#include "Engine.h"

//============================================================


void SoundManager::Init()
{
    FMOD::System_Create(&m_soundSystem);
    m_soundSystem->init(256, FMOD_INIT_NORMAL, 0);

    m_soundSystem->createSound("..\\Resources\\Sound\\BGM\\Main_Lobby_Sound.mp3", FMOD_LOOP_NORMAL, 0, &m_sound[MAIN_LOBBY]);
    m_soundSystem->createSound("..\\Resources\\Sound\\UI\\Lobby_Select_Weapon.mp3", FMOD_DEFAULT, 0, &m_sound[SELECT_WEAPON]);
    m_soundSystem->createSound("..\\Resources\\Sound\\UI\\Lobby_Weapon_Selected.mp3", FMOD_DEFAULT, 0, &m_sound[WEAPON_SELECTED]);
}

void SoundManager::soundPlay(int _type)
{
    m_soundSystem->playSound(m_sound[_type], 0, false, &m_channel[_type]);


    if (_type == Sounds::SELECT_WEAPON)
    {
        m_channel[_type]->setVolume(0.5f); // 볼륨을 50%로 설정
    }
    if(_type == Sounds::WEAPON_SELECTED)
    {
        m_channel[_type]->setVolume(0.5f); // 볼륨을 50%로 설정
    }
    if (_type == Sounds::MAIN_LOBBY)
    {
        m_channel[_type]->setVolume(0.3f); // 볼륨을 30%로 설정
    }

    m_soundSystem->update();
}

void SoundManager::soundStop(int _type)
{
    m_channel[_type]->stop();
    m_soundSystem->update();
}