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
	//���콺 ������� ���� P�Է½� ���α׷��� �����ϵ��� �Ͽ���.
	if (INPUT->GetButton(KEY_TYPE::ESC))
		PostQuitMessage(0);

	MoveUpdate();

	if (isMouseMod)
	{
		RotationUpdate();
	}
	else if (!isMouseMod)
	{

	}

	if (INPUT->GetButtonDown(KEY_TYPE::P))
	{
		if (isMouseMod == false)
			isMouseMod = true;
		else if (isMouseMod == true)
			isMouseMod = false;
	}


	//Picking �Է��� Ȯ��
	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
	}
}

void TestCameraScript::MoveUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	Vec3 tempPos = pos;

	if (INPUT->GetButton(KEY_TYPE::W))
	{
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

		//std::cout << "W �Է� ó����" << std::endl;

		// Right���Ϳ� ���������� ������ -> ����
		//pos += XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::S))
	{
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		//std::cout << "S �Է� ó����" << std::endl;

		//pos -= XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::A))
	{
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		//std::cout << "A �Է� ó����" << std::endl;

		// Look���Ϳ� ���������� ������ -> ������
		//pos -= XMVector3Cross(GetTransform()->GetLook(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::D))
	{
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		//std::cout << "D �Է� ó����" << std::endl;

		//pos += XMVector3Cross(GetTransform()->GetLook(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	//pos.y = tempPos.y;

	// J�� ���ȴٰ� ��������
	if (INPUT->GetButtonUp(KEY_TYPE::J))
	{
		// �Ʒ� ������ ����ȴ�.
		cs_packet_box_create bc;
		bc.size = sizeof(cs_packet_box_create);
		bc.type = CS_BOX_CREATE;
		bc.x = pos.x;
		bc.y = pos.y;
		bc.z = pos.z;

		session->Send_Packet(&bc);
	}

	//��ġ�� ����Ǿ������� ������ �����ϵ��� ����
	if (pos != tempPos)
	{
		//------------------------------------
		cs_packet_pos_info packet;
		packet.size = sizeof(cs_packet_pos_info);
		packet.type = CS_POS_INFO;
		packet.x = pos.x;
		packet.y = pos.y;
		packet.z = pos.z;

		session->Send_Packet(&packet);
		//-------------------------------------
	}

	GetTransform()->SetLocalPosition(pos);
}


void TestCameraScript::RotationUpdate()
{
	//���� ���� ���콺�� �����ǰ��� �о�ͼ� �����Ѵ�.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	Vec3 rotation = GetTransform()->GetLocalRotation();
	//���� ���콺�� �������� �߻��ߴٸ�
	if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	{
		//������ ���� �����Ѵ�.
		int moveX = nowMousePos.x - WINDOW_MIDDLE_X;
		int moveY = nowMousePos.y - WINDOW_MIDDLE_Y;

		//������
		if (moveX > 0)
		{
			rotation.y += DELTA_TIME * moveX;
			GetTransform()->SetLocalRotation(rotation);
		}

		// ����
		if (moveX < 0)
		{
			rotation.y += DELTA_TIME * moveX;
			GetTransform()->SetLocalRotation(rotation);
		}

		// �Ʒ�
		if (moveY > 0)
		{
			rotation.x += DELTA_TIME * moveY;
			GetTransform()->SetLocalRotation(rotation);
		}

		//��
		if (moveY < 0)
		{
			rotation.x += DELTA_TIME * moveY;
			GetTransform()->SetLocalRotation(rotation);
		}

		//---------------------------------
		// �̰����� rotation������ server�� �Ѱ��ָ� �ȴ�.
		cs_packet_mouse_info mi;
		mi.size = sizeof(cs_packet_mouse_info);
		mi.type = CS_MOUSE_INFO;
		mi.x = rotation.x;
		mi.y = rotation.y;
		mi.z = 0.0f;

		session->Send_Packet(&mi);
		//---------------------------------
	}


	//���콺�� ��ġ�� �߾����� �ʱ�ȭ ���ش�.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}
