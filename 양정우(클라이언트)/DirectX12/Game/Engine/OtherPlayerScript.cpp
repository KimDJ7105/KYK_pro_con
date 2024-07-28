#include "pch.h"
#include "OtherPlayerScript.h"
#include "Camera.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"

#include "Quaternion.h"

OtherPlayerScript::OtherPlayerScript()
{
}

OtherPlayerScript::~OtherPlayerScript()
{
}

void OtherPlayerScript::LateUpdate()
{
	GetPlayerGuns();

	//if (GetAnimator()->GetCurrentClipIndex() == 15)
	//{
	//	deadSequence_Start += DELTA_TIME;
	//}

	//if (0.992357 < deadSequence_Start && isRevive == false)
	//{
	//	/*GET_SINGLE(SceneManager)->RemoveObject_otherPlayer(OT_PLAYER, GetTransform()->GetObjectID());
	//	GET_SINGLE(SceneManager)->OUT_OF_RENDERING(OT_PLAYER, GetTransform()->GetObjectID());
	//	GET_SINGLE(SceneManager)->CreateHeadCoreObject(GetTransform()->GetObjectID());*/

	//	GET_SINGLE(SceneManager)->CreateHeadCoreObject(GetTransform()->GetObjectID());
	//	GET_SINGLE(SceneManager)->RemoveObject(OT_PLAYER , GetTransform()->GetObjectID());
	//	GET_SINGLE(SceneManager)->OUT_OF_RENDERING(OT_OTHER_PLAYER_MAIN , GetTransform()->GetObjectID());
	//	GET_SINGLE(SceneManager)->OUT_OF_RENDERING(OT_OTHER_PLAYER_SUB , GetTransform()->GetObjectID());
	//	nowGunObject = nullptr;
	//	isRevive = true;
	//	GET_SINGLE(SceneManager)->Update();
	//}

	uint32 state = GetAnimator()->GetCurrentClipIndex();
	if (GetTransform()->GetIsWeaponChange() == true)
	{
		if (nowGunObject != nullptr
			&& playerSubGunObject != nullptr
			&& playerMainGunObject != nullptr)
		{
			//���⸦ �����Ѵ�.
			if (nowGunObject == playerSubGunObject)
			{
				nowGunObject = playerMainGunObject;
				playerSubGunObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				GetTransform()->SetIsWeaponChange(false);
			}
			else if (nowGunObject == playerMainGunObject)
			{
				nowGunObject = playerSubGunObject;
				playerMainGunObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				GetTransform()->SetIsWeaponChange(false);
			}
		}
	}


	if (nowGunObject != nullptr)
	{
		// �÷��̾��� ���� ȸ������ �����´�
		Quaternion playerRotation = Quaternion::FromEulerAngles(GetTransform()->GetLocalRotation());

		// �Ѱ� �÷��̾��� ����� ��ġ
		Vec3 gunOffset = Vec3(5.f, 25.f, 13.f);

		shared_ptr<GameObject> gun = nowGunObject;

		// ���� �÷��̾��� �߽����� �̵��Ѵ�.
		gun->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

		// ���� ȸ���� 180�� ȸ����Ű�� ���� ���ʹϾ�
		Quaternion additionalRotation = Quaternion::FromEulerAngles(Vec3(0, 3.14f, 0));

		// ���� ȸ���� �÷��̾��� ȸ�� + �߰� ȸ������ ����
		Quaternion finalRotation = playerRotation * additionalRotation;
		gun->GetTransform()->SetLocalRotation(finalRotation.ToEulerAngles());

		// �������� ȸ���� �������� �̵�
		Vec3 rotatedOffset = finalRotation.Rotate(gunOffset);
		Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
		gun->GetTransform()->SetLocalPosition(newPosition);

	}
}

void OtherPlayerScript::GetPlayerGuns()
{
	if (playerSubGunObject == nullptr)
	{
		playerSubGunObject = GET_SINGLE(SceneManager)->GetOtherPlayerSubGun(GetTransform()->GetObjectID());
	}
	if (playerMainGunObject == nullptr)
	{
		playerMainGunObject = GET_SINGLE(SceneManager)->GetOtherPlayerMainGun(GetTransform()->GetObjectID());
	}
	if (playerSubGunObject != nullptr && isSet == false)
	{
		nowGunObject = playerSubGunObject;
		isSet = true;
	}
}
