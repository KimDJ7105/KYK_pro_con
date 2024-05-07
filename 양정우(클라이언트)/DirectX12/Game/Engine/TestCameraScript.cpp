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
		// previousTitle 배열이 비어 있는지 확인하고, 비어 있다면 windowTitle 내용을 복사합니다.
		wcscpy_s(previousTitle, windowTitle);
	}
	else if (wcscmp(windowTitle, previousTitle) != 0 && isWindowCapture == true)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, NULL, 0, NULL, NULL);
		char* buffer = new char[len];
		WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, buffer, len, NULL, NULL);

		const char* compareString = "PROJECT_ReWork";

		// strcmp 함수를 사용하여 buffer와 비교하고자 하는 문자열을 비교
		if (strstr(buffer, compareString) != nullptr)
		{
			//현재 활성화된 화면이 게임화면이라면
			::ShowCursor(false);
			isMouseMod = true;
			::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
		}
		else {
			//현재 활성화된 화면이 게임이 아니라면
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
	


	//마우스 디버깅을 위해 P입력시 프로그램이 종료하도록 하였다.
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


	//Picking 입력을 확인
	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		
		shared_ptr<GameObject> pickedObject;

		pickedObject = GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
		if (pickedObject != NULL)
		{
			int a = pickedObject->GetTransform()->GetObjectType();

			//여기서 타입이 플레이어일때만
			//즉 OT_PLAYER일때만 정보를 전달하도록 한다.
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

		//std::cout << "W 입력 처리중" << std::endl;

		// Right벡터와 수직벡터의 외적값 -> 정면
		pos += XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::S))
	{
		//pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
		//std::cout << "S 입력 처리중" << std::endl;

		pos -= XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::A))
	{
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
		//std::cout << "A 입력 처리중" << std::endl;

		// Look벡터와 수직벡터의 외적값 -> 오른쪽
		//pos -= XMVector3Cross(GetTransform()->GetLook(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::D))
	{
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;
		//std::cout << "D 입력 처리중" << std::endl;

		//pos += XMVector3Cross(GetTransform()->GetLook(), Vec3(0.f, 1.f, 0.f)) * _speed * DELTA_TIME;
	}

	pos.y = tempPos.y;

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

	//위치가 변경되었을때만 서버에 전송하도록 수정
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
	//먼저 현재 마우스의 포지션값을 읽어와서 저장한다.
	POINT nowMousePos;
	::GetCursorPos(&nowMousePos);

	Vec3 rotation = GetTransform()->GetLocalRotation();
	//만약 마우스가 움직임이 발생했다면
	if (nowMousePos.x != WINDOW_MIDDLE_X || nowMousePos.y != WINDOW_MIDDLE_Y)
	{
		//움직인 값을 저장한다.
		int moveX = nowMousePos.x - WINDOW_MIDDLE_X;
		int moveY = nowMousePos.y - WINDOW_MIDDLE_Y;

		//오른쪽
		if (moveX > 0)
		{
			rotation.y += DELTA_TIME * moveX;
			GetTransform()->SetLocalRotation(rotation);
		}

		// 왼쪽
		if (moveX < 0)
		{
			rotation.y += DELTA_TIME * moveX;
			GetTransform()->SetLocalRotation(rotation);
		}

		// 아래
		if (moveY > 0)
		{
			rotation.x += DELTA_TIME * moveY;
			GetTransform()->SetLocalRotation(rotation);
		}

		//위
		if (moveY < 0)
		{
			rotation.x += DELTA_TIME * moveY;
			GetTransform()->SetLocalRotation(rotation);
		}

		//---------------------------------
		// 이곳에서 rotation정보를 server에 넘겨주면 된다.
		cs_packet_mouse_info mi;
		mi.size = sizeof(cs_packet_mouse_info);
		mi.type = CS_MOUSE_INFO;
		mi.x = rotation.x;
		mi.y = rotation.y;
		mi.z = 0.0f;

		session->Send_Packet(&mi);
		//---------------------------------
	}


	//마우스의 위치를 중앙으로 초기화 해준다.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}
