#include "pch.h"
#include "CrusherScript.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Timer.h"
#include "Transform.h"




CrusherScript::CrusherScript()
{
}

CrusherScript::~CrusherScript()
{
}

void CrusherScript::LateUpdate()
{
	if (isStart == false)
	{
		int crusherID = GetTransform()->GetObjectID();
		crusherScale = GetTransform()->GetLocalScale().x;
		Vec3 crusherPos = GetTransform()->GetLocalPosition();

		GET_SINGLE(SceneManager)->CreateCrusherBlade(crusherID, crusherScale, 1, crusherPos);
		GET_SINGLE(SceneManager)->CreateCrusherBlade(crusherID + 1, crusherScale, 2, crusherPos);
		GET_SINGLE(SceneManager)->CreateCrusherBlade(crusherID + 2, crusherScale, 3, crusherPos);

		isStart = true;

		auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetTransform()->GetObjectType() == OT_CRUSHER_BLADE)
			{
				if (gameObject->GetTransform()->GetObjectID() == crusherID)
				{
					blade_bottom = gameObject;
				}
				else if (gameObject->GetTransform()->GetObjectID() == crusherID + 1)
				{
					blade_middle = gameObject;
				}
				else if (gameObject->GetTransform()->GetObjectID() == crusherID + 2)
				{
					blade_upper = gameObject;
				}
			}
		}
	}



	// Crusher의 현재 회전값을 가져옴
	Quaternion crusherRotation = Quaternion::FromEulerAngles(GetTransform()->GetLocalRotation());

	// 각 blade의 오프셋 (Crusher의 위치에서의 상대적 위치)
	Vec3 bladeOffsets[3] = {
		Vec3(0.0f, -(crusherScale / 2), ((crusherScale * 1 * 2) / 5)), // blade_bottom의 오프셋
		Vec3(0.0f, -(crusherScale / 2), ((crusherScale * 2 * 2) / 5)),  // blade_middle의 오프셋
		Vec3(0.0f, -(crusherScale / 2), ((crusherScale * 3 * 2) / 5))   // blade_upper의 오프셋
	};

	shared_ptr<GameObject> blades[3] = { blade_bottom, blade_middle, blade_upper };

	for (int i = 0; i < 3; ++i)
	{
		// blade를 Crusher의 중심으로 이동
		blades[i]->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

		// blade의 회전을 Crusher의 회전으로 설정
		blades[i]->GetTransform()->SetLocalRotation(crusherRotation.ToEulerAngles());

		// 오프셋을 회전된 방향으로 이동
		Vec3 rotatedOffset = crusherRotation.Rotate(bladeOffsets[i]);

		// 원래 떨어져 있던 거리로 이동
		Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
		blades[i]->GetTransform()->SetLocalPosition(newPosition);


		//==============================


		// 현재 회전값 가져오기
		Quaternion currentRotation = Quaternion::FromEulerAngles(blades[i]->GetTransform()->GetLocalRotation());

		// X축으로 회전 추가
		Quaternion additionalRotation = QuaternionFromAxisAngle(Vec3(1, 0, 0), bladeRotationSpeed/* * DELTA_TIME*/);

		// 새로운 회전값 계산 (현재 회전 + 추가 회전)
		Quaternion newRotation = currentRotation * additionalRotation;

		// 최종 회전값 설정
		blades[i]->GetTransform()->SetLocalRotation(newRotation.ToEulerAngles());

	}



	bladeRotationSpeed += DELTA_TIME * 2;


}
