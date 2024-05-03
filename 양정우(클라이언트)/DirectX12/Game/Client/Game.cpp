#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"
#include "session.h"

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	GET_SINGLE(SceneManager)->LoadScene(L"TestScene");
}

void Game::Update()
{
	GEngine->Update();
}

//void Game::MoveThisSession(tcp::socket& sock)
//{
//	MoveSession(sock);
//}
//
//SESSION* Game::GetGameSession()
//{
//	return GetSession();
//}