#include "pch.h"
#include "FMODSystem.h"
#include "Engine.h"


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
    FMOD::System* system;
    FMOD::System_Create(&system);
    m_soundSystem.reset(system);
    m_soundSystem->init(256, FMOD_INIT_NORMAL, nullptr);

    auto createSound = [this](const char* path, FMOD_MODE mode, Sounds sound) {
        FMOD::Sound* soundPtr;
        m_soundSystem->createSound(path, mode, nullptr, &soundPtr);
        m_sound[static_cast<size_t>(sound)].reset(soundPtr, [](FMOD::Sound* sound) {
            if (sound) {
                sound->release();
            }
            });
        };

    createSound("..\\Resources\\Sound\\BGM\\Main_Lobby_Sound.mp3", FMOD_LOOP_NORMAL, Sounds::MAIN_LOBBY);
    createSound("..\\Resources\\Sound\\UI\\Lobby_Select_Weapon.mp3", FMOD_DEFAULT, Sounds::SELECT_WEAPON);
    createSound("..\\Resources\\Sound\\UI\\Lobby_Weapon_Selected.mp3", FMOD_DEFAULT, Sounds::WEAPON_SELECTED);
}

void SoundManager::soundPlay(Sounds soundType)
{
    auto index = static_cast<size_t>(soundType);
    m_soundSystem->playSound(m_sound[index].get(), nullptr, false, nullptr);

    FMOD::Channel* channel;
    m_soundSystem->getChannel(index, &channel);
    m_channel[index].reset(channel, [](FMOD::Channel* channel) {});

    if (soundType == Sounds::SELECT_WEAPON)
    {
        m_channel[index]->setVolume(0.7f); // 볼륨을 50%로 설정
    }
    else if(soundType == Sounds::WEAPON_SELECTED)
    {
        m_channel[index]->setVolume(0.7f); // 볼륨을 50%로 설정
    }
    else if (soundType == Sounds::MAIN_LOBBY)
    {
        m_channel[index]->setVolume(0.1f); // 볼륨을 30%로 설정
    }

    m_soundSystem->update();
}

void SoundManager::soundStop(Sounds soundType)
{
    auto index = static_cast<size_t>(soundType);
    if (m_channel[index]) {
        m_channel[index]->stop();
        m_soundSystem->update();
    }
}