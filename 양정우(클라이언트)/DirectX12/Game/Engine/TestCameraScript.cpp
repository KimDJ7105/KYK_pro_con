#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "BoxCollider.h"
#include "SoundPlayer.h"
#include "Animator.h"

#include "session.h"

#include "Quaternion.h"


extern int playerID;

TestCameraScript::TestCameraScript()
{
	// �÷��̾��� ���� �ӵ� �ʱ�ȭ
	verticalVelocity = 0.0f;

}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{

	if (playerObject == NULL)
	{
		playerObject = GET_SINGLE(SceneManager)->GetPlayer(playerID);
	}

	if (playerGunObject == NULL)
	{
		playerGunObject = GET_SINGLE(SceneManager)->GetPlayerGun(playerID);
	}
	if (playerHeadCoreObject == NULL)
	{
		playerHeadCoreObject = GET_SINGLE(SceneManager)->GetPlayerHeadCoreQbject(playerID);
	}


	if (cursor == nullptr)
	{
		auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetTransform()->GetObjectType() == OT_CURSOR)
			{
				cursor = gameObject;
			}
		}
	}

	if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
	{
		moveSpeed = 200.f;
	}
	else if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
	{
		moveSpeed = 100.f;
	}



	{
		// ���� ��ġ ����
		previousPosition = GetTransform()->GetLocalPosition();

		// �� �����Ӹ��� �߷¿� ���� ���� �ӵ��� ���ҽ�Ŵ

		// ���� ���� ���¸� �Ǻ��Ͽ� isGrounded ������ ����
		bool isGrounded = (GetTransform()->GetLocalPosition().y <= 0.0f);

		// ���� ���� ���¿����� �߷¿� ���� �ӵ� ���Ҹ� ����
		if (isGrounded)
		{
			verticalVelocity -= GRAVITY * DELTA_TIME;
		}

		// �÷��̾��� ���� ��ġ�� ������
		Vec3 currentPosition = GetTransform()->GetLocalPosition();

		Vec3 tempPos = currentPosition;

		// �÷��̾��� �̵� �ӵ� �� ���� ���� (���÷� WASD Ű�� �̿��� �̵�)
		Vec3 moveDirection = Vec3(0.0f, 0.0f, 0.0f);

		if (INPUT->GetButtonDown(KEY_TYPE::W))
		{
			wKeyState = true;

			if (!isMoving) {
				isMoving = true;

				cs_packet_move_key_down mkd;
				mkd.size = sizeof(cs_packet_move_key_down);
				mkd.type = CS_MOVE_KEY_DOWN;

				main_session->Send_Packet(&mkd);
			}
		}
		else if (INPUT->GetButtonUp(KEY_TYPE::W))
		{
			wKeyState = false;

			if (isMoving && !wKeyState && !aKeyState && !sKeyState && !dKeyState) {
				isMoving = false;

				cs_packet_move_key_up mku;
				mku.size = sizeof(cs_packet_move_key_up);
				mku.type = CS_MOVE_KEY_UP;

				main_session->Send_Packet(&mku);
			}
		}
		if (INPUT->GetButtonDown(KEY_TYPE::S))
		{
			sKeyState = true;
			if (!isMoving) {
				isMoving = true;

				cs_packet_move_key_down mkd;
				mkd.size = sizeof(cs_packet_move_key_down);
				mkd.type = CS_MOVE_KEY_DOWN;

				main_session->Send_Packet(&mkd);
			}
		}
		else if (INPUT->GetButtonUp(KEY_TYPE::S))
		{
			sKeyState = false;

			if (isMoving && !wKeyState && !aKeyState && !sKeyState && !dKeyState) {
				isMoving = false;

				cs_packet_move_key_up mku;
				mku.size = sizeof(cs_packet_move_key_up);
				mku.type = CS_MOVE_KEY_UP;

				main_session->Send_Packet(&mku);
			}

		}
		if (INPUT->GetButtonDown(KEY_TYPE::A))
		{
			aKeyState = true;
			if (!isMoving) {
				isMoving = true;

				cs_packet_move_key_down mkd;
				mkd.size = sizeof(cs_packet_move_key_down);
				mkd.type = CS_MOVE_KEY_DOWN;

				main_session->Send_Packet(&mkd);
			}
		}
		else if (INPUT->GetButtonUp(KEY_TYPE::A))
		{
			aKeyState = false;
			if (isMoving && !wKeyState && !aKeyState && !sKeyState && !dKeyState) {
				isMoving = false;

				cs_packet_move_key_up mku;
				mku.size = sizeof(cs_packet_move_key_up);
				mku.type = CS_MOVE_KEY_UP;

				main_session->Send_Packet(&mku);
			}
		}

		if (INPUT->GetButtonDown(KEY_TYPE::D))
		{
			dKeyState = true;
			if (!isMoving) {
				isMoving = true;

				cs_packet_move_key_down mkd;
				mkd.size = sizeof(cs_packet_move_key_down);
				mkd.type = CS_MOVE_KEY_DOWN;

				main_session->Send_Packet(&mkd);
			}
		}
		else if (INPUT->GetButtonUp(KEY_TYPE::D))
		{
			dKeyState = false;

			if (isMoving && !wKeyState && !aKeyState && !sKeyState && !dKeyState) {
				isMoving = false;

				cs_packet_move_key_up mku;
				mku.size = sizeof(cs_packet_move_key_up);
				mku.type = CS_MOVE_KEY_UP;

				main_session->Send_Packet(&mku);
			}
		}

		if (main_session->get_isMapOpen() == false)
		{
			if (wKeyState)
			{
				moveDirection += XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f));
			}
			if (sKeyState)
			{
				moveDirection -= XMVector3Cross(GetTransform()->GetRight(), Vec3(0.f, 1.f, 0.f));
			}
			if (aKeyState)
			{
				moveDirection -= GetTransform()->GetRight();
			}
			if (dKeyState)
			{
				moveDirection += GetTransform()->GetRight();
			}
		}

		// �̵� ���� ������ ���̸� 1�� ����ȭ�Ͽ� �̵� �ӵ��� �����ϰ� ��
		if (moveDirection.LengthSquared() > 0.0f)
		{
			moveDirection.Normalize();
		}
		// �÷��̾��� ��ġ�� �̵� ����� �ӵ��� ���� ������Ʈ
		currentPosition += moveDirection * moveSpeed * DELTA_TIME;

		

		if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
		{
			if (playerObject != NULL)
			{
				shared_ptr<GameObject> overlap = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, 99);
				if (overlap != NULL)
				{
					isOverlap = true;
					currentPosition = previousPosition; // �浹 �� ���� ��ġ�� �ǵ���
				}
				else
				{
					isOverlap = false;
				}
				shared_ptr<GameObject> overlap_blade = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_CRUSHER_BLADE);
				if (overlap_blade != NULL)
				{
					//isCrushed = true;
					cs_packet_hit_by_grinder hbg;
					hbg.type = CS_HIT_BY_GRINDER;
					hbg.size = sizeof(cs_packet_hit_by_grinder);

					main_session->Send_Packet(&hbg);
				}
				else
				{
					//isCrushed = false;

				}

				shared_ptr<GameObject> overlap_Laser = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_LASER);
				if (overlap_Laser != NULL)
				{
					//������Į
					cs_packet_hit_by_laser hbl;
					hbl.size = sizeof(cs_packet_hit_by_laser);
					hbl.type = CS_HIT_BY_LASER;

					main_session->Send_Packet(&hbl);
				}
				else
				{
				}

			}
		}
		else if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
		{
			if (playerHeadCoreObject != NULL)
			{
				shared_ptr<GameObject> overlap = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerHeadCoreObject, 99);
				if (overlap != NULL)
				{
					isOverlap = true;
					currentPosition = previousPosition; // �浹 �� ���� ��ġ�� �ǵ���
				}
				else
				{
					isOverlap = false;
				}
				shared_ptr<GameObject> overlap_blade = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerHeadCoreObject, OT_CRUSHER_BLADE);
				if (overlap_blade != NULL)
				{
					// �γ��ھ� �����϶� �м�⿡ �������
				}
				else
				{
					
				}

				shared_ptr<GameObject> overlap_Laser = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerHeadCoreObject, OT_LASER);
				if (overlap_Laser != NULL)
				{
					// �γ��ھ� �����ϋ� �������� �������
				}
				else
				{
				}

			}
		}



		// �÷��̾ �� ������ �������� ���� �����ϱ� ���� y ��ǥ�� 0���� ����
		if (currentPosition.y < 0.0f)
		{
			currentPosition.y = 0.0f;
			// ���� ���� ���·� �����Ͽ� ���� �ӵ��� 0���� �ʱ�ȭ
			verticalVelocity = 0.0f;
		}


		//��ġ�� ����Ǿ������� ������ �����ϵ��� ����
		if (currentPosition != tempPos)
		{

			float addedPos_Y;
			if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
			{
				addedPos_Y = -40.f;
			}
			else if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
			{
				addedPos_Y = -35.f;
			}
			//------------------------------------
			cs_packet_pos_info packet;
			packet.size = sizeof(cs_packet_pos_info);
			packet.type = CS_POS_INFO;
			packet.x = currentPosition.x;
			packet.y = currentPosition.y + addedPos_Y;
			packet.z = currentPosition.z;

			main_session->Send_Packet(&packet);
			//-------------------------------------
		}
		else if (currentPosition == tempPos)
		{
		}


		// ������Ʈ�� ��ġ�� �÷��̾ �ݿ�
		GetTransform()->SetLocalPosition(currentPosition);

	}

	if (INPUT->GetButton(KEY_TYPE::P))
	{
		GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 0.f));
	}


	if (isMouseMod)
	{
		RotationUpdate();
		cursor->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	}
	else if (!isMouseMod)
	{
		POINT nowMousePos;
		::GetCursorPos(&nowMousePos);
		ScreenToClient(GetActiveWindow(), &nowMousePos);

		int screenWidth = WINDOW_WIDTH;  // Example screen width
		int screenHeight = WINDOW_HEIGHT; // Example screen height

		// Assuming the screen origin (0,0) is at the top-left corner
		Vec2 uiPos;
		uiPos.x = nowMousePos.x - (screenWidth / 2.0f);
		uiPos.y = (screenHeight / 2.0f) - nowMousePos.y;

		if (cursor != nullptr)
		{
			cursor->GetTransform()->SetLocalPosition(Vec3(uiPos.x, uiPos.y * 1.1 - 60.f, 500.f));
		}
	}



#ifdef DEBUG_ON
	//���콺 ������� ���� P�Է½� ���α׷��� �����ϵ��� �Ͽ���.
	if (INPUT->GetButton(KEY_TYPE::C))
		PostQuitMessage(0);
#endif

	HWND foregroundWindow = GetForegroundWindow();
	wchar_t windowTitle[256] = { 0 };
	GetWindowTextW(foregroundWindow, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));

	if (previousTitle[0] == L'\0')
	{
		// previousTitle �迭�� ��� �ִ��� Ȯ���ϰ�, ��� �ִٸ� windowTitle ������ �����մϴ�.
		wcscpy_s(previousTitle, windowTitle);
	}


	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
	{
		if (isMouseMod == false)
		{
			isMouseMod = true;
			std::cout << "���콺 ��� true 348" << std::endl;


			isWindowCapture = true;
			::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
			//std::cout << "�������� ���� üũ��尡 �ߵ���" << std::endl;
		}
		else if (isMouseMod == true)
		{
			isMouseMod = false;
			std::cout << "���콺 ��� false 358" << std::endl;
			isWindowCapture = false;
			//std::cout << "�������� ���� üũ��尡 ������" << std::endl;
		}
	}
	
	if (isWindowCapture == true)
	{
		//â�� ��ȭ�� �Ͼ�ٸ�.
		if (wcscmp(windowTitle, previousTitle) != 0)
		{

			//std::cout << "â�� ����Ǿ���" << std::endl;

			int len = WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, NULL, 0, NULL, NULL);
			char* buffer = new char[len];
			WideCharToMultiByte(CP_ACP, 0, windowTitle, -1, buffer, len, NULL, NULL);

			const char* compareString = "PROJECT_ReWork";

			// strcmp �Լ��� ����Ͽ� buffer�� ���ϰ��� �ϴ� ���ڿ��� ��
			if (strstr(buffer, compareString) != nullptr)
			{

				//std::cout << "���� ȭ���� ����ȭ���Դϴ�" << std::endl;

				//���� Ȱ��ȭ�� ȭ���� ����ȭ���̶��
				isMouseMod = true;
				std::cout << "���콺 ��� true 386" << std::endl;
				::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
			}
			else {

				//std::cout << "���� ȭ���� ����ȭ���� �ƴմϴ�." << std::endl;

				//���� Ȱ��ȭ�� ȭ���� ������ �ƴ϶��
				isMouseMod = false;
				std::cout << "���콺 ��� false 395" << std::endl;
			}

			delete[] buffer;
		}
		else
		{
			//std::cout << "â�� ������� ����" << std::endl;
		}
	}
	else if(isWindowCapture == false)
	{
		
	}

	if (main_session->get_isMapOpen() == false)
	{
		if (isMouseMod == false)
		{
			isMouseMod = true;
			std::cout << "���콺 ��� true 415" << std::endl;
		}
		

		//Picking �Է��� Ȯ��
		if (INPUT->GetButton(KEY_TYPE::LBUTTON))
		{
#ifdef DEBUG_ON
			std::cout << "COOLTIME" << std::endl;
#endif
			if (clickCooldown <= timeElapse)
			{
#ifdef DEBUG_ON
				std::cout << "FIRE" << std::endl;
#endif
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
						ppi.target_id = pickedObject->GetTransform()->GetObjectID();

						main_session->Send_Packet(&ppi);
					}

					else {
						cs_packet_picking_info ppi;
						ppi.size = sizeof(cs_packet_picking_info);
						ppi.type = CS_PICKING_INFO;
						ppi.target_id = -1;

						main_session->Send_Packet(&ppi);
					}
				}

				else {
					cs_packet_picking_info ppi;
					ppi.size = sizeof(cs_packet_picking_info);
					ppi.type = CS_PICKING_INFO;
					ppi.target_id = -1;

					main_session->Send_Packet(&ppi);
				}

				timeElapse = 0.f;
			}
		}
	}
	else if (main_session->get_isMapOpen() == true)
	{
		isMouseMod = false;
		//std::cout << "���콺 ��� false 477" << std::endl;

		if (INPUT->GetButtonUp(KEY_TYPE::LBUTTON))
		{

			std::cout << "Pressed Button Type : " << GET_SINGLE(SceneManager)->GetButtonType() << std::endl;
			std::cout << "Pressed Button ID : " << GET_SINGLE(SceneManager)->GetButtonID() << std::endl;

			Vec3 laser_pos = GET_SINGLE(SceneManager)->GetLaserPosition(GET_SINGLE(SceneManager)->GetButtonID());


			/*float laser_start = laser_pos.x - 150.f;
			float laser_end = laser_pos.x + 150.f;*/

			Vec3 laser_dir = Vec3(-1.57f, 1.57f, 0.f);

			/*std::cout << "Trap pos X : " << laser_pos.x << std::endl;
			std::cout << "Trap pos Y : " << laser_pos.y << std::endl;
			std::cout << "Trap pos Z : " << laser_pos.z << std::endl;

			std::cout << "Trap Start pos X : " << laser_start << std::endl;
			std::cout << "Trap End pos X : " << laser_end << std::endl;*/

			// ���ȣĮ
			cs_packet_trigger_laser tl;
			tl.size = sizeof(cs_packet_trigger_laser);
			tl.type = CS_TRIGGER_LASER;
			tl.room_num = GET_SINGLE(SceneManager)->GetButtonID();
			tl.x = laser_pos.x;
			tl.y = laser_pos.y;
			tl.z = laser_pos.z;

			main_session->Send_Packet(&tl);

		}
	}

	

	if (pickedMovingObject != NULL)
		RotatingPickedObject();


	if (INPUT->GetButtonDown(KEY_TYPE::E))
	{
		if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
		{
			//�÷��̾� ID�� Ž��
			shared_ptr<GameObject> playerObject = GET_SINGLE(SceneManager)->GetPlayer(playerID);

			//playerObject�� OT_KEYCARDŸ�԰��� �浹�� �Ͼ������ Ȯ��
			shared_ptr<GameObject> keyCard = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_KEYCARD);

			if (keyCard != NULL)
			{
				Vec3 pos = playerObject->GetTransform()->GetLocalPosition();

				cs_packet_try_get_key tgk;
				tgk.size = sizeof(cs_packet_try_get_key);
				tgk.type = CS_TRY_GET_KEY;
				tgk.x = pos.x;
				tgk.y = pos.y;
				tgk.z = pos.z;
				tgk.key_id = keyCard->GetTransform()->GetObjectID();
				//pos xyz �� �÷��̾��� ���� ��ġ
				//key_id�� �浹�� ī��Ű�� id��
				main_session->Send_Packet(&tgk);

				GET_SINGLE(SceneManager)->SetKeyCardPosition(150 + 75 * haveKeycard, (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)) - 130, haveKeycard);
				haveKeycard++;
			}

			shared_ptr<GameObject> terminal = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_TERMINAL);

			if (terminal != NULL)
			{
				cs_packet_try_use_tmn tut;
				tut.size = sizeof(cs_packet_try_use_tmn);
				tut.type = CS_TRY_USE_TMN;
				tut.terminal_id = terminal->GetTransform()->GetObjectID();

				main_session->Send_Packet(&tut);
			}

			shared_ptr<GameObject> rabbitfoot = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_RABBITFOOT);

			if (rabbitfoot != NULL)
			{
				Vec3 pos = playerObject->GetTransform()->GetLocalPosition();

				cs_packet_try_get_rabbitfoot tgr;
				tgr.size = sizeof(cs_packet_try_get_rabbitfoot);
				tgr.type = CS_TRY_GET_RABBITFOOT;
				tgr.x = pos.x;
				tgr.y = pos.y;
				tgr.z = pos.z;
				tgr.obj_id = rabbitfoot->GetTransform()->GetObjectID();

				main_session->Send_Packet(&tgr);

				GET_SINGLE(SceneManager)->SetRabbitFootUI();
			}

			shared_ptr<GameObject> exit = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_EXIT);

			if (exit != NULL)
			{
				Vec3 pos = playerObject->GetTransform()->GetLocalPosition();

				cs_packet_try_escape te;
				te.size = sizeof(cs_packet_try_escape);
				te.type = CS_TRY_ESCAPE;

				main_session->Send_Packet(&te);
			}

			shared_ptr<GameObject> mediKit = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_MEDIKIT);

			if (mediKit != NULL)
			{
				//�޵�ŶĮ
			}

			shared_ptr<GameObject> ammobox = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_AMMOBOX);

			if (ammobox != NULL)
			{
				//�Ƹ�ڽ�Į
			}
		}
		else if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
		{
			//�γ��ھ �� �÷��̾��� ID�� Ž��
			shared_ptr<GameObject> playerObject = GET_SINGLE(SceneManager)->GetPlayerHeadCoreQbject(playerID);


			shared_ptr<GameObject> pad = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_RESURRECTION_PAD);

			if (pad != NULL)
			{
				Vec3 pos = playerObject->GetTransform()->GetLocalPosition();

				cs_packet_use_resurrection ur;
				ur.size = sizeof(cs_packet_use_resurrection);
				ur.type = CS_USE_RESURRECTION;

				main_session->Send_Packet(&ur);
			}
		}

	}
	
	if (INPUT->GetButtonDown(KEY_TYPE::R))
	{
		/*auto& obj = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
		for (auto& a : obj)
		{
			if (a->GetTransform()->GetObjectType() != OT_PLAYER)
				continue;
			a->GetAnimator()->Play(1);
		}*/

		cs_packet_reload_mag lm;
		lm.size = sizeof(cs_packet_reload_mag);
		lm.type = CS_RELOAD_MAG;

		main_session->Send_Packet(&lm);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::T))
	{
		/*std::cout << "test go on\n";
		test_packet tp;
		tp.size = sizeof(test_packet);
		tp.type = TEST_SPAWN_RBF;

		main_session->Send_Packet(&tp);*/

		main_session->close_socket();

		//���� ���� �ҷ�����
		GET_SINGLE(SceneManager)->LoadEndingGameScene(L"EndingScene");

		//���ΰ��� ���� ������Ʈ���� �����Ѵ�
		GET_SINGLE(SceneManager)->RemoveSceneObject(GET_SINGLE(SceneManager)->GetMainScene());
	}
	
	if (INPUT->GetButtonDown(KEY_TYPE::M))
	{
		GET_SINGLE(SceneManager)->RemoveMapUI();
	}




	if (playerObject != nullptr && playerGunObject != nullptr && playerHeadCoreObject != nullptr)
	{
		if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
		{
			playerHeadCoreObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			{

				Vec3 rotation = GetTransform()->GetLocalRotation();

				Vec3 gunOffset(5.0f, -5.0f, 15.0f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

				// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
				Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

				// gunOffset�� ȸ����Ŵ
				Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

				// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
				if (playerGunObject != NULL) playerGunObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

				// ���� ȸ�� �������� �����Ͽ� ���ʹϾ� ����
				Vec3 gunRotationOffset(-1.57f, 3.14f, 0.0f); // ���� ȸ�� ������
				Quaternion gunRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), gunRotationOffset.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), gunRotationOffset.x);

				// �÷��̾��� ȸ������ ���� ȸ���� �߰��Ͽ� ���� ���� ȸ�� ���ʹϾ� ����
				Quaternion finalGunRotationQuat = playerRotationQuat * gunRotationQuat;

				Vec3 gunRotation = finalGunRotationQuat.ToEulerAngles();

				// ȸ���� ����
				if (playerGunObject != NULL) playerGunObject->GetTransform()->SetLocalRotation(gunRotation);

				// �÷��̾ �������� �� �ݴ� �������� �̵�
				Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
				if (playerGunObject != NULL) playerGunObject->GetTransform()->SetLocalPosition(newPosition);
			}
			{
				Vec3 rotation = GetTransform()->GetLocalRotation();

				Vec3 gunOffset(0.0f, -40.0f, 10.0f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

				// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
				Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

				// gunOffset�� ȸ����Ŵ
				Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

				// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
				if (playerGunObject != NULL) playerObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

				// ���� ȸ�� �������� �����Ͽ� ���ʹϾ� ����
				Vec3 gunRotationOffset(0.f, 3.14f, 0.0f); // ���� ȸ�� ������
				Quaternion gunRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), gunRotationOffset.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), gunRotationOffset.x);

				// �÷��̾��� ȸ������ ���� ȸ���� �߰��Ͽ� ���� ���� ȸ�� ���ʹϾ� ����
				Quaternion finalGunRotationQuat = playerRotationQuat * gunRotationQuat;

				Vec3 gunRotation = finalGunRotationQuat.ToEulerAngles();

				// ȸ���� ����
				if (playerObject != NULL) playerObject->GetTransform()->SetLocalRotation(gunRotation);

				// �÷��̾ �������� �� �ݴ� �������� �̵�
				Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
				if (playerObject != NULL) playerObject->GetTransform()->SetLocalPosition(newPosition);

				if (playerObject != NULL) Vec3 hispos = playerObject->GetTransform()->GetLocalPosition();
			}
		}
		else if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
		{
			playerGunObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			playerObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));

			{
				playerHeadCoreObject->GetTransform()->SetLocalPosition(Vec3(
					GetTransform()->GetLocalPosition().x,
					GetTransform()->GetLocalPosition().y - 35.f,
					GetTransform()->GetLocalPosition().z)
				);
				Vec3 rot = playerHeadCoreObject->GetTransform()->GetLocalRotation();
				rot.y = GetTransform()->GetLocalRotation().y;
				playerHeadCoreObject->GetTransform()->SetLocalRotation(rot);
			}
		}
	}

	

	
	

	wcscpy_s(previousTitle, windowTitle);

	timeElapse += DELTA_TIME;


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

		main_session->Send_Packet(&packet);
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

			//playerRotation.x -= DELTA_TIME * moveY;

			GetTransform()->SetLocalRotation(rotation);
		}

		//��
		if (moveY < 0)
		{
			rotation.x += DELTA_TIME * moveY;

			//playerRotation.x -= DELTA_TIME * moveY;

			GetTransform()->SetLocalRotation(rotation);
		}


		if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
		{
			//---------------------------------
			// �̰����� rotation������ server�� �Ѱ��ָ� �ȴ�.
			cs_packet_mouse_info mi;
			mi.size = sizeof(cs_packet_mouse_info);
			mi.type = CS_MOUSE_INFO;
			//mi.x = rotation.x;
			mi.x = 0.f;
			mi.y = rotation.y + 3.14f;
			mi.z = 0.0f;

			main_session->Send_Packet(&mi);
			//---------------------------------
		}
		else if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
		{
			//---------------------------------
			// �̰����� rotation������ server�� �Ѱ��ָ� �ȴ�.
			cs_packet_mouse_info mi;
			mi.size = sizeof(cs_packet_mouse_info);
			mi.type = CS_MOUSE_INFO;
			//mi.x = rotation.x;
			mi.x = -1.57f;
			mi.y = rotation.y;
			mi.z = 0.0f;

			main_session->Send_Packet(&mi);
			//---------------------------------
		}
		
		
	}


	//���콺�� ��ġ�� �߾����� �ʱ�ȭ ���ش�.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}

void TestCameraScript::RotatingPickedObject()
{
	Vec3 pos = pickedMovingObject->GetTransform()->GetLocalPosition();
	Vec3 rotation = pickedMovingObject->GetTransform()->GetLocalRotation();
	Vec3 scale = pickedMovingObject->GetTransform()->GetLocalScale();

	
	if (INPUT->GetButton(KEY_TYPE::UP))
	{
		pos.z += _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::DOWN))
	{
		pos.z -= _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::LEFT))
	{
		pos.x -= _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::RIGHT))
	{
		pos.x += _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numUP))
	{
		rotation.x += _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numDOWN))
	{
		rotation.x -= _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numLEFT))
	{
		rotation.y += _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numRIGHT))
	{
		rotation.y -= _objspeed * DELTA_TIME;
	}

	if (INPUT->GetButton(KEY_TYPE::numONE))
	{
		scale.x += _objspeed * DELTA_TIME;
		scale.y += _objspeed * DELTA_TIME;
		scale.z += _objspeed * DELTA_TIME;
	}
	if (INPUT->GetButton(KEY_TYPE::numTHREE))
	{
		scale.x -= _objspeed * DELTA_TIME;
		scale.y -= _objspeed * DELTA_TIME;
		scale.z -= _objspeed * DELTA_TIME;
	}


	pickedMovingObject->GetTransform()->SetLocalPosition(pos);
	pickedMovingObject->GetTransform()->SetLocalRotation(rotation);
	pickedMovingObject->GetTransform()->SetLocalScale(scale);

#ifdef DEBUG_ON
	std::cout << "Object Pos : (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
	std::cout << "Object Rotation : (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")" << std::endl;
	std::cout << "Object Scale : (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
#endif
}
