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

	//���콺 ������� ���� P�Է½� ���α׷��� �����ϵ��� �Ͽ���.
	if (INPUT->GetButton(KEY_TYPE::P))
		PostQuitMessage(0);

	//�̰��� ��ư�� ������������ ������������ �����ؾ� �� ��Ȯ�� �������� �����ϴ�.

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

	//���콺 �Է¿� ���� ȸ��
	//���� ������ �� �����Ӹ��� ���콺�� ��ġ�� ����Ѵ�.
	//�׷��Ƿ� �߾����κ��� ���콺�� �󸶳� ������������ �˾Ƴ���
	//������ ����ŭ ȸ���� �����ָ� ���������� �ʹ�.

	
	//���� ���� ���콺�� �����ǰ��� �о�ͼ� �����Ѵ�.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	// DEBUG
	//std::cout << "���� ���콺 ��ġ : {" << nowMousePos.x << ", " << nowMousePos.y << "}" << std::endl;

	//���� ���콺�� �������� �߻��ߴٸ�
	if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	{
		//������ ���� �����Ѵ�.
		int moveX = nowMousePos.x - WINDOW_MIDDLE_X;
		int moveY = nowMousePos.y - WINDOW_MIDDLE_Y;

		// DEBUG
		//std::cout << "���콺 ��ȭ�� : {" << moveX << ", " << moveY << "}" << std::endl;

		//������
		if (moveX > 0)
		{
			Vec3 rotation = GetTransform()->GetLocalRotation();
			rotation.y += DELTA_TIME * moveX;
			GetTransform()->SetLocalRotation(rotation);
		}

		// ����
		if (moveX < 0)
		{
			Vec3 rotation = GetTransform()->GetLocalRotation();
			rotation.y += DELTA_TIME * moveX;
			GetTransform()->SetLocalRotation(rotation);
		}

		// �Ʒ�
		if (moveY > 0)
		{
			Vec3 rotation = GetTransform()->GetLocalRotation();
			rotation.x += DELTA_TIME * moveY;
			GetTransform()->SetLocalRotation(rotation);
		}

		//��
		if (moveY < 0)
		{
			Vec3 rotation = GetTransform()->GetLocalRotation();
			rotation.x += DELTA_TIME * moveY;
			GetTransform()->SetLocalRotation(rotation);
		}
	}

	//���콺�� ��ġ�� �߾����� �ʱ�ȭ ���ش�.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}