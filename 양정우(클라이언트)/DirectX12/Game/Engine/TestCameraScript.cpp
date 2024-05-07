#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

#include "session.h"

extern int playerID;

TestCameraScript::TestCameraScript()
{
}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	HWND foregroundWindow = GetForegroundWindow();
	wchar_t windowTitle[256] = { 0 };
	GetWindowTextW(foregroundWindow, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));

	if (previousTitle[0] == L'\0')
	{
		// previousTitle �迭�� ��� �ִ��� Ȯ���ϰ�, ��� �ִٸ� windowTitle ������ �����մϴ�.
		wcscpy_s(previousTitle, windowTitle);
	}
	else if (wcscmp(windowTitle, previousTitle) != 0 && isWindowCapture == true)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, NULL, 0, NULL, NULL);
		char* buffer = new char[len];
		WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, buffer, len, NULL, NULL);

		const char* compareString = "PROJECT_ReWork";

		// strcmp �Լ��� ����Ͽ� buffer�� ���ϰ��� �ϴ� ���ڿ��� ��
		if (strstr(buffer, compareString) != nullptr)
		{
			//���� Ȱ��ȭ�� ȭ���� ����ȭ���̶��
			::ShowCursor(false);
			isMouseMod = true;
			::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
		}
		else {
			//���� Ȱ��ȭ�� ȭ���� ������ �ƴ϶��
			::ShowCursor(true);
			isMouseMod = false;
		}

		delete[] buffer;
	}
	else
	{

	}


	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
	{
		if (isMouseMod == false)
		{
			::ShowCursor(false);
			isMouseMod = true;
			isWindowCapture = true;
			::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
		}
		else if (isMouseMod == true)
		{
			::ShowCursor(true);
			isMouseMod = false;
			isWindowCapture = false;
		}
	}
	


	//���콺 ������� ���� P�Է½� ���α׷��� �����ϵ��� �Ͽ���.
	if (INPUT->GetButton(KEY_TYPE::C))
		PostQuitMessage(0);

	MoveUpdate();

	if (isMouseMod)
	{
		RotationUpdate();
	}
	else if (!isMouseMod)
	{
		
	}


	//Picking �Է��� Ȯ��
	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		
		shared_ptr<GameObject> pickedObject;

		pickedObject = GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
		if (pickedObject != NULL)
		{
			int a = pickedObject->GetTransform()->GetObjectType();

			//���⼭ Ÿ���� �÷��̾��϶���
			//�� OT_PLAYER�϶��� ������ �����ϵ��� �Ѵ�.
			if (pickedObject->GetTransform()->GetObjectType() == OT_PLAYER)
			{
				cs_packet_picking_info ppi;
				ppi.size = sizeof(cs_packet_picking_info);
				ppi.type = CS_PICKING_INFO;
				ppi.shooter_id = playerID;
				ppi.target_id = pickedObject->GetTransform()->GetObjectID();

				session->Send_Packet(&ppi);
			}
		}
		else
		{
			//Empty!
		}

		
		

		/*scene->RemoveGameObject(pickedObject); */
	}

	wcscpy_s(previousTitle, windowTitle);
}

void TestCameraScript::MoveUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	Vec3 tempPos = pos;

	if (INPUT->GetButton(KEY_TYPE::W))
	{
		//pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

		//std::cout << "W �Է� ó����" << std::endl;

		// Right���Ϳ� ���������� ������ -> ����
		pos += XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::S))
	{
		//pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		//std::cout << "S �Է� ó����" << std::endl;

		pos -= XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
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

	pos.y = tempPos.y;

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
