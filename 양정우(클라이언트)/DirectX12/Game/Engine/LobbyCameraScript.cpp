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
		cl_packet_start_game sg;
		sg.type = CL_START_GAME;
		sg.size = sizeof(cl_packet_start_game);

		session->Send_Packet(&sg);

		GET_SINGLE(SceneManager)->LoadMainScene(L"TestScene");
	}
}