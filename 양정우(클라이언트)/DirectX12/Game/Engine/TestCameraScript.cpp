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
	Vec3 dir = GetTransform()->GetLocalRotation();

	/*if (_tempPos.x == 0 && _tempPos.y == 0)
	{
		_tempPos = INPUT->GetMousePos();
	}*/
	

	//마우스 입력에 따른 회전
	//현재 구조는 매 프레임마다 마우스의 위치를 기억한다.
	//그러므로 이전 프레임의 마우스 좌표값을 저장할 temp가 필요하다.
	//여기서 temp는 0, 0으로 초기화한다.
	
	//먼저 현재 마우스의 포지션값을 읽어와서 저장한다.
	const POINT& nowMousePos = INPUT->GetMousePos();
	
	//이전 마우스 위치와 현재 마우스 위치의 차이를 비교한다.
	
	
	//마지막으로 현재 마우스의 포지션을 갱신해준다.
	//_tempPos = nowMousePos;
	

	GetTransform()->SetLocalRotation(dir);
}