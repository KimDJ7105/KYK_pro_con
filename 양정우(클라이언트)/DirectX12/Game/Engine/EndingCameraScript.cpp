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
		//엔딩스크립트

		// 로비 씬을 불러오고
		GET_SINGLE(SceneManager)->LoadLobbyScene(L"LobbyScene");

		//엔딩 씬에 있던 오브젝트들을 제거
		GET_SINGLE(SceneManager)->RemoveSceneObject(GET_SINGLE(SceneManager)->GetEndingScene());
	}


}
