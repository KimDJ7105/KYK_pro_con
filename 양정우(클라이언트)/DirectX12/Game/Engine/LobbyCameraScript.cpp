#include "pch.h"
#include "LobbyCameraScript.h"
#include "SceneManager.h"
#include "Input.h"
#include "Timer.h"
#include "GameObject.h"
#include "Transform.h"

#include "session.h"

void worker_SM_thread(boost::asio::io_context* io_con)
{
	io_con->run();

	delete main_session;

	std::cout << "메인 서버 쓰레드 종료\n";
}

LobbyCameraScript::LobbyCameraScript()
{
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



	if (INPUT->GetButtonDown(KEY_TYPE::SPACEBAR))
	{
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

		main_session->do_connect(endpoint);

		serverthread_p = new std::thread(worker_SM_thread, &main_io_con);

		//GET_SINGLE(SceneManager)->RemoveSceneObject(GET_SINGLE(SceneManager)->GetLobbyScene());
	}

	if(INPUT->GetButtonUp(KEY_TYPE::LBUTTON))
	{

		std::cout << "Pressed Button Type : " << GET_SINGLE(SceneManager)->GetButtonType() << std::endl;
		std::cout << "Pressed Button ID : " << GET_SINGLE(SceneManager)->GetButtonID() << std::endl;

		
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
	

}