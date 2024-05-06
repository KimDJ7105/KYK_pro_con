#pragma once

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
	void LoadScene(std::wstring sceneName);

	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

	shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }
	void SetActiveScene(shared_ptr<Scene> activeScene) { activeScene = _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

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
	void ChangeObjectMovement(int object_id, float x, float y, float z, float dirX, float dirY, float dirZ);

	void CreateAisle(float aisleX, float aisleY, float aisleZ, float aisleScale);
	void CreateAisle2(float aisleX, float aisleY, float aisleZ, float aisleScale);
	void CreateMap(float mapX, float mapY, float mapZ, float aisleScale);

	void AddComputeShader(int threadX, int threadY, int threadZ);

	shared_ptr<GameObject> GetPlayer() { return _player; }

};

