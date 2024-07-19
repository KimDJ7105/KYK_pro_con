#pragma once
#include "Scene.h"

using std::shared_ptr;
using std::wstring;

class Scene;
class GameObject;
class ObjectManager;

enum
{
	MAX_LAYER = 32
};



class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Update();
	void Render();
	void LoadMainScene(std::wstring sceneName);

	void LoadLobbyScene(std::wstring sceneName);

	void LoadEndingGameScene(std::wstring sceneName);

	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

	shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }
	void SetActiveScene(shared_ptr<Scene> activeScene) { activeScene = _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

	shared_ptr<Scene> LoadLobbyScene();

	shared_ptr<Scene> LoadEndingScene();

	shared_ptr<Scene> mainGameScene = std::make_shared<Scene>();

	shared_ptr<Scene> lobbyGameScene = std::make_shared<Scene>();

	shared_ptr<Scene> endingGameScene = std::make_shared<Scene>();

private:
	shared_ptr<Scene> _activeScene;

	std::array<wstring, MAX_LAYER> _layerNames;
	std::map<wstring, uint8> _layerIndex;

	//여기부턴 서버연결을 위한 도구--------------------------------
	shared_ptr<GameObject> _player;
	std::vector<shared_ptr<GameObject>> _otherPlayer;
	// TODO: bool 무언가 플레이어가 입장했음을 알수있는 판단변수

public:
	void CreateAvatar(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ);
	shared_ptr<GameObject> CreateBoxObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ);
	void CreatePlayerObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ);
	void CreatePlayerHandObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ);
	void CreatePlayerGunObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ);
	void CreatePlayerHeadCoreObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ);
	
	void ChangeObjectMovement(int object_id, float x, float y, float z, float dirX, float dirY, float dirZ, int animationID);
	void ChangeObjectAnimation(int object_id, int animationID);
	
	void CreateHeadCoreObject(int object_id);
	
	Vec3 FindAislePosition(int aisleNum);
	Vec3 FindRoomPosition(int roomNum);
	void CreateGameObject(int aisleNum, int object_type, int object_ID);

	void CreateAisle(float aisleX, float aisleY, float aisleZ, float aisleScale, int type, int ID);
	void CreateAisle2(float aisleX, float aisleY, float aisleZ, float aisleScale, int type, int ID);
	void CreateMap(float mapX, float mapY, float mapZ, float aisleScale, int type, int ID);
	void CreateOutDoor(float mapX, float mapY, float mapZ, float aisleScale, int ID);
	void CreateOutDoor2(float mapX, float mapY, float mapZ, float aisleScale, int ID);
	void CreateAABBBox(Vec3 aabbPosition, Vec3 aabbScale);
	int RenderAABBBox(Vec3 aabbPosition, Vec3 aabbScale);
	void UpdateAABBBox(int boxNum, Vec3 pos, Vec3 scale, Vec3 rotation);

	void AddComputeShader(int threadX, int threadY, int threadZ);

	shared_ptr<GameObject> GetPlayer() { return _player; }


	void SetPlayerID(int pID) { _playerID = pID; }
	shared_ptr<GameObject> CheckCollisionWithSceneObjects(const std::shared_ptr<GameObject>& objectToCheck, int object_Type);

private:
	int _playerID;

	int boxNum = 0;
	//충돌박스의 타입넘버는 99이다

public:
	shared_ptr<GameObject> GetPlayer(int ID);	//101

	shared_ptr<GameObject> GetPlayerGun(int ID);	//102

	shared_ptr<GameObject> GetPlayerHeadCoreQbject(int ID);


	void RemoveObject(int object_type, int object_id);


public:
	void SetMapPosition(int x, int y);
	void SetKeyCardPosition(int x, int y, int keyCardNum);


private:
	int playerHP = 100;
	int bullet = 30;

public:
	void CalculateHP(int damagedHP);

	void CalculateBullet(int nowBullet);
	void SetBullet(int BulletCount);
	void SetRabbitFootUI();

	void CreateMapObjectsUI(int object_type, int loc_type, int loc_num);
	Vec3 CalculateMapUIPosition(int loc_num);
	Vec3 CalculateAisleUIPosition(int loc_num);

	void RemoveMapUI();

	void SetPlayerLocation(float x, float y, float z, float dirx, float diry, float dirz);

	void CreateMovingObject(float x, float y, float z, float dirx, float diry, float dirz, int obj_id, int obj_type);

	void CreateCrusherBlade(int object_ID, float object_size, int blade_num, Vec3 crusherPos);


	void RemoveSceneObject(shared_ptr<Scene> scene_erase);


	shared_ptr<Scene> GetMainScene() { return mainGameScene; }

	shared_ptr<Scene> GetLobbyScene() { return lobbyGameScene; }

	shared_ptr<Scene> GetEndingScene() { return endingGameScene; }

private:
	int button_type = -1;
	int button_id = -1;

public:
	void SetButton(int btn_type, int btn_id);
	int GetButtonType() { return button_type; }
	int GetButtonID() { return button_id; }

	Vec3 GetLaserPosition(int room_num);

private:
	bool isPlayerDead = false;

public:
	void SetPlayerDead(bool isdead) { isPlayerDead = isdead; }
	bool GetPlayerDead() { return isPlayerDead; }
};

