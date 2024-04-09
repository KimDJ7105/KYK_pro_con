#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

#include "session.h"

TestCameraScript::TestCameraScript()
{
}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	//마우스 디버깅을 위해 P입력시 프로그램이 종료하도록 하였다.
	if (INPUT->GetButton(KEY_TYPE::ESC))
		PostQuitMessage(0);

	MoveUpdate();

	RotationUpdate();

	//Picking 입력을 확인
	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
	}
}

void TestCameraScript::MoveUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
	{
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;
		std::cout << "W 입력 처리중" << std::endl;
	}

	if (INPUT->GetButton(KEY_TYPE::S))
	{
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		std::cout << "S 입력 처리중" << std::endl;
	}

	if (INPUT->GetButton(KEY_TYPE::A))
	{
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		std::cout << "A 입력 처리중" << std::endl;
	}

	if (INPUT->GetButton(KEY_TYPE::D))
	{
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		std::cout << "D 입력 처리중" << std::endl;
	}

	// J가 눌렸다가 떼어지면
	if (INPUT->GetButtonUp(KEY_TYPE::J))
	{
		// 아래 내용이 실행된다.
		cs_packet_box_create bc;
		bc.size = sizeof(cs_packet_box_create);
		bc.type = CS_BOX_CREATE;
		bc.x = pos.x;
		bc.y = pos.y;
		bc.z = pos.z;

		session->Send_Packet(&bc);
	}

	cs_packet_pos_info packet;
	packet.size = sizeof(cs_packet_pos_info);
	packet.type = CS_POS_INFO;
	packet.x = pos.x;
	packet.y = pos.y;
	packet.z = pos.z;

	session->Send_Packet(&packet);

	GetTransform()->SetLocalPosition(pos);
}


void TestCameraScript::RotationUpdate()
{
	//먼저 현재 마우스의 포지션값을 읽어와서 저장한다.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	//만약 마우스가 움직임이 발생했다면
	if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	{
		//움직인 값을 저장한다.
		int moveX = nowMousePos.x - WINDOW_MIDDLE_X;
		int moveY = nowMousePos.y - WINDOW_MIDDLE_Y;

		//오른쪽
		if (moveX > 0)
		{
			Vec3 rotation = GetTransform()->GetLocalRotation();
			rotation.y += DELTA_TIME * moveX;
			GetTransform()->SetLocalRotation(rotation);
		}

		// 왼쪽
		if (moveX < 0)
		{
			Vec3 rotation = GetTransform()->GetLocalRotation();
			rotation.y += DELTA_TIME * moveX;
			GetTransform()->SetLocalRotation(rotation);
		}

		// 아래
		if (moveY > 0)
		{
			Vec3 rotation = GetTransform()->GetLocalRotation();
			rotation.x += DELTA_TIME * moveY;
			GetTransform()->SetLocalRotation(rotation);
		}

		//위
		if (moveY < 0)
		{
			Vec3 rotation = GetTransform()->GetLocalRotation();
			rotation.x += DELTA_TIME * moveY;
			GetTransform()->SetLocalRotation(rotation);
		}
	}

	//마우스의 위치를 중앙으로 초기화 해준다.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}