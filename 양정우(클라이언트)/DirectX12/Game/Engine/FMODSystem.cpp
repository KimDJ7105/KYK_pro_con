#include "pch.h"
#include "FMODSystem.h"


void FMODSystem::Init()
{
	FMOD::System* systemRaw;
	FMOD::System_Create(&systemRaw);
	_system = std::shared_ptr<FMOD::System>(systemRaw, [](FMOD::System* ptr) {
		ptr->release();
		});

	_system->init(512, FMOD_INIT_NORMAL, nullptr);




}

void FMODSystem::Update()
{
	_system->update();
}

void FMODSystem::Set3DListenerAttributes(int listenerIndex, const FMOD_VECTOR& position, const FMOD_VECTOR& velocity, const FMOD_VECTOR& forward, const FMOD_VECTOR& up)
{
	_system->set3DListenerAttributes(listenerIndex, &position, &velocity, &forward, &up);
}




//============================================================


void SoundManager::Init()
{
    FMOD::System_Create(&m_soundSystem);
    m_soundSystem->init(256, FMOD_INIT_NORMAL, 0);

    m_soundSystem->createSound("..\Resources\Sound\BGM_Space.mp3", FMOD_LOOP_NORMAL, 0, &m_sound[BGM_SPACE]);
    m_soundSystem->createSound("..\Resources\Sound\BGM_Desert.mp3", FMOD_LOOP_NORMAL, 0, &m_sound[BGM_DESERT]);
    m_soundSystem->createSound("..\Resources\Sound\Weapon_Assult_Fire.wav", FMOD_DEFAULT, 0, &m_sound[WEAPON_ASSULT_FIRE]);
    m_soundSystem->createSound("..\Resources\Sound\Env_Hit_Enemy.wav", FMOD_DEFAULT, 0, &m_sound[ENV_HIT_ENEMY]);
    m_soundSystem->createSound("..\Resources\Sound\Env_Eat_Item.wav", FMOD_DEFAULT, 0, &m_sound[ENV_EAT_ITEM]);
}

void SoundManager::soundPlay(int _type)
{
    m_soundSystem->playSound(m_sound[_type], 0, false, &m_channel[_type]);

    if (_type == WEAPON_ASSULT_FIRE)
    {
        m_channel[_type]->setVolume(0.5f); // 볼륨을 10%로 설정
    }
    if (_type == BGM_SPACE)
    {
        m_channel[_type]->setVolume(0.7f); // 볼륨을 10%로 설정
    }

    m_soundSystem->update();
}

void SoundManager::soundStop(int _type)
{
    m_channel[_type]->stop();
    m_soundSystem->update();
}