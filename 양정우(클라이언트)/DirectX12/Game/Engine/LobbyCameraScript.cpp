#include "pch.h"
#include "LobbyCameraScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"

#include "session.h"

LobbyCameraScript::LobbyCameraScript()
{
}

LobbyCameraScript::~LobbyCameraScript()
{
}


void LobbyCameraScript::LateUpdate()
{
	if (INPUT->GetButtonDown(KEY_TYPE::SPACEBAR))
	{
		GET_SINGLE(SceneManager)->LoadMainScene(L"TestScene");


		//당근칼
		// 아래에는 Main Scene과 연결할 Main Server에 대한 코드
	}
}