#include "pch.h"
#include "LaserScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"

#include "SoundManager.h"

LaserScript::LaserScript()
{
	
}

LaserScript::~LaserScript()
{
}

void LaserScript::LateUpdate()
{
	if (isSet == false)
	{
		GET_SINGLE(SoundManager)->soundPlay(LASER_CUTTER, GetTransform()->GetLocalPosition(), true);
		my_NUM = GET_SINGLE(SceneManager)->GetLaserNum();
		GET_SINGLE(SceneManager)->AddLaserNum();
		isSet = true;
	}
	

	if (isSet == true)
	{
		GET_SINGLE(SoundManager)->UpdateSoundPosition(LASER_CUTTER, my_NUM, GetTransform()->GetLocalPosition());

	}


}
