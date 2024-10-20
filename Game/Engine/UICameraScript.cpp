#include "pch.h"
#include "UICameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "BoxCollider.h"
#include "Animator.h"

#include "session.h"

#include "Quaternion.h"

#include "SoundManager.h"

UICameraScript::UICameraScript()
{
}

UICameraScript::~UICameraScript()
{
}

void UICameraScript::LateUpdate()
{

	if (INPUT->GetButtonDown(KEY_TYPE::Q))
	{
		if (cameraMod == false)
		{
			Vec3 currentRotation = GetTransform()->GetLocalRotation();
			currentRotation.x = 1.57f;
			currentRotation.y = -1.57f;
			GetTransform()->SetLocalRotation(currentRotation);

			cameraMod = true;
		}
		else if (cameraMod == true)
		{


			Vec3 currentRotation = GetTransform()->GetLocalRotation();
			currentRotation.x = 0.f;
			currentRotation.y = 0.f;
			GetTransform()->SetLocalRotation(currentRotation);


			cameraMod = false;

		}

	}
}
