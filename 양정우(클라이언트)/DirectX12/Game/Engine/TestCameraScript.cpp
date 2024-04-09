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
	KeyboardUpdate();

	MouseUpdate();

	
}

void TestCameraScript::KeyboardUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	//마우스 디버깅을 위해 P입력시 프로그램이 종료하도록 하였다.
	if (INPUT->GetButton(KEY_TYPE::P))
		PostQuitMessage(0);

	//이곳에 버튼이 눌러졌을때와 떼어졌을때를 구분해야 더 정확한 움직임이 가능하다.

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;


	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
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


void TestCameraScript::MouseUpdate()
{

	//마우스 입력에 따른 회전
	//현재 구조는 매 프레임마다 마우스의 위치를 기억한다.
	//그러므로 중앙으로부터 마우스가 얼마나 움직였는지를 알아내서
	//움직인 값만큼 회전에 곱해주면 되지않을까 싶다.

	
	//먼저 현재 마우스의 포지션값을 읽어와서 저장한다.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	// DEBUG
	//std::cout << "현재 마우스 위치 : {" << nowMousePos.x << ", " << nowMousePos.y << "}" << std::endl;

	//만약 마우스가 움직임이 발생했다면
	if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	{
		//움직인 값을 저장한다.
		int moveX = nowMousePos.x - WINDOW_MIDDLE_X;
		int moveY = nowMousePos.y - WINDOW_MIDDLE_Y;

		// DEBUG
		//std::cout << "마우스 변화량 : {" << moveX << ", " << moveY << "}" << std::endl;

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