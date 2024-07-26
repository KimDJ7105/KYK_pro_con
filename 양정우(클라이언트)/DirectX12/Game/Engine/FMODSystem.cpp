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