#include "pch.h"
#include "CrusherScript.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "Timer.h"
#include "Transform.h"
#include "SoundManager.h"




CrusherScript::CrusherScript()
{
	/*GET_SINGLE(SoundManager)->SetSoundProperties(CRUSHER_MOVING, 50.f, 100.f);
	GET_SINGLE(SoundManager)->soundPlay(CRUSHER_MOVING, GetTransform()->GetLocalPosition(), true);*/
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
		GET_SINGLE(SceneManager)->CreateCrusherBlade(crusherID + 100, crusherScale, 2, crusherPos);
		GET_SINGLE(SceneManager)->CreateCrusherBlade(crusherID + 200, crusherScale, 3, crusherPos);

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
				else if (gameObject->GetTransform()->GetObjectID() == crusherID + 100)
				{
					blade_middle = gameObject;
				}
				else if (gameObject->GetTransform()->GetObjectID() == crusherID + 200)
				{
					blade_upper = gameObject;
				}
			}
		}

		GET_SINGLE(SoundManager)->soundPlay(CRUSHER_MOVING, GetTransform()->GetLocalPosition(), true);

		my_NUM = GET_SINGLE(SceneManager)->GetCrusherNum();
		GET_SINGLE(SceneManager)->AddCrusherNum();
	}
	GET_SINGLE(SoundManager)->UpdateSoundPosition(CRUSHER_MOVING, my_NUM, GetTransform()->GetLocalPosition());


	// Crusher�� ���� ȸ������ ������
	Quaternion crusherRotation = Quaternion::FromEulerAngles(GetTransform()->GetLocalRotation());

	// �� blade�� ������ (Crusher�� ��ġ������ ����� ��ġ)
	Vec3 bladeOffsets[3] = {
		Vec3(0.0f, -(crusherScale / 2), ((crusherScale * 1 * 2) / 5)), // blade_bottom�� ������
		Vec3(0.0f, -(crusherScale / 2), ((crusherScale * 2 * 2) / 5)),  // blade_middle�� ������
		Vec3(0.0f, -(crusherScale / 2), ((crusherScale * 3 * 2) / 5))   // blade_upper�� ������
	};

	shared_ptr<GameObject> blades[3] = { blade_bottom, blade_middle, blade_upper };

	for (int i = 0; i < 3; ++i)
	{
		// blade�� Crusher�� �߽����� �̵�
		blades[i]->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

		// blade�� ȸ���� Crusher�� ȸ������ ����
		blades[i]->GetTransform()->SetLocalRotation(crusherRotation.ToEulerAngles());

		// �������� ȸ���� �������� �̵�
		Vec3 rotatedOffset = crusherRotation.Rotate(bladeOffsets[i]);

		// ���� ������ �ִ� �Ÿ��� �̵�
		Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
		blades[i]->GetTransform()->SetLocalPosition(newPosition);


		//==============================


		// ���� ȸ���� ��������
		Quaternion currentRotation = Quaternion::FromEulerAngles(blades[i]->GetTransform()->GetLocalRotation());

		// X������ ȸ�� �߰�
		Quaternion additionalRotation = QuaternionFromAxisAngle(Vec3(1, 0, 0), bladeRotationSpeed/* * DELTA_TIME*/);

		// ���ο� ȸ���� ��� (���� ȸ�� + �߰� ȸ��)
		Quaternion newRotation = currentRotation * additionalRotation;

		// ���� ȸ���� ����
		blades[i]->GetTransform()->SetLocalRotation(newRotation.ToEulerAngles());

	}



	bladeRotationSpeed += DELTA_TIME * 2;


}