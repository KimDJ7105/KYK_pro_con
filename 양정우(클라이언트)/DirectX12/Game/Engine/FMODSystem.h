#pragma once
#include <memory>


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

