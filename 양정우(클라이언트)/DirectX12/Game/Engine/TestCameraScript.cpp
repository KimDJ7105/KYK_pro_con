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
#include "Animator.h"

#include "session.h"

#include "Quaternion.h"

#include "SoundManager.h"


extern int playerID;



TestCameraScript::TestCameraScript()
{
	// �÷��̾��� ���� �ӵ� �ʱ�ȭ
	verticalVelocity = 0.0f;

	next_send_time = std::chrono::steady_clock::now();
	next_send_time_for_eye = std::chrono::steady_clock::now();
}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	
	GET_SINGLE(SoundManager)->UpdateListener(GetTransform()->GetLocalPosition(), Vec3(0.f, 0.f, 0.f), GetTransform()->GetLook(), GetTransform()->GetUp());

	SetObjects();

	if (GET_SINGLE(SceneManager)->GetPlayerRevive() == true)
	{
		nowGunObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		nowGunObject = playerSubGunObject;
		nowGun = 0;
		GET_SINGLE(SceneManager)->SetPlayerRevive(false);
		haveKeycard = 0;
	}

	if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
	{
		moveSpeed = 200.f;
	}
	else if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
	{
		moveSpeed = 100.f;
	}


	if (INPUT->GetButtonDown(KEY_TYPE::SHIFT))
	{
		isDash = true;


		cs_packet_run_key_down rkd;
		rkd.size = sizeof(cs_packet_run_key_down);
		rkd.type = CS_RUN_KEY_DOWN;

		main_session->Send_Packet(&rkd);
	}
	else if (INPUT->GetButtonUp(KEY_TYPE::SHIFT))
	{
		cs_packet_run_key_up rku;
		rku.size = sizeof(cs_packet_move_key_up);
		rku.type = CS_RUN_KEY_UP;
		main_session->Send_Packet(&rku);

		isDash = false;
	}



	// �̵�Į
#pragma region Moving

	if(GET_SINGLE(SceneManager)->Get_isGameStart() == true)
	{
		// ���� ��ġ ����
		previousPosition = GetTransform()->GetLocalPosition();

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
		

		if (isDash == false)
		{
			// �÷��̾��� ��ġ�� �̵� ����� �ӵ��� ���� ������Ʈ
			currentPosition += moveDirection * moveSpeed * DELTA_TIME;
			
			
		}
		else if (isDash == true)
		{
			// �÷��̾��� ��ġ�� �̵� ����� �ӵ��� ���� ������Ʈ
			currentPosition += moveDirection * moveSpeed * 1.3 * DELTA_TIME;
		}

		

		if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
		{
			if (playerObject != NULL)
			{
				shared_ptr<GameObject> overlap = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_WALLAABB);
				if (overlap != NULL)
				{
					//���浹Į
					if (is_moveable(moveDirection, overlap))
					{
						isOverlap = true;
						currentPosition = previousPosition;
					}
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
				shared_ptr<GameObject> overlap = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerHeadCoreObject, OT_WALLAABB);
				if (overlap != NULL)
				{
					//���浹Į
					if (is_moveable(moveDirection, overlap))
					{
						isOverlap = true;
						currentPosition = previousPosition;
					}
				}
				else
				{
					isOverlap = false;
				}
				shared_ptr<GameObject> overlap_blade = GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerHeadCoreObject, OT_CRUSHER_BLADE);
				if (overlap_blade != NULL)
				{
					// �γ��ھ� �����϶� �м�⿡ �������
					cs_packet_hit_by_grinder hbg;
					hbg.type = CS_HIT_BY_GRINDER;
					hbg.size = sizeof(cs_packet_hit_by_grinder);

					main_session->Send_Packet(&hbg);
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

		//����Į2

		//��ġ�� ����Ǿ������� ������ �����ϵ��� ����
		if (currentPosition != tempPos)
		{
			auto now = std::chrono::steady_clock::now();

			if (now >= next_send_time) {

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

				next_send_time = now + interval;
			}

			if (isMoving)
			{
				if (isDash)
				{
					if (GET_SINGLE(SoundManager)->IsSoundPlaying(PLAYER_RUN) == false)
					{
						if (GET_SINGLE(SoundManager)->IsSoundPlaying(PLAYER_WALK) == true)
						{
							GET_SINGLE(SoundManager)->soundStop(PLAYER_WALK);
						}
						GET_SINGLE(SoundManager)->soundPlay(PLAYER_RUN, GetTransform()->GetLocalPosition(), true);
					}
				}
				else if(!isDash)
				{
					if (GET_SINGLE(SoundManager)->IsSoundPlaying(PLAYER_WALK) == false)
					{
						if (GET_SINGLE(SoundManager)->IsSoundPlaying(PLAYER_RUN) == true)
						{
							GET_SINGLE(SoundManager)->soundStop(PLAYER_RUN);
						}
						GET_SINGLE(SoundManager)->soundPlay(PLAYER_WALK, GetTransform()->GetLocalPosition(), true);
					}
				}
			}

		}
		else if (currentPosition == tempPos)
		{
			GET_SINGLE(SoundManager)->soundStop(PLAYER_WALK);
			GET_SINGLE(SoundManager)->soundStop(PLAYER_RUN);
		}


		// ������Ʈ�� ��ġ�� �÷��̾ �ݿ�
		GetTransform()->SetLocalPosition(currentPosition);

	}

#pragma endregion

	if (INPUT->GetButtonDown(KEY_TYPE::KEY_1))
	{
		weaponChanging = true;
		weaponTimeElapse = 0.0;
		if (nowGunObject != playerMainGunObject)
		{
			nowGunObject->GetAnimator()->ClearSequence();
			playerObject->GetAnimator()->ClearSequence();

			if (nowGunObject == playerSubGunObject)
			{
				nowGunObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				nowGunObject = playerMainGunObject;
				nowGun = 1;
				
			}
			nowGunObject->GetAnimator()->AddToSequence(3);
			nowGunObject->GetAnimator()->AddToSequence(0);
			playerObject->GetAnimator()->AddToSequence(3);
			playerObject->GetAnimator()->AddToSequence(0);
			GET_SINGLE(SoundManager)->soundPlay(PLAYER_GUN_CHANGE, GetTransform()->GetLocalPosition(), false);
		}

		cs_packet_change_gun cg;
		cg.size = sizeof(cs_packet_change_gun);
		cg.type = CS_CHANGE_GUN;
		cg.pressed_key = 1;

		main_session->Send_Packet(&cg);


		//0�̸� �������	(GT_SM				0)
		//1�̸� ��ź��		(GT_SG				1)
		//2�̸� ���ݼ���	(GT_AR				2)
		//3�̸� ���ݼ���	(GT_SR				3)

		int type = GET_SINGLE(SceneManager)->GetMainWeapon_type();

		if (type == 0)//���
		{
			clickCooldown = 0.1;
			GET_SINGLE(SceneManager)->SetMaxBullet(30);
			GET_SINGLE(SceneManager)->SetGunUI(type);
		}
		else if (type == 1)//��ź
		{
			clickCooldown = 1;
			GET_SINGLE(SceneManager)->SetMaxBullet(8);
			GET_SINGLE(SceneManager)->SetGunUI(type);
		}
		else if (type == 2)//����
		{
			clickCooldown = 1;
			GET_SINGLE(SceneManager)->SetMaxBullet(5);
			GET_SINGLE(SceneManager)->SetGunUI(type);
		}
		else //����
		{
			clickCooldown = 0.125;
			GET_SINGLE(SceneManager)->SetMaxBullet(25);
			GET_SINGLE(SceneManager)->SetGunUI(3);
		}
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_2))
	{
		weaponChanging = true;
		weaponTimeElapse = 0.0;
		if (nowGunObject != playerSubGunObject)
		{
			nowGunObject->GetAnimator()->ClearSequence();
			playerObject->GetAnimator()->ClearSequence();


			if (nowGunObject == playerMainGunObject)
			{
				nowGunObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				nowGunObject = playerSubGunObject;
				nowGun = 0;
			}

			nowGunObject->GetAnimator()->AddToSequence(3);
			nowGunObject->GetAnimator()->AddToSequence(0);
			playerObject->GetAnimator()->AddToSequence(3);
			playerObject->GetAnimator()->AddToSequence(0);
			GET_SINGLE(SoundManager)->soundPlay(PLAYER_GUN_CHANGE, GetTransform()->GetLocalPosition(), false);
		}

		cs_packet_change_gun cg;
		cg.size = sizeof(cs_packet_change_gun);
		cg.type = CS_CHANGE_GUN;
		cg.pressed_key = 2;

		main_session->Send_Packet(&cg);




		clickCooldown = 0.333;
		GET_SINGLE(SceneManager)->SetMaxBullet(15);
		GET_SINGLE(SceneManager)->SetGunUI(4);
	}






#ifdef DEBUG_ON
	//���콺 ������� ���� C�Է½� ���α׷��� �����ϵ��� �Ͽ���.
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

			std::cout << "���� ȭ���� ����ȭ���Դϴ�" << std::endl;

			//���� Ȱ��ȭ�� ȭ���� ����ȭ���̶��
			isMouseMod = true;
		}
		else {

			std::cout << "���� ȭ���� ����ȭ���� �ƴմϴ�." << std::endl;

			//���� Ȱ��ȭ�� ȭ���� ������ �ƴ϶��
			isMouseMod = false;
		}

		delete[] buffer;
	}

	if (main_session->get_isMapOpen() == false)
	{
		if (isMouseMod != false)
		{
			RotationUpdate();
			cursor->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		}
		//Picking �Է��� Ȯ��

		if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
		{
			if (INPUT->GetButton(KEY_TYPE::LBUTTON))
			{
#ifdef DEBUG_ON
				
#endif
				if (clickCooldown <= fireTimeElapse && GET_SINGLE(SceneManager)->GetBullet() != 0)
				{
#ifdef DEBUG_ON
					
#endif

					nowGunObject->GetAnimator()->ClearSequence();
					playerObject->GetAnimator()->ClearSequence();

					const POINT& pos = INPUT->GetMousePos();

					shared_ptr<GameObject> pickedObject;

					

					pickedObject = GET_SINGLE(SceneManager)->Pick(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);

					int type = GET_SINGLE(SceneManager)->GetMainWeapon_type();
					//0�̸� �������	(GT_SM				0)
					//1�̸� ��ź��		(GT_SG				1)
					//2�̸� ���ݼ���	(GT_AR				2)
					//3�̸� ���ݼ���	(GT_SR				3)

					if (nowGun == 0)
					{
						GET_SINGLE(SoundManager)->soundPlay(WEAPON_PISTOL, GetTransform()->GetLocalPosition(), false);

						gunRange = 360.f;
					}
					else if (nowGun == 1)
					{
						if (type == 0)//���
						{
							GET_SINGLE(SoundManager)->soundPlay(WEAPON_SUB_MACHINE_GUN, GetTransform()->GetLocalPosition(), false);
							gunRange = 480.f;

						}
						else if (type == 1)//��ź
						{
							GET_SINGLE(SoundManager)->soundPlay(WEAPON_SHOTGUN, GetTransform()->GetLocalPosition(), false);
							gunRange = 192.f;
						}
						else if (type == 2)//����
						{
							GET_SINGLE(SoundManager)->soundPlay(WEAPON_SNIPER, GetTransform()->GetLocalPosition(), false);
							gunRange = 1200.f;
						}
						else if (type == 3)//����
						{
							GET_SINGLE(SoundManager)->soundPlay(WEAPON_ASSAULT_RIFLE, GetTransform()->GetLocalPosition(), false);
							gunRange = 600.f;
						}
					}



					if (pickedObject != NULL)
					{
						
						//���⼭ Ÿ���� �÷��̾��϶���
						//�� OT_PLAYER�϶��� ������ �����ϵ��� �Ѵ�.
						if (pickedObject->GetTransform()->GetObjectType() == OT_PLAYER)
						{

							float range = GET_SINGLE(SceneManager)->CalculateRange(GetTransform()->GetLocalPosition(), pickedObject->GetTransform()->GetLocalPosition());
							//�̰��� ��Ÿ��� �Է�
							if (range <= gunRange)
							{
								cs_packet_picking_info ppi;
								ppi.size = sizeof(cs_packet_picking_info);
								ppi.type = CS_PICKING_INFO;
								ppi.target_id = pickedObject->GetTransform()->GetObjectID();

								main_session->Send_Packet(&ppi);
							}
							else
							{
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
					}

					else {
						cs_packet_picking_info ppi;
						ppi.size = sizeof(cs_packet_picking_info);
						ppi.type = CS_PICKING_INFO;
						ppi.target_id = -1;

						main_session->Send_Packet(&ppi);
					}

					fireTimeElapse = 0.f;
					flameTimeElapse = 0.f;

					nowGunObject->GetAnimator()->AddToSequence(1);
					nowGunObject->GetAnimator()->AddToSequence(0);

					playerObject->GetAnimator()->AddToSequence(1);
					playerObject->GetAnimator()->AddToSequence(0);
				}
			}
		}
	}
	else if (main_session->get_isMapOpen() == true)
	{
		if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
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

			if (INPUT->GetButtonUp(KEY_TYPE::LBUTTON))
			{

				std::cout << "Pressed Button Type : " << GET_SINGLE(SceneManager)->GetButtonType() << std::endl;
				std::cout << "Pressed Button ID : " << GET_SINGLE(SceneManager)->GetButtonID() << std::endl;
				Vec3 laser_pos = GET_SINGLE(SceneManager)->GetLaserPosition(GET_SINGLE(SceneManager)->GetButtonID());

				Vec3 laser_dir = Vec3(-1.57f, 1.57f, 0.f);

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
	}

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
				::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
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

				GET_SINGLE(SceneManager)->SetRabbitFootUI(-150, (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)) - 130);
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
				cs_packet_use_medikit um;
				um.type = CS_USE_MEDIKIT;
				um.size = sizeof(cs_packet_use_medikit);
				um.kit_id = mediKit->GetTransform()->GetObjectID();

				main_session->Send_Packet(&um);
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
	


	// ��ȣ�ۿ� UI
	if (GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_KEYCARD) != nullptr)
	{
		GET_SINGLE(SceneManager)->SetInteractUI(Vec3(0, -100, 500.f), OT_UI_INTERACT);
	}
	else if (GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_TERMINAL) != nullptr)
	{
		GET_SINGLE(SceneManager)->SetInteractUI(Vec3(0, -100, 500.f), OT_UI_INTERACT);
	}
	else if (GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_RABBITFOOT) != nullptr)
	{
		GET_SINGLE(SceneManager)->SetInteractUI(Vec3(0, -100, 500.f), OT_UI_INTERACT);
	}
	else if (GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_MEDIKIT) != nullptr)
	{
		GET_SINGLE(SceneManager)->SetInteractUI(Vec3(0, -100, 500.f), OT_UI_INTERACT);
	}
	else
	{
		GET_SINGLE(SceneManager)->SetInteractUI(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f), OT_UI_INTERACT);
	}
	
	//��Ȱ UI
	if (GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(GET_SINGLE(SceneManager)->GetPlayerHeadCoreQbject(playerID) , OT_RESURRECTION_PAD) != nullptr)
	{
		if (GET_SINGLE(SceneManager)->GetPlayerDead() == true)
		{
			GET_SINGLE(SceneManager)->SetInteractUI(Vec3(0, -100, 500.f), OT_UI_REVIVE);
		}
	}
	else
	{
		GET_SINGLE(SceneManager)->SetInteractUI(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f), OT_UI_REVIVE);
	}

	// Ż�� UI
	if (GET_SINGLE(SceneManager)->CheckCollisionWithSceneObjects(playerObject, OT_EXIT) != nullptr)
	{
		GET_SINGLE(SceneManager)->SetInteractUI(Vec3(0, -100, 500.f), OT_UI_ESCAPE);
	}
	else
	{
		GET_SINGLE(SceneManager)->SetInteractUI(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f), OT_UI_ESCAPE);
	}











	if (INPUT->GetButtonDown(KEY_TYPE::R))
	{
		cs_packet_reload_mag lm;
		lm.size = sizeof(cs_packet_reload_mag);
		lm.type = CS_RELOAD_MAG;

		main_session->Send_Packet(&lm);

		nowGunObject->GetAnimator()->ClearSequence();
		playerObject->GetAnimator()->ClearSequence();

		// �÷��̾� �Ѱ� �÷��̾��� �ִϸ����Ϳ� �ִϸ��̼� �������� �߰�
		nowGunObject->GetAnimator()->AddToSequence(2);
		nowGunObject->GetAnimator()->AddToSequence(0);

		playerObject->GetAnimator()->AddToSequence(2);
		playerObject->GetAnimator()->AddToSequence(0);

		GET_SINGLE(SoundManager)->soundPlay(PLAYER_GUN_RELOAD, GetTransform()->GetLocalPosition(), false);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::T))
	{
		//main_session->close_socket();

		////���� ���� �ҷ�����
		//GET_SINGLE(SceneManager)->LoadBadEndingGameScene(L"EndingScene");

		////���ΰ��� ���� ������Ʈ���� �����Ѵ�
		//GET_SINGLE(SceneManager)->RemoveSceneObject(GET_SINGLE(SceneManager)->GetMainScene());

		test_packet tp;
		tp.size = sizeof(test_packet);
		tp.type = TEST_SPAWN_RBF;

		main_session->Send_Packet(&tp);
	}

	if (weaponTimeElapse > weaponChangetime && weaponChanging == true)
	{
		weaponChanging = false;
	}
		

	
	if (playerObject != nullptr && nowGunObject != nullptr && playerHeadCoreObject != nullptr)
	{
		if (GET_SINGLE(SceneManager)->GetPlayerDead() == false)
		{
			playerHeadCoreObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			{

				Vec3 rotation = GetTransform()->GetLocalRotation();

				Vec3 gunOffset(2.5f, -24 + 20, 9.5f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

				// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
				Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

				// gunOffset�� ȸ����Ŵ
				Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

				// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
				if (nowGunObject != NULL) nowGunObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

				// ���� ȸ�� �������� �����Ͽ� ���ʹϾ� ����
				Vec3 gunRotationOffset(0.f, 0.f, 0.f); // ���� ȸ�� ������

				if (weaponChanging == true)
					gunRotationOffset.x = gunRotationOffset.x - 1.57f;
				Quaternion gunRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), gunRotationOffset.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), gunRotationOffset.x);

				// �÷��̾��� ȸ������ ���� ȸ���� �߰��Ͽ� ���� ���� ȸ�� ���ʹϾ� ����
				Quaternion finalGunRotationQuat = playerRotationQuat * gunRotationQuat;

				Vec3 gunRotation = finalGunRotationQuat.ToEulerAngles();

				// ȸ���� ����
				if (nowGunObject != NULL) nowGunObject->GetTransform()->SetLocalRotation(gunRotation);

				// �÷��̾ �������� �� �ݴ� �������� �̵�
				Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
				if (nowGunObject != NULL) nowGunObject->GetTransform()->SetLocalPosition(newPosition);
			}
			{
				Vec3 rotation = GetTransform()->GetLocalRotation();

				Vec3 gunOffset(0.0f, -24.0f, 0.0f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

				// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
				Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

				// gunOffset�� ȸ����Ŵ
				Vec3 rotatedOffset = playerRotationQuat.Rotate(gunOffset);

				// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
				if (playerObject != NULL) playerObject->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

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
			nowGunObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
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

	if (flameParticle != nullptr && flameLight != nullptr)
	{
		if(flameTimeElapse < flameDuration)
		{
			{
				Vec3 rotation = GetTransform()->GetLocalRotation();

				Vec3 particle(2.5f, -24 + 22, 15.f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

				// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
				Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

				// gunOffset�� ȸ����Ŵ
				Vec3 rotatedOffset = playerRotationQuat.Rotate(particle);

				// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
				if (flameParticle != NULL) flameParticle->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

				// ���� ȸ�� �������� �����Ͽ� ���ʹϾ� ����
				Vec3 particleRotationOffset(0.f, 0.f, 0.f); // ���� ȸ�� ������

				Quaternion particleRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), particleRotationOffset.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), particleRotationOffset.x);

				// �÷��̾��� ȸ������ ���� ȸ���� �߰��Ͽ� ���� ���� ȸ�� ���ʹϾ� ����
				Quaternion finalParticleRotationQuat = playerRotationQuat * particleRotationQuat;

				Vec3 particleRotation = finalParticleRotationQuat.ToEulerAngles();

				// ȸ���� ����
				if (flameParticle != NULL) flameParticle->GetTransform()->SetLocalRotation(particleRotation);

				// �÷��̾ �������� �� �ݴ� �������� �̵�
				Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
				if (flameParticle != NULL) flameParticle->GetTransform()->SetLocalPosition(newPosition);
			}
			{
				Vec3 rotation = GetTransform()->GetLocalRotation();

				Vec3 particle(2.5f, -24 + 22, 30.f); // �Ʒ��� 2, ���������� 2, z���� ������ �����ϰ� ����

				// �÷��̾��� ȸ������ ���ʹϾ����� ��ȯ
				Quaternion playerRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), rotation.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), rotation.x);

				// gunOffset�� ȸ����Ŵ
				Vec3 rotatedOffset = playerRotationQuat.Rotate(particle);

				// playerGunObject�� ��ġ�� �÷��̾��� ��ġ�� �̵�
				if (flameLight != NULL) flameLight->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

				// ���� ȸ�� �������� �����Ͽ� ���ʹϾ� ����
				Vec3 particleRotationOffset(0.f, 0.f, 0.f); // ���� ȸ�� ������

				Quaternion particleRotationQuat = QuaternionFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), particleRotationOffset.y) *
					QuaternionFromAxisAngle(Vec3(1.0f, 0.0f, 0.0f), particleRotationOffset.x);

				// �÷��̾��� ȸ������ ���� ȸ���� �߰��Ͽ� ���� ���� ȸ�� ���ʹϾ� ����
				Quaternion finalParticleRotationQuat = playerRotationQuat * particleRotationQuat;

				Vec3 particleRotation = finalParticleRotationQuat.ToEulerAngles();

				// ȸ���� ����
				if (flameLight != NULL) flameLight->GetTransform()->SetLocalRotation(particleRotation);

				// �÷��̾ �������� �� �ݴ� �������� �̵�
				Vec3 newPosition = GetTransform()->GetLocalPosition() + rotatedOffset;
				if (flameLight != NULL) flameLight->GetTransform()->SetLocalPosition(newPosition);
			}
		}
		else
		{
			flameParticle->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			flameLight->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		}
	}


	wcscpy_s(previousTitle, windowTitle);

	fireTimeElapse += DELTA_TIME;

	flameTimeElapse += DELTA_TIME;

	if (weaponChanging == true)
	{
		weaponTimeElapse += DELTA_TIME;
	}
	
	if (GET_SINGLE(SceneManager)->Get_isGameStart() == true)
	{
		StartUI_END -= DELTA_TIME;
	}

	if (-2 < StartUI_END <= 0)
	{
		GET_SINGLE(SceneManager)->SetStartUI(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	}


	if (GET_SINGLE(SceneManager)->Get_isEscapeShow() == true)
	{
		Escape_End -= DELTA_TIME;
	}

	if (-2 < Escape_End <= 0)
	{
		GET_SINGLE(SceneManager)->SetEscapeUI(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	}

}

void TestCameraScript::SetObjects()
{
	if (playerObject == NULL)
	{
		playerObject = GET_SINGLE(SceneManager)->GetPlayer(playerID);
		/*playerObject->GetAnimator()->AddToSequence(3);
		playerObject->GetAnimator()->AddToSequence(0);*/
	}
	if (playerSubGunObject == NULL)
	{
		playerSubGunObject = GET_SINGLE(SceneManager)->GetPlayerSubGun(playerID);

		nowGunObject = playerSubGunObject;

		GET_SINGLE(SceneManager)->SetGunUI(4);

		/*nowGunObject->GetAnimator()->AddToSequence(3);
		nowGunObject->GetAnimator()->AddToSequence(0);*/
	}
	if (playerMainGunObject == NULL)
	{
		playerMainGunObject = GET_SINGLE(SceneManager)->GetPlayerMainGun(playerID);
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

	if (flameParticle == nullptr)
	{
		auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetTransform()->GetObjectType() == OT_PARTICLE_GUNSHOT)
			{
				flameParticle = gameObject;
			}
		}
	}

	if (flameLight == nullptr)
	{
		auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetTransform()->GetObjectType() == OT_LIGHT_GUNSHOT)
			{
				flameLight = gameObject;
			}
		}
	}

	if (nowGunObject != nullptr)
	{
		weaponChangetime = nowGunObject->GetAnimator()->GetAnimationEndTime(3);
	}
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

			if (rotation.x < -1.57)
			{
				rotation.x = -1.56f;
			}
			else if (1.57f < rotation.x)
			{
				rotation.x = 1.56f;
			}



			GetTransform()->SetLocalRotation(rotation);
		}

		//��
		if (moveY < 0)
		{
			rotation.x += DELTA_TIME * moveY;

			//playerRotation.x -= DELTA_TIME * moveY;

			if (rotation.x < -1.57)
			{
				rotation.x = -1.56f;
			}
			else if (1.57f < rotation.x)
			{
				rotation.x = 1.56f;
			}

			GetTransform()->SetLocalRotation(rotation);
		}

		//����Į1
		auto now = std::chrono::steady_clock::now();
		if (now >= next_send_time_for_eye) {
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

			next_send_time_for_eye = now + interval;
		}
	}

	//���콺�� ��ġ�� �߾����� �ʱ�ȭ ���ش�.
	::SetCursorPos(WINDOW_MIDDLE_X, WINDOW_MIDDLE_Y);
}

bool TestCameraScript::is_moveable(const Vec3& moveDirection, const shared_ptr<GameObject>& overlap)
{
	Vec3 rayStart3 = GetTransform()->GetLocalPosition();
	Vec3 rayEnd3 = rayStart3 + moveDirection * moveSpeed * DELTA_TIME;

	Vec4 rayStart = Vec4(rayStart3.x, rayStart3.y, rayStart3.z, 1.0f);
	Vec4 rayDirection = Vec4(rayEnd3.x - rayStart3.x, rayEnd3.y - rayStart3.y, rayEnd3.z - rayStart3.z, 0.0f);

	float distance = 0.0f;

	if (overlap->GetCollider()->Intersects(rayStart, rayDirection, distance))
	{
		return true;
	}

	return false;
}