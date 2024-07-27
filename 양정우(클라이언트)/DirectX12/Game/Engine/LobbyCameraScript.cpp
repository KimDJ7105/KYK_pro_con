#include "pch.h"
#include "LobbyCameraScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"

#include "SoundManager.h"

#include "session.h"

void worker_SM_thread(boost::asio::io_context* io_con)
{
	io_con->run();

	delete main_session;

	std::cout << "메인 서버 쓰레드 종료\n";
}

LobbyCameraScript::LobbyCameraScript()
{
	GET_SINGLE(SoundManager)->soundPlay(BGM_MAIN_LOBBY, Vec3(0.f, 0.f, 0.f), true);
}

LobbyCameraScript::~LobbyCameraScript()
{
	
}


void LobbyCameraScript::LateUpdate()
{
	
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

	if(INPUT->GetButtonUp(KEY_TYPE::LBUTTON))
	{

		std::cout << "Pressed Button Type : " << GET_SINGLE(SceneManager)->GetButtonType() << std::endl;
		std::cout << "Pressed Button ID : " << GET_SINGLE(SceneManager)->GetButtonID() << std::endl;

		if (GET_SINGLE(SceneManager)->GetButtonType() == OT_UI_START_BTN)
		{
			GET_SINGLE(SoundManager)->soundStop(BGM_MAIN_LOBBY);

			GET_SINGLE(SoundManager)->soundPlay(GAME_START, Vec3(0.f, 0.f, 0.f), false);
			cl_packet_start_game sg;
			sg.type = CL_START_GAME;
			sg.size = sizeof(cl_packet_start_game);

			session->Send_Packet(&sg);

			GET_SINGLE(SceneManager)->LoadMainScene(L"TestScene");

			//서버칼(메인 씬 구성 종료지점)
			tcp::resolver resolver(main_io_con);
			auto endpoint = resolver.resolve(main_server_ip, main_server_port);

			tcp::socket sock(main_io_con);

			main_session = new SESSION(std::move(sock));

			MoveGuntype();
			
			main_session->do_connect(endpoint);

			serverthread_p = new std::thread(worker_SM_thread, &main_io_con);

			GET_SINGLE(SceneManager)->RemoveSceneObject(GET_SINGLE(SceneManager)->GetLobbyScene());
		}
		else if (GET_SINGLE(SceneManager)->GetButtonType() == OT_UI_WEAPON_BTN)
		{
			GET_SINGLE(SoundManager)->soundPlay(SELECT_WEAPON, Vec3(0.f, 0.f, 0.f), false);
			GetWeaponSelectUI();
		}
		else if(GET_SINGLE(SceneManager)->GetButtonType() == OT_UI_EXIT_BTN)
		{
			PostQuitMessage(0);
		}
		else if (GET_SINGLE(SceneManager)->GetButtonType() == OT_UI_WEAPON_CHANGE)
		{
			auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
			for (auto& gameObject : gameObjects)
			{
				if (gameObject->GetTransform()->GetObjectType() == OT_UI_WEAPON_SELECT)
				{
					if (gameObject->GetTransform()->GetObjectID() == GET_SINGLE(SceneManager)->GetButtonID())
					{
						gameObject->GetTransform()->SetLocalPosition(Vec3(-220.f, 350.f, 500.f));

						// 총선택칼
						session->set_guntype(gameObject->GetTransform()->GetObjectID());
						
						//GET_SINGLE(SceneManager)->GetButtonID()의 값이
						//0이면 기관단총	(GT_SM				0)
						//1이면 산탄총		(GT_SG				1)
						//2이면 돌격소총	(GT_AR				2)
						//3이면 저격소총	(GT_SR				3)
						GET_SINGLE(SceneManager)->SetMainWeapon_Type(gameObject->GetTransform()->GetObjectID());

						GET_SINGLE(SoundManager)->soundPlay(WEAPON_SELECTED, Vec3(0.f, 0.f, 0.f), false);
					}
					else
					{
						gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
					}
				}
			}
		}
	}
}

void LobbyCameraScript::GetWeaponSelectUI()
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() == OT_UI_TITLE)
		{
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		}
		else if (gameObject->GetTransform()->GetObjectType() == OT_UI_WEAPON_CHANGE)
		{
			if (gameObject->GetTransform()->GetObjectID() == 0)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-350.f, -350.f, 500.f));
			}
			else if (gameObject->GetTransform()->GetObjectID() == 1)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-350.f, -200.f, 500.f));
			}
			else if (gameObject->GetTransform()->GetObjectID() == 2)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-90.f, -350.f, 500.f));
			}
			else if (gameObject->GetTransform()->GetObjectID() == 3)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-90.f, -200.f, 500.f));
			}
			else if (gameObject->GetTransform()->GetObjectID() == 4)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-220.f, 100.f, 500.f));
			}
			else if (gameObject->GetTransform()->GetObjectID() == 5)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-220.f, -100.f, 500.f));
			}
			else if (gameObject->GetTransform()->GetObjectID() == 6)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-220.f - 150.f, 350.f + 75.f, 500.f));
			}
			else if (gameObject->GetTransform()->GetObjectID() == 7)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-220.f - 150.f, 100.f + 75.f, 500.f));
			}
			else if (gameObject->GetTransform()->GetObjectID() == 8)
			{
				gameObject->GetTransform()->SetLocalPosition(Vec3(-220.f, 350.f, 500.f));
			}
		}
	}
}
