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
#include "SoundManager.h"

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

	if (GetAnimator()->GetCurrentClipIndex() == 4
		|| GetAnimator()->GetCurrentClipIndex() == 9
		|| GetAnimator()->GetCurrentClipIndex() == 14)
	{

		isFired = true;

		if (nowGunObject == playerSubGunObject)
		{
			gunShot = 0.333f;
			if (gunShot <= fireTimeElapse)
			{
				GET_SINGLE(SoundManager)->soundPlay(OTHER_WEAPON_PISTOL, GetTransform()->GetLocalPosition(), false);
				fireTimeElapse = 0.f;
			}
				
			
		}
		else if (nowGunObject == playerMainGunObject)
		{
			if (myGunType == GT_AR)
			{
				gunShot = 0.125;
				if (gunShot <= fireTimeElapse)
				{
					GET_SINGLE(SoundManager)->soundPlay(OTHER_WEAPON_ASSAULT_RIFLE, GetTransform()->GetLocalPosition(), false);
					fireTimeElapse = 0.f;
				}
			}
			else if (myGunType == GT_SM)
			{
				gunShot = 0.1f;
				if (gunShot <= fireTimeElapse)
				{
					GET_SINGLE(SoundManager)->soundPlay(OTHER_WEAPON_SUB_MACHINE_GUN, GetTransform()->GetLocalPosition(), false);
					fireTimeElapse = 0.f;
				}
			}
			else if (myGunType == GT_SG)
			{
				gunShot = 1.f;
				if (gunShot <= fireTimeElapse)
				{
					GET_SINGLE(SoundManager)->soundPlay(OTHER_WEAPON_SHOTGUN, GetTransform()->GetLocalPosition(), false);
					fireTimeElapse = 0.f;
				}
			}
			else if (myGunType == GT_SR)
			{
				gunShot = 1.f;
				if (gunShot <= fireTimeElapse)
				{
					GET_SINGLE(SoundManager)->soundPlay(OTHER_WEAPON_SNIPER, GetTransform()->GetLocalPosition(), false);
					fireTimeElapse = 0.f;
				}
			}
		}
	}


	if (GetAnimator()->GetCurrentClipIndex() == 5)
	{
		if (GET_SINGLE(SoundManager)->IsSoundPlaying(OTHER_PLAYER_WALK) == false)
			GET_SINGLE(SoundManager)->soundPlay(OTHER_PLAYER_WALK, GetTransform()->GetLocalPosition(), false);
	}
	else if (GetAnimator()->GetCurrentClipIndex() == 10)
	{
		if (GET_SINGLE(SoundManager)->IsSoundPlaying(OTHER_PLAYER_RUN) == false)
			GET_SINGLE(SoundManager)->soundPlay(OTHER_PLAYER_RUN, GetTransform()->GetLocalPosition(), false);
	}
	else if(GetAnimator()->GetCurrentClipIndex() == 0)
	{
		GET_SINGLE(SoundManager)->soundStop(OTHER_PLAYER_WALK);
		GET_SINGLE(SoundManager)->soundStop(OTHER_PLAYER_RUN);
	}


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


	fireTimeElapse += DELTA_TIME;
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
		myGunType = GetTransform()->GetGunType();
	}
	if (playerSubGunObject != nullptr && isSet == false)
	{
		nowGunObject = playerSubGunObject;
		isSet = true;
	}
}
