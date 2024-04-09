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

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	// 아래
	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 3.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	//위
	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 3.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	//오른쪽
	if (INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DELTA_TIME * 3.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	// 왼쪽
	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= DELTA_TIME * 3.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

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

	// 현재 바라보고있는 방향.
	Vec3 dir = GetTransform()->GetLocalRotation();

	//마우스 입력에 따른 회전
	//현재 구조는 매 프레임마다 마우스의 위치를 기억한다.
	//그러므로 중앙으로부터 마우스가 얼마나 움직였는지를 알아내서
	//움직인 값만큼 회전에 곱해주면 되지않을까 싶다.

	
	//먼저 현재 마우스의 포지션값을 읽어와서 저장한다.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	// DEBUG
	std::cout << "X : " << nowMousePos.x << "|| Y : " << nowMousePos.y << std::endl;

	//만약 마우스가 움직임이 발생했다면
	//if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	//{
	//	//움직인 값을 저장한다.
	//	int x = nowMousePos.x - WINDOW_MIDDLE_X;
	//	int y = nowMousePos.y - WINDOW_MIDDLE_Y;
	//}
	//이전 마우스 위치와 현재 마우스 위치의 차이값을 계산한다.

	// 모든것을 계산하고 난 이후 바라보고있는 방향 갱신.
	GetTransform()->SetLocalRotation(dir);

	//마우스의 위치를 중앙으로 초기화 해준다.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}