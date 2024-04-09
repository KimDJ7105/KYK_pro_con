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

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	// �Ʒ�
	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 3.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	//��
	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 3.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	//������
	if (INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DELTA_TIME * 3.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	// ����
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

	// ���� �ٶ󺸰��ִ� ����.
	Vec3 dir = GetTransform()->GetLocalRotation();

	//���콺 �Է¿� ���� ȸ��
	//���� ������ �� �����Ӹ��� ���콺�� ��ġ�� ����Ѵ�.
	//�׷��Ƿ� �߾����κ��� ���콺�� �󸶳� ������������ �˾Ƴ���
	//������ ����ŭ ȸ���� �����ָ� ���������� �ʹ�.

	
	//���� ���� ���콺�� �����ǰ��� �о�ͼ� �����Ѵ�.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	// DEBUG
	std::cout << "X : " << nowMousePos.x << "|| Y : " << nowMousePos.y << std::endl;

	//���� ���콺�� �������� �߻��ߴٸ�
	//if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	//{
	//	//������ ���� �����Ѵ�.
	//	int x = nowMousePos.x - WINDOW_MIDDLE_X;
	//	int y = nowMousePos.y - WINDOW_MIDDLE_Y;
	//}
	//���� ���콺 ��ġ�� ���� ���콺 ��ġ�� ���̰��� ����Ѵ�.

	// ������ ����ϰ� �� ���� �ٶ󺸰��ִ� ���� ����.
	GetTransform()->SetLocalRotation(dir);

	//���콺�� ��ġ�� �߾����� �ʱ�ȭ ���ش�.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}