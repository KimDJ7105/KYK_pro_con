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


	uint32 state = GetAnimator()->GetCurrentClipIndex();
	if (state == 2
		|| state == 7
		|| state == 12)
	{
		//���⸦ �����Ѵ�.
		if (nowGunObject == playerSubGunObject)
		{
			nowGunObject = playerMainGunObject;
		}
		else if (nowGunObject == playerMainGunObject)
		{
			nowGunObject = playerSubGunObject;
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
	if (playerSubGunObject != nullptr)
	{
		nowGunObject = playerSubGunObject;
	}
}
