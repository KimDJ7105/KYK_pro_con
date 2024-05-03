#pragma once

class SESSION;

class Game
{
public:

	void Init(const WindowInfo& info);
	void Update();

	/*void MoveThisSession(tcp::socket& sock);

	SESSION* GetGameSession();*/
};

