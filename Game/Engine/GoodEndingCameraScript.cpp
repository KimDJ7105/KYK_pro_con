#include "pch.h"
#include "GoodEndingCameraScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"

#include "session.h"

GoodEndingCameraScript::GoodEndingCameraScript()
{
}

GoodEndingCameraScript::~GoodEndingCameraScript()
{
}

void GoodEndingCameraScript::LateUpdate()
{
	if (INPUT->GetButtonDown(KEY_TYPE::SPACEBAR))
	{
		//������ũ��Ʈ

		PostQuitMessage(0);

		//// �κ� ���� �ҷ�����
		//GET_SINGLE(SceneManager)->LoadLobbyScene(L"LobbyScene");

		////���� ���� �ִ� ������Ʈ���� ����
		//GET_SINGLE(SceneManager)->RemoveSceneObject(GET_SINGLE(SceneManager)->GetGoodEndingScene());
	}


}