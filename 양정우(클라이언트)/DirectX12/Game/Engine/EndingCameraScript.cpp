#include "pch.h"
#include "EndingCameraScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"

#include "session.h"

EndingCameraScript::EndingCameraScript()
{
}

EndingCameraScript::~EndingCameraScript()
{
}

void EndingCameraScript::LateUpdate()
{
	if (INPUT->GetButtonDown(KEY_TYPE::SPACEBAR))
	{
		//������ũ��Ʈ

		// �κ� ���� �ҷ�����
		GET_SINGLE(SceneManager)->LoadLobbyScene(L"LobbyScene");

		//���� ���� �ִ� ������Ʈ���� ����
		GET_SINGLE(SceneManager)->RemoveSceneObject(GET_SINGLE(SceneManager)->GetEndingScene());
	}


}
