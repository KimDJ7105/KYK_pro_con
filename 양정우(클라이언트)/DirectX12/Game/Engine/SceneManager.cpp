#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "ObjectManager.h"

#include "TestCameraScript.h"
#include "LobbyCameraScript.h"
#include "Resources.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MeshData.h"
#include "TestDragon.h"
#include "Animator.h"
#include "BladeRotateScript.h"
#include "CrusherScript.h"
#include "ButtonScript.h"
#include "EndingCameraScript.h"
#include "GoodEndingCameraScript.h"
#include "OtherPlayerScript.h"

#include "ObjectManager.h"

#include "session.h"




std::vector<MyGameObject> vp_ObjectManager;

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

// TEMP
void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

void SceneManager::LoadMainScene(std::wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadTestScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::LoadLobbyScene(std::wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadLobbyScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::LoadBadEndingGameScene(std::wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadBadEndingScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::LoadGoodEndingGameScene(std::wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadGoodEndingScene();

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// 기존 데이터 삭제
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}

shared_ptr<GameObject> SceneManager::Pick(int32 screenX, int32 screenY)
{
	shared_ptr<Camera> camera = GetActiveScene()->GetMainCamera();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	Matrix projectionMatrix = camera->GetProjectionMatrix();

	// ViewSpace에서 Picking 진행
	float viewX = (+2.0f * screenX / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / projectionMatrix(1, 1);

	Matrix viewMatrix = camera->GetViewMatrix();
	Matrix viewMatrixInv = viewMatrix.Invert();

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	float minDistance = FLT_MAX;
	shared_ptr<GameObject> picked;

	// ViewSpace에서의 Ray 정의
	Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);		//중심에서
	Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);		//여기로 쐈다.

	// WorldSpace에서의 Ray 정의
	rayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);	//그걸 3D로 표현
	rayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);		//그걸 3D로 표현
	rayDir.Normalize();	//방향을 노멀라이징한다.


	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCollider() == nullptr)
			continue;

		// WorldSpace에서 연산
		float distance = 0.f;
		if (gameObject->GetCollider()->Intersects(rayOrigin, rayDir, OUT distance) == false)
			continue;

		//만약 최소거리가 distance보다 클때, 즉 광선이 오브젝트한테 맞았을때
		if (distance < minDistance)
		{
			if (gameObject->GetTransform()->GetObjectType() == OT_PLAYER && gameObject->GetTransform()->GetObjectID() == _playerID)
				continue;

			minDistance = distance;	//최소거리를 갱신해주고
			picked = gameObject;	//맞은 오브젝트를 기록
		}
	}

	return picked;	//맞은 오브젝트를 리턴한다.
}

shared_ptr<GameObject> SceneManager::CheckCollisionWithSceneObjects(const std::shared_ptr<GameObject>& objectToCheck, int object_Type)
{
	//objectToCheck->GetTransform()->GetLocalPosition();


	//objectToChecks은 나 자신
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();


	std::shared_ptr<GameObject> collidedObject;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject == nullptr)
			continue;

		//collider컴포넌트가 없거나 자기 자신일때
		if (gameObject->GetCollider() == nullptr || gameObject == objectToCheck)
			continue;

		//collider 타입이 BOX가 아닐때.
		if (gameObject->GetCollider()->GetColliderType() != ColliderType::Box)
			continue;

		if (gameObject->GetTransform()->GetObjectType() != object_Type)
			continue;


		if (gameObject->GetTransform()->GetObjectType() == object_Type)
		{
			//체크하는 나 자신과 내가 원하는 게임오브젝트의 충돌판정을 시작한다.
			if (objectToCheck->GetCollider()->isColliding(gameObject->GetCollider()->GetBoxCollider()))
			{
				//만약 충돌을 했다면.
				return gameObject;
			}
		}

		
	}

	return NULL;
}

shared_ptr<GameObject> SceneManager::GetPlayer(int ID)
{

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_UI_PLAYERHAND)
			continue;

		if (gameObject->GetTransform()->GetObjectID() == ID)
			return gameObject;
	}


	return 0;
}

shared_ptr<GameObject> SceneManager::GetPlayerSubGun(int ID)
{

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_UI_SUB_WEAPON_3D)
			continue;

		if (gameObject->GetTransform()->GetObjectID() == ID)
			return gameObject;
	}


	return 0;
}

shared_ptr<GameObject> SceneManager::GetPlayerMainGun(int ID)
{

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_UI_MAIN_WEAPON_3D)
			continue;

		if (gameObject->GetTransform()->GetObjectID() == ID)
			return gameObject;
	}


	return 0;
}

shared_ptr<GameObject> SceneManager::GetPlayerHeadCoreQbject(int ID)
{

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_HEADCORE)
			continue;

		if (gameObject->GetTransform()->GetObjectID() == ID)
			return gameObject;
	}


	return 0;
}

shared_ptr<Scene> SceneManager::LoadGoodEndingScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
	//...
	//...
	//...
#pragma endregion

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());		// N = 1, F = 1000, FOV = 45

		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 0.f));

		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음

		camera->AddComponent(make_shared<GoodEndingCameraScript>());

		goodEndingGameScene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		goodEndingGameScene->AddGameObject(camera);
	}
#pragma endregion

#pragma region Deferred_Rendering_Info

	for (int32 i = 0; i < 6; i++)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));			// 1에서 1000사이 아무 값
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;
			if (i < 3)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			else if (i < 5)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			else
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
			//texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");//compute shader

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		//goodEndingGameScene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Main_UI
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Good_Ending_UI", L"..\\Resources\\Texture\\Ending_UI\\Good_Ending_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);

		badEndingGameScene->AddGameObject(sphere);
	}
#pragma endregion


	return goodEndingGameScene;

}

shared_ptr<Scene> SceneManager::LoadBadEndingScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
	//...
	//...
	//...
#pragma endregion

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());		// N = 1, F = 1000, FOV = 45

		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 0.f));

		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음

		camera->AddComponent(make_shared<EndingCameraScript>());

		badEndingGameScene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		badEndingGameScene->AddGameObject(camera);
	}
#pragma endregion

#pragma region Deferred_Rendering_Info

	for (int32 i = 0; i < 6; i++)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));			// 1에서 1000사이 아무 값
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;
			if (i < 3)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			else if (i < 5)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			else
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
			//texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");//compute shader

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		//endingGameScene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Main_UI
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Bad_Ending_UI", L"..\\Resources\\Texture\\Ending_UI\\Bad_Ending_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);

		badEndingGameScene->AddGameObject(sphere);
	}
#pragma endregion


	return badEndingGameScene;

}

shared_ptr<Scene> SceneManager::LoadLobbyScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
	//...
	//...
	//...
#pragma endregion

#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());		// N = 1, F = 1000, FOV = 45
		camera->AddComponent(make_shared<LobbyCameraScript>());

		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 0.f));

		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음

		lobbyGameScene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		lobbyGameScene->AddGameObject(camera);
	}
#pragma endregion

#pragma region Deferred_Rendering_Info

	for (int32 i = 0; i < 6; i++)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));			// 1에서 1000사이 아무 값
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;
			if (i < 3)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			else if (i < 5)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			else
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
			//texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");//compute shader

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		//lobbyGameScene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Main_UI
	//메인 UI 틀
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Lobby_Main_UI", L"..\\Resources\\Texture\\Lobby_UI\\PR_Main_Screen_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);

		lobbyGameScene->AddGameObject(sphere);
	}

	// Start버튼
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(470.f, 120.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Lobby_Start_UI", L"..\\Resources\\Texture\\Lobby_UI\\Game_Start_Button.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_START_BTN);
		//sphere->GetTransform()->SetObjectID();

		sphere->AddComponent(make_shared<ButtonScript>());

		lobbyGameScene->AddGameObject(sphere);
	}

	// 장비 버튼
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(470.f, 0.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Lobby_Weapon_UI", L"..\\Resources\\Texture\\Lobby_UI\\Weapon_Change_Button.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_BTN);
		//sphere->GetTransform()->SetObjectID();

		sphere->AddComponent(make_shared<ButtonScript>());

		lobbyGameScene->AddGameObject(sphere);
	}

	// 나가기 버튼
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.2, WINDOW_HEIGHT * 0.1, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(470.f, -120.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Lobby_End_UI", L"..\\Resources\\Texture\\Lobby_UI\\Exit_Button.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_EXIT_BTN);
		//sphere->GetTransform()->SetObjectID();

		sphere->AddComponent(make_shared<ButtonScript>());

		lobbyGameScene->AddGameObject(sphere);
	}
#pragma endregion

#pragma region OT_UI_TITLE
	//메인 타이틀 UI
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 531), ((WINDOW_HEIGHT / 1200.f) * 101), 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(-220.f, 0.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Lobby_Logo_UI", L"..\\Resources\\Texture\\Lobby_UI\\ReWalk_Logo_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_TITLE);
		//sphere->GetTransform()->SetObjectID();

		lobbyGameScene->AddGameObject(sphere);
	}
#pragma endregion

#pragma region OT_UI_WEAPON_CHANGE

	//선택박스_1
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 2.5, ((WINDOW_HEIGHT / 1200.f) * 536) / 2.5, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Empty_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\Empty_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(8);

		lobbyGameScene->AddGameObject(sphere);
	}

	//기관단총 선택
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 2.5, ((WINDOW_HEIGHT / 1200.f) * 536) / 2.5, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"SMG01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\SMG01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_SELECT);
		sphere->GetTransform()->SetObjectID(GT_SM);

		lobbyGameScene->AddGameObject(sphere);
	}

	//산탄총 선택
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 2.5, ((WINDOW_HEIGHT / 1200.f) * 536) / 2.5, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"SG01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\SG01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_SELECT);
		sphere->GetTransform()->SetObjectID(GT_SG);

		lobbyGameScene->AddGameObject(sphere);
	}

	//돌격소총 선택
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 2.5, ((WINDOW_HEIGHT / 1200.f) * 536) / 2.5, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"AR01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\AR01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_SELECT);
		sphere->GetTransform()->SetObjectID(GT_AR);

		lobbyGameScene->AddGameObject(sphere);
	}

	//저격소총 선택
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 2.5, ((WINDOW_HEIGHT / 1200.f) * 536) / 2.5, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"SR01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\SR01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_SELECT);
		sphere->GetTransform()->SetObjectID(GT_SR);

		lobbyGameScene->AddGameObject(sphere);
	}

	//선택박스_2
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 2.5, ((WINDOW_HEIGHT / 1200.f) * 536) / 2.5, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Empty_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\Empty_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(4);

		lobbyGameScene->AddGameObject(sphere);
	}
	//선택박스_2_보조무기
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 876) / 2.5, ((WINDOW_HEIGHT / 1200.f) * 537) / 2.5, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"P01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\P01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(4);

		lobbyGameScene->AddGameObject(sphere);
	}

	//선택박스_3_왼쪽 아래
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 4, ((WINDOW_HEIGHT / 1200.f) * 536) / 4, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Empty_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\Empty_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(0);
		sphere->AddComponent(make_shared<ButtonScript>());

		lobbyGameScene->AddGameObject(sphere);
	}
	//선택박스_3_왼쪽 아래_기관단총
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 876) / 4, ((WINDOW_HEIGHT / 1200.f) * 537) / 4, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"SMG01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\SMG01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(0);

		lobbyGameScene->AddGameObject(sphere);
	}

	//선택박스_4_왼쪽 위
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 4, ((WINDOW_HEIGHT / 1200.f) * 536) / 4, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Empty_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\Empty_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(1);
		sphere->AddComponent(make_shared<ButtonScript>());

		lobbyGameScene->AddGameObject(sphere);
	}
	//선택박스_4_왼쪽 위_산탄총
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 879) / 4, ((WINDOW_HEIGHT / 1200.f) * 536) / 4, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"SG01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\SG01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(1);

		lobbyGameScene->AddGameObject(sphere);
	}

	//선택박스_5_오른쪽 아래
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 4, ((WINDOW_HEIGHT / 1200.f) * 536) / 4, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Empty_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\Empty_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(2);
		sphere->AddComponent(make_shared<ButtonScript>());

		lobbyGameScene->AddGameObject(sphere);
	}
	//선택박스_5_오른쪽 아래_돌격소총
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 876) / 4, ((WINDOW_HEIGHT / 1200.f) * 539) / 4, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"AR01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\AR01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(2);

		lobbyGameScene->AddGameObject(sphere);
	}

	//선택박스_6_오른쪽 위
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 875) / 4, ((WINDOW_HEIGHT / 1200.f) * 536) / 4, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Empty_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\Empty_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(3);
		sphere->AddComponent(make_shared<ButtonScript>());

		lobbyGameScene->AddGameObject(sphere);
	}
	//선택박스_6_오른쪽 위_저격소총
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 876) / 4, ((WINDOW_HEIGHT / 1200.f) * 537) / 4, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"SR01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\SR01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(3);

		lobbyGameScene->AddGameObject(sphere);
	}
	

	//주무기선택 UI
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 482) / 2, ((WINDOW_HEIGHT / 1200.f) * 176) / 2, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"PrimaryList_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\PrimaryList_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(5);

		lobbyGameScene->AddGameObject(sphere);
	}

	//선택박스_1_번호
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 196) / 3, ((WINDOW_HEIGHT / 1200.f) * 193) / 3, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Number01_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\Number01_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(6);

		lobbyGameScene->AddGameObject(sphere);
	}

	//선택박스_2_번호
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(((WINDOW_WIDTH / 1600.f) * 196) / 3, ((WINDOW_HEIGHT / 1200.f) * 193) / 3, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Number02_Main_UI", L"..\\Resources\\Texture\\Lobby_UI_Weapon\\Number02_Main_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_WEAPON_CHANGE);
		sphere->GetTransform()->SetObjectID(7);

		lobbyGameScene->AddGameObject(sphere);
	}

#pragma endregion

	//Cursor
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.02, WINDOW_HEIGHT * 0.02, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Cursor", L"..\\Resources\\Texture\\Cursor.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_CURSOR);

		lobbyGameScene->AddGameObject(sphere);
	}

	return lobbyGameScene;
}


shared_ptr<Scene> SceneManager::LoadTestScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
	//...
	//...
	//...
#pragma endregion

#pragma region ComputeShader
	//AddComputeShader(1, 1024, 1);
#pragma endregion

	//shared_ptr<Scene> scene = make_shared<Scene>(); 전역으로 올림 왜? 변수 쓸라고

	// 메인카메라를 가장 먼저 추가(안그럼 버그남)
#pragma region Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());		// N = 1, F = 1000, FOV = 45
		camera->AddComponent(make_shared<TestCameraScript>());

		camera->GetCamera()->SetFar(10000.f);
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 0.f));

		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음

		camera->GetTransform()->SetObjectType(OT_PLAYER_CAMERA);

		mainGameScene->AddGameObject(camera);

		

		_player = camera;//카메라를 플레이어로 설정
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		mainGameScene->AddGameObject(camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");

			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Space.jpg");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		mainGameScene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Sphere(Object)
	{
		//shared_ptr<GameObject> obj = make_shared<GameObject>();
		//obj->SetName(L"OBJ");
		//obj->AddComponent(make_shared<Transform>());
		//obj->AddComponent(make_shared<SphereCollider>()); // 이것을 추가함으로서 픽킹의 적용을 받는다.
		//obj->GetTransform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));
		//obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		//obj->SetStatic(false);	// false로 하여 그림자의 적용을 받는다
		//shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		//{
		//	shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
		//	meshRenderer->SetMesh(sphereMesh);
		//}
		//{
		//	/*shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
		//	shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
		//	shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
		//	shared_ptr<Material> material = make_shared<Material>();
		//	material->SetShader(shader);
		//	material->SetTexture(0, texture);
		//	material->SetTexture(1, texture2);
		//	meshRenderer->SetMaterial(material);*/
		//	// Resource.cpp GameObject부분으로 이동

		//	shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
		//	//material->SetInt(0, 1);
		//	meshRenderer->SetMaterial(material->Clone());
		//	/*material->SetInt(0, 0);
		//	meshRenderer->SetMaterial(material->Clone());*/
		//}

		//std::dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetRadius(0.5f);
		//std::dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));

		//obj->AddComponent(meshRenderer);
		//scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Terrain
	{
		/*shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->AddComponent(make_shared<Terrain>());
		obj->AddComponent(make_shared<MeshRenderer>());

		obj->GetTransform()->SetLocalScale(Vec3(50.f, 250.f, 50.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-100.f, -200.f, 300.f));
		obj->SetStatic(true);
		obj->GetTerrain()->Init(64, 64);
		obj->SetCheckFrustum(false);

		scene->AddGameObject(obj);*/
	}
#pragma endregion

#pragma region Plane
	{
		/*shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(1000.f, 1.f, 1000.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, -100.f, 500.f));
		obj->SetStatic(true);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject")->Clone();
			material->SetInt(0, 0);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);*/
	}
#pragma endregion

#pragma region Cube
	/*{
		shared_ptr<GameObject> cube = make_shared<GameObject>();
		cube->AddComponent(make_shared<Transform>());
		cube->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
		cube->GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 30.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(cubeMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
			shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			material->SetTexture(1, texture2);
			meshRenderer->SetMaterial(material);
		}
		cube->AddComponent(meshRenderer);
		scene->AddGameObject(cube);

		_otherPlayer.push_back(cube);
	}*/
#pragma endregion

#pragma region UI

	//CrossHair
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_HEIGHT * 0.05, WINDOW_HEIGHT * 0.05, 50.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(0, 0, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Crosshair", L"..\\Resources\\Texture\\PR_CROSSHAIR01_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		mainGameScene->AddGameObject(sphere);
	}

	//HP틀
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.8, WINDOW_WIDTH * 0.8, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(0, (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)), 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"HP_Main", L"..\\Resources\\Texture\\PR_HP_BAR_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		mainGameScene->AddGameObject(sphere);
	}

	//HP 노드
	for(int i = 0; i < 10; i++)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3((WINDOW_WIDTH * 0.7) * 0.1, (WINDOW_WIDTH * 0.7) * 0.1, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(-(WINDOW_WIDTH / 2) + 510 + 65 * i, (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)), 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"HP_Sub", L"..\\Resources\\Texture\\PR_HP_SQUARE_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);

		sphere->GetTransform()->SetObjectType(OT_UI_HP);
		sphere->GetTransform()->SetObjectID(i);

		mainGameScene->AddGameObject(sphere);
	}

	//KeyCard
	for(int i = 0; i < 3; i++)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_HEIGHT * 0.1, WINDOW_HEIGHT * 0.1, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(11111111111111111, 11111111111111111, 500.f));//0, 0, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"KeyCard_Item", L"..\\Resources\\Texture\\PR_KEYCARD_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_KEYCARD);
		sphere->GetTransform()->SetObjectID(i);

		mainGameScene->AddGameObject(sphere);
	}

	//slash
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_HEIGHT * 0.3, WINDOW_HEIGHT * 0.3, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(0, -(WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)), 500.f));//0, 0, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"SlashUI", L"..\\Resources\\Texture\\Number\\PR_SLASH_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_SLASH);
		sphere->GetTransform()->SetObjectID(1);

		mainGameScene->AddGameObject(sphere);
	}

	//MaxBullet
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_HEIGHT * 0.2, WINDOW_HEIGHT * 0.2, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(30, -(WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)), 500.f));//0, 0, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"PR_NUM3_UI", L"..\\Resources\\Texture\\Number\\PR_NUM3_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_MAXBULLET);
		sphere->GetTransform()->SetObjectID(1);

		mainGameScene->AddGameObject(sphere);
	}
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_HEIGHT * 0.2, WINDOW_HEIGHT * 0.2, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(60, -(WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)), 500.f));//0, 0, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"PR_NUM0_UI", L"..\\Resources\\Texture\\Number\\PR_NUM0_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_MAXBULLET);
		sphere->GetTransform()->SetObjectID(1);

		mainGameScene->AddGameObject(sphere);
	}

	//NowBullet
	for (int i = 0; i < 10; i++)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_HEIGHT * 0.3, WINDOW_HEIGHT * 0.3, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f));//0, 0, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			wstring fileName = L"PR_NUM" + std::to_wstring(i) + L"_UI";
			wstring filePath = L"..\\Resources\\Texture\\Number\\" + fileName + L".png";
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(fileName, filePath);

			//shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"PR_NUM0_UI", L"..\\Resources\\Texture\\Number\\PR_NUM0_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_CURRENTBULLET);
		sphere->GetTransform()->SetObjectID(i);

		mainGameScene->AddGameObject(sphere);
	}
	for (int i = 10; i < 20; i++)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_HEIGHT * 0.3, WINDOW_HEIGHT * 0.3, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f));//0, 0, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			wstring fileName = L"PR_NUM" + std::to_wstring(i - 10) + L"_UI";
			wstring filePath = L"..\\Resources\\Texture\\Number\\" + fileName + L".png";
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(fileName, filePath);

			//shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"PR_NUM3_UI", L"..\\Resources\\Texture\\Number\\PR_NUM3_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_CURRENTBULLET);
		sphere->GetTransform()->SetObjectID(i);

		mainGameScene->AddGameObject(sphere);
	}


	//Gun UI
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_HEIGHT * 0.5, WINDOW_HEIGHT * 0.5, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(0, 150 -(WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)), 500.f));//0, 0, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"GunUI", L"..\\Resources\\Texture\\PR_SMG01_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_SMG);
		sphere->GetTransform()->SetObjectID(1);

		mainGameScene->AddGameObject(sphere);
	}
	//Rabbit Foot
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.05, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"RabbitFoot_UI", L"..\\Resources\\Texture\\PR_Rabbitfoot_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_RABBITFOOT);
		sphere->GetTransform()->SetObjectID(1);

		mainGameScene->AddGameObject(sphere);
	}

	//Map 틀
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH, WINDOW_HEIGHT, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(1111111111111110, 1111111111111111110, 500.f));//0, 0, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Map_under", L"..\\Resources\\Texture\\PR_BACKGROUND_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_MAP);
		sphere->GetTransform()->SetObjectID(1);

		mainGameScene->AddGameObject(sphere);
	}
	//Map
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH /2, WINDOW_HEIGHT / 2, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(111111111110, 11111111111111130, 500.f));	//0, 30, 500;
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Map", L"..\\Resources\\Texture\\PR_MAP_UI.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_UI_MAP);
		sphere->GetTransform()->SetObjectID(2);
		
		mainGameScene->AddGameObject(sphere);
	}
	//Red_Square
	for (int i = 0; i < 25; i++)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.06, WINDOW_HEIGHT * 0.06, 500.f));
		sphere->GetTransform()->SetLocalPosition(/*CalculateMapUIPosition(i)*/Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Red_Square", L"..\\Resources\\Texture\\Red_Square.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);

		sphere->GetTransform()->SetObjectType(OT_UI_MAP_BUTTON);
		sphere->GetTransform()->SetObjectID(i);
		sphere->AddComponent(make_shared<ButtonScript>());

		mainGameScene->AddGameObject(sphere);
	}

#pragma endregion

#pragma region Deferred_Rendering_Info

	for (int32 i = 0; i < 6; i++)
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));			// 1에서 1000사이 아무 값
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;
			if (i < 3)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			else if (i < 5)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
			else
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
				//texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");//compute shader

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		//scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 50.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));		// WHITE
		light->GetLight()->SetAmbient(Vec3(0.f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		mainGameScene->AddGameObject(light);
	}
#pragma endregion

#pragma region Point Light
	//{
	//	shared_ptr<GameObject> light = make_shared<GameObject>();
	//	light->AddComponent(make_shared<Transform>());
	//	light->GetTransform()->SetLocalPosition(Vec3(1200.f, 374.f, 1200.f));
	//	light->AddComponent(make_shared<Light>());
	//	//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0.f));
	//	light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
	//	light->GetLight()->SetDiffuse(Vec3(1.f, 1.0f, 1.f));		// GREEN
	//	light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	//	light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
	//	light->GetLight()->SetLightRange(400.f);
	//	//light->GetLight()->SetLightAngle(XM_PI / 4);
	//	mainGameScene->AddGameObject(light);
	//}
	for (int j = 0; j < 5; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			shared_ptr<GameObject> light = make_shared<GameObject>();
			light->AddComponent(make_shared<Transform>());
			light->GetTransform()->SetLocalPosition(Vec3(i * 600.f, 374.f, j * 600.f));
			light->AddComponent(make_shared<Light>());
			//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0.f));
			light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
			light->GetLight()->SetDiffuse(Vec3(1.f, 1.0f, 1.f));		// GREEN
			light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
			light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
			light->GetLight()->SetLightRange(400.f);
			//light->GetLight()->SetLightAngle(XM_PI / 4);
			mainGameScene->AddGameObject(light);
		}
	}
#pragma endregion

#pragma region Spot Light
	//{
	//	shared_ptr<GameObject> light = make_shared<GameObject>();
	//	light->AddComponent(make_shared<Transform>());
	//	light->GetTransform()->SetLocalPosition(Vec3(1200.f, 374.f, 1200.f));
	//	light->AddComponent(make_shared<Light>());
	//	light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
	//	light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
	//	light->GetLight()->SetDiffuse(Vec3(1.0f, 1.f, 1.0f));		// Color
	//	light->GetLight()->SetAmbient(Vec3(0.f, 0.f, 0.1f));
	//	light->GetLight()->SetSpecular(Vec3(0.f, 0.f, 0.1f));
	//	light->GetLight()->SetLightRange(400.f);
	//	light->GetLight()->SetLightAngle(XM_PI / 4);
	//	mainGameScene->AddGameObject(light);
	//}
#pragma endregion

#pragma region ParticleSystem
	/*{
		shared_ptr<GameObject> particle = make_shared<GameObject>();
		particle->AddComponent(make_shared<Transform>());
		particle->AddComponent(make_shared<ParticleSystem>());
		particle->SetCheckFrustum(false);
		particle->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 100.f));
		scene->AddGameObject(particle);
	}*/
#pragma endregion

#pragma region Tessellation Test
	{
		/*shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->AddComponent(make_shared<Transform>());
		gameObject->GetTransform()->SetLocalPosition(Vec3(0, 0, 300));
		gameObject->GetTransform()->SetLocalScale(Vec3(100, 100, 100));
		gameObject->GetTransform()->SetLocalRotation(Vec3(0, 0, 0));

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
			meshRenderer->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Tessellation"));
		}
		gameObject->AddComponent(meshRenderer);

		scene->AddGameObject(gameObject);*/
	}
#pragma endregion

#pragma region FBX Player
	{
		{
			//플레이어의 애니메이션 모델이 복합적으로 있는 모델을 불러오는 함수
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadPlayerModel(L"..\\Resources\\FBX\\Player2\\Player_Walk.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Player1");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);

				//gameObject->AddComponent(make_shared<TestDragon>());
				//std::dynamic_pointer_cast<TestDragon>(gameObject->GetMeshRenderer())->Set(2);

				gameObject->GetTransform()->SetObjectID(999);
				gameObject->GetTransform()->SetObjectType(999);

				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}


				mainGameScene->AddGameObject(gameObject);
				
			}
		}
		{
			//플레이어의 애니메이션 모델이 복합적으로 있는 모델을 불러오는 함수
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadGunAnimation(L"..\\Resources\\FBX\\Player_Gun2\\test01.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"PlayerGunAnimation1");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalPosition(Vec3(5.f, 13.f, 15.f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				//gameObject->AddComponent(make_shared<TestDragon>());
				//std::dynamic_pointer_cast<TestDragon>(gameObject->GetMeshRenderer())->Set(2);

				gameObject->GetTransform()->SetObjectID(999);
				gameObject->GetTransform()->SetObjectType(999);

				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}
				

				mainGameScene->AddGameObject(gameObject);
				
			}
		}



		{
			//플레이어의 애니메이션 모델이 복합적으로 있는 모델을 불러오는 함수
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadPlayerModel(L"..\\Resources\\FBX\\Player2\\Player_Walk.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Player2");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalPosition(Vec3(0.f + 10.f, 0.f, 0.f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);

				//gameObject->AddComponent(make_shared<TestDragon>());
				//std::dynamic_pointer_cast<TestDragon>(gameObject->GetMeshRenderer())->Set(2);

				gameObject->GetTransform()->SetObjectID(9999);
				gameObject->GetTransform()->SetObjectType(9999);

				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}

				mainGameScene->AddGameObject(gameObject);

			}
		}
		{
			//플레이어의 애니메이션 모델이 복합적으로 있는 모델을 불러오는 함수
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadGunAnimation(L"..\\Resources\\FBX\\Player_Gun2\\test01.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"PlayerGunAnimation2");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalPosition(Vec3(5.f + 10.f, 13.f, 15.f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				//gameObject->AddComponent(make_shared<TestDragon>());
				//std::dynamic_pointer_cast<TestDragon>(gameObject->GetMeshRenderer())->Set(2);

				gameObject->GetTransform()->SetObjectID(9999);
				gameObject->GetTransform()->SetObjectType(9999);


				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}

				mainGameScene->AddGameObject(gameObject);

			}
		}


		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Player6\\dddd.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Player0");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
				//gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				mainGameScene->AddGameObject(gameObject);
				//gameObject->AddComponent(make_shared<TestDragon>());
			}
		}
	}
#pragma endregion
	
#pragma region Team_Player
	//플레이어들 소환 코드
	//y = 0.f
	// Blue
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBlueTeamModel(L"..\\Resources\\FBX\\PlayerBlue\\IDLE_2\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Player99");
			gameObject->SetCheckFrustum(false);

			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalPosition(Vec3(1214.f, 0.f, 1200.f));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			//gameObject->AddComponent(make_shared<TestDragon>());
			//gameObject->AddComponent(make_shared<OtherPlayerScript>());

			//_otherPlayer.push_back(gameObject);
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//// Green
	//{
	//	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadGreenTeamModel(L"..\\Resources\\FBX\\PlayerBlue\\IDLE_2\\Green_Player_Single_IShoot.fbx");
	//	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetName(L"Player100");
	//		gameObject->SetCheckFrustum(false);
	//		gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	//		//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
	//		gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	//		gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
	//		gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
	//		// 각 게임 오브젝트에 독립적인 머티리얼 설정
	//		for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
	//		{
	//			shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
	//			gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
	//		}
	//		mainGameScene->AddGameObject(gameObject);
	//	}
	//}

	//// Red
	//{
	//	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadRedTeamModel(L"..\\Resources\\FBX\\PlayerRed\\IDLE_2\\Red_Player_Single_IShoot.fbx");
	//	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetName(L"Player101");
	//		gameObject->SetCheckFrustum(false);
	//		gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	//		//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
	//		gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	//		gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
	//		gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
	//		// 각 게임 오브젝트에 독립적인 머티리얼 설정
	//		for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
	//		{
	//			shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
	//			gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
	//		}
	//		mainGameScene->AddGameObject(gameObject);
	//	}
	//}

	//// Purple
	//{
	//	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadPurpleTeamModel(L"..\\Resources\\FBX\\PlayerPurple\\IDLE_2\\Purple_Player_Single_IShoot.fbx");
	//	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetName(L"Player102");
	//		gameObject->SetCheckFrustum(false);
	//		gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	//		//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
	//		gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	//		gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
	//		gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
	//		// 각 게임 오브젝트에 독립적인 머티리얼 설정
	//		for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
	//		{
	//			shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
	//			gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
	//		}
	//		mainGameScene->AddGameObject(gameObject);
	//	}
	//}
#pragma endregion

#pragma region Guns
	//총 소환 코드

	//y = 40.f
	
	//돌격소총
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_AR_GunModel(L"..\\Resources\\FBX\\PlayerGun_AR\\IDLE_2\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AR");
			gameObject->SetCheckFrustum(false);

			//gameObject->GetTransform()->SetLocalPosition(Vec3(1200.f + 5, 25.f, 1200.f + 13));
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.1f, 0.1f, 0.1f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			//gameObject->AddComponent(make_shared<TestDragon>());

			//_otherPlayer.push_back(gameObject);
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//권총
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_PO_GunModel(L"..\\Resources\\FBX\\PlayerGun_PO\\IDLE_2\\PO01_Player_Single_IShoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"PO");
			gameObject->SetCheckFrustum(false);

			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.1f, 0.1f, 0.1));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}

			//_otherPlayer.push_back(gameObject);
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//샷건
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SG_GunModel(L"..\\Resources\\FBX\\PlayerGun_SG\\IDLE_2\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"SG");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//기관단총
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SMG_GunModel(L"..\\Resources\\FBX\\PlayerGun_SM\\IDLE_2\\SMG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"SMG");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalPosition(Vec3(1200.f, 40.f, 1200.f));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			//gameObject->AddComponent(make_shared<TestDragon>());
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//스나이퍼
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SR_GunModel(L"..\\Resources\\FBX\\PlayerGun_SR\\IDLE_2\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"SR");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalPosition(Vec3(1250.f, 40.f, 1200.f));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			//gameObject->AddComponent(make_shared<TestDragon>());
			mainGameScene->AddGameObject(gameObject);
		}
	}
#pragma endregion
	
#pragma region FPS_Hand
	// 손 소환 코드
	
	//Vec3 hand = Vec3(1200.f, 18.f, 1200.f);
	// y = 0.f
	// Blue_Hand
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBlueHandModel(L"..\\Resources\\FBX\\Player_Hand_Blue\\IDLE\\Blue_FP_Shoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Blue_Hand");
			gameObject->SetCheckFrustum(false);
			//gameObject->GetTransform()->SetLocalPosition(hand);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.03f, 0.03f, 0.03f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//// Green_Hand
	//{
	//	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadGreenHandModel(L"..\\Resources\\FBX\\Player_Hand_Green\\IDLE\\Green_FP_Shoot.fbx");
	//	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetName(L"Green_Hand");
	//		gameObject->SetCheckFrustum(false);
	//		gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	//		//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
	//		gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	//		gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
	//		gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
	//		// 각 게임 오브젝트에 독립적인 머티리얼 설정
	//		for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
	//		{
	//			shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
	//			gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
	//		}
	//		mainGameScene->AddGameObject(gameObject);
	//	}
	//}

	//// Red_Hand
	//{
	//	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadRedHandModel(L"..\\Resources\\FBX\\Player_Hand_Red\\IDLE\\Red_FP_Shoot.fbx");
	//	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetName(L"Red_Hand");
	//		gameObject->SetCheckFrustum(false);
	//		gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	//		//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
	//		gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	//		gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
	//		gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
	//		// 각 게임 오브젝트에 독립적인 머티리얼 설정
	//		for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
	//		{
	//			shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
	//			gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
	//		}
	//		mainGameScene->AddGameObject(gameObject);
	//	}
	//}

	//// Purple_Hand
	//{
	//	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadPurpleHandModel(L"..\\Resources\\FBX\\Player_Hand_Purple\\IDLE\\Purple_FP_Shoot.fbx");
	//	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetName(L"Purple_Hand");
	//		gameObject->SetCheckFrustum(false);
	//		gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
	//		//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
	//		gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	//		gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
	//		gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
	//		// 각 게임 오브젝트에 독립적인 머티리얼 설정
	//		for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
	//		{
	//			shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
	//			gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
	//		}
	//		mainGameScene->AddGameObject(gameObject);
	//	}
	//}
#pragma endregion

#pragma region FPS_Gun

	// y = 40.f

	//AR_FPS
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_AR_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_AR\\IDLE\\AR01_FP_Shoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AR_FPS");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//PO_FPS
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_PO_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_PO\\IDLE\\P01_FP_Shoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"PO_FPS");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalPosition(Vec3(hand.x + 2.5f, hand.y + 20.f, hand.z + 9.5f));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			gameObject->AddComponent(make_shared<TestDragon>());

			mainGameScene->AddGameObject(gameObject);
		}
	}

	//SG_FPS
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SG_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_SG\\IDLE\\SG01_FP_Shoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"SG_FPS");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//SMG_FPS
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SMG_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_SMG\\IDLE\\SMG01_FP_Shoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"SMG_FPS");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(1150.f, 40.f, 1200.f));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			gameObject->AddComponent(make_shared<TestDragon>());
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//SR_FPS
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SR_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_SR\\IDLE\\SR01_FP_Shoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"SR_FPS");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(1100.f, 40.f, 1200.f));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			gameObject->AddComponent(make_shared<TestDragon>());
			mainGameScene->AddGameObject(gameObject);
		}
	}
#pragma endregion

#pragma region All Map
	int x = 0;
	int y = 0;
	
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{

			CreateAisle((375 + 225) * i, 0, 300 + j * 600, 150, OT_CORRIDOR, x);
			x++;
		}
		if (i != 4)
		{
			for (int j = 0; j < 5; j++)
			{
				CreateAisle2(300 + i * 600, 0, (375 + 225) * j, 150, OT_CORRIDOR, x);
				x++;
			}
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			CreateMap((375 + 225) * i, 0, (375 + 225) * j, 150, OT_ROOM, y);
			y++;
		}
	}
	
	for (int i = 0; i < 5; i++)
	{
		CreateOutDoor(-180, 0, 600 * i, 150.f, x);
		x++;
		CreateOutDoor(2400 + 180, 0, 600 * i, 150.f, x);
		x++;

		CreateOutDoor2(600 * i, 0, -180, 150.f, x);
		x++;
		CreateOutDoor2(600 * i, 0, 2400 + 180, 150.f, x);
		x++;
	}
#pragma endregion

	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\SMG01.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"MeGun");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));		//0.f, 45.f, 100.f
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			//gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\CardKey.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"CardKey");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Console.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"Console");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Object.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"RabbitFoot");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}

	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\RevivePad.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"RevivalPad");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}


	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Exit.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"Exit");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{

		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.02, WINDOW_HEIGHT * 0.02, 500.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Cursor", L"..\\Resources\\Texture\\Cursor.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		sphere->GetTransform()->SetObjectType(OT_CURSOR);

		mainGameScene->AddGameObject(sphere);
	}


	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AmmoBox.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"AmmoBox");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));


			mainGameScene->AddGameObject(gameObject);
		}
	}

	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Medkit.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"Medkit");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));


			mainGameScene->AddGameObject(gameObject);
		}
	}

	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\HeadCore_Blue.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"HeadCore_Blue");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));

			mainGameScene->AddGameObject(gameObject);
		}
	}

	return mainGameScene;
}


void SceneManager::CreateCrusherBlade(int object_ID, float object_size, int blade_num, Vec3 crusherPos)
{
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\CrusherBlade01.bin");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Crusher_Blade");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(crusherPos.x, crusherPos.y - (object_size / 2), crusherPos.z + ((object_size * blade_num * 2) / 5)));
			gameObject->GetTransform()->SetLocalScale(Vec3(object_size, object_size, object_size));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);

			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}


			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3((meshData->GetAABBExtents().x * object_size), (meshData->GetAABBExtents().y * object_size), (meshData->GetAABBExtents().z * object_size)));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(crusherPos.x, crusherPos.y - (object_size / 2), crusherPos.z + ((object_size * blade_num * 2) / 5)));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);


			gameObject->GetTransform()->SetObjectType(OT_CRUSHER_BLADE);
			gameObject->GetTransform()->SetObjectID(object_ID);

			mainGameScene->AddGameObject(gameObject);
		}
	}
}

void SceneManager::CreateAvatar(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ)
{
	{
		mainGameScene->GetMainCamera()->GetTransform()->SetLocalPosition(Vec3(x, y, z));
		mainGameScene->GetMainCamera()->GetTransform()->SetObjectType(object_type);
		mainGameScene->GetMainCamera()->GetTransform()->SetObjectID(object_id);
		mainGameScene->GetMainCamera()->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));
	}
}

void SceneManager::RemoveObject(int object_type, int object_id)
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject == nullptr)
			continue;
		//if (gameObject->GetTransform()->GetObjectID() != object_type)
		//	continue;
		//if (gameObject->GetTransform()->GetObjectID() != object_id)
		//	continue;

		if (gameObject->GetTransform()->GetObjectID() == object_id
			&& gameObject->GetTransform()->GetObjectType() == object_type)
		{
			GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(gameObject);
			break;
		}
	}
}

void SceneManager::RemoveSceneObject(shared_ptr<Scene> scene_erase)
{
	auto& gameObjects = scene_erase->GetGameObjects();
	std::vector<std::shared_ptr<GameObject>> objectsToRemove;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject == nullptr)
			continue;
		objectsToRemove.push_back(gameObject);
	}

	for (auto& gameObject : objectsToRemove)
	{
		scene_erase->RemoveGameObject(gameObject);
	}

}

void SceneManager::SetButton(int btn_type, int btn_id)
{
	button_type = btn_type;
	button_id = btn_id;
}

Vec3 SceneManager::GetLaserPosition(int room_num)
{
	int col = room_num / 5;
	int row = room_num % 5;

	// Calculate the center position of the room
	float x = col * 600.f;
	float z = row * 600.f;
	float y = 0.0f; // Assuming y is always 0 based on the problem description

	return Vec3(x, y, z);
}

void SceneManager::RemoveMapUI()
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();


	std::vector <std::shared_ptr< GameObject >> toRemove;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject == nullptr)
			continue;

		if (gameObject->GetTransform()->GetObjectType() == OT_UI_MAPOBJECTS)
		{
			toRemove.push_back(gameObject);
		}
	}

	for (auto& gameObject : toRemove)
	{
		GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(gameObject);
	}
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() == OT_UI_MAP_BUTTON)
		{
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
		}
	}
}

void SceneManager::SetMapPosition(int x, int y)
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() == OT_UI_MAP)
		{
			if (gameObject->GetTransform()->GetObjectID() == 1)
			{
				Vec3 pos = gameObject->GetTransform()->GetLocalPosition();

				pos.x = x;
				pos.y = y;

				gameObject->GetTransform()->SetLocalPosition(pos);
			}
			if (gameObject->GetTransform()->GetObjectID() == 2)
			{
				Vec3 pos = gameObject->GetTransform()->GetLocalPosition();

				pos.x = x;
				pos.y = y + 30.f;

				gameObject->GetTransform()->SetLocalPosition(pos);
			}
		}
		else if (gameObject->GetTransform()->GetObjectType() == OT_UI_MAP_BUTTON)
		{
			gameObject->GetTransform()->SetLocalPosition(CalculateMapUIPosition(gameObject->GetTransform()->GetObjectID()));
		}
	}
}

Vec3 SceneManager::CalculateMapUIPosition(int loc_num)
{
	// 초기 좌표
	float initial_x = -344.0f;
	float initial_y = -227.0f;
	float initial_z = 500.0f;

	// 거리 차이
	float x_distance = 172.0f;
	float y_distance = 128.5f;

	// 좌표 계산
	float x = initial_x + x_distance * (loc_num / 5);
	float y = initial_y + y_distance * (loc_num % 5 );
	float z = initial_z;

	return Vec3(x, y, z);
}

Vec3 SceneManager::CalculateAisleUIPosition(int loc_num)
{
	float x, y = 0.0f, z = 500.0f;

	if (loc_num >= 0 && loc_num <= 3) {
		x = -344.0f + loc_num;
		y = -162.75f + 128.5f * loc_num;
	}
	else if (loc_num >= 4 && loc_num <= 8) {
		x = -258.0f + (loc_num - 4);
		y = -227.0f + 128.5f * (loc_num - 4);
	}
	else if (loc_num >= 9 && loc_num <= 12) {
		x = -172.0f + loc_num - 9;
		y = -162.75f + 128.5f * (loc_num - 9);
	}
	else if (loc_num >= 13 && loc_num <= 17) {
		x = -86.0f + loc_num - 13;
		y = -227.0f + 128.5f * (loc_num - 13);
	}
	else if (loc_num >= 18 && loc_num <= 21) {
		x = 0.0f + loc_num - 18;
		y = -162.75f + 128.5f * (loc_num - 18);
	}
	else if (loc_num >= 22 && loc_num <= 26) {
		x = 86.0f + loc_num - 22;
		y = -227.0f + 128.5f * (loc_num - 22);
	}
	else if (loc_num >= 27 && loc_num <= 30) {
		x = 172.0f + loc_num - 27;
		y = -162.75f + 128.5f * (loc_num - 27);
	}
	else if (loc_num >= 31 && loc_num <= 35) {
		x = 258.0f + loc_num - 31;
		y = -227.0f + 128.5f * (loc_num - 31);
	}
	else if (loc_num >= 36 && loc_num <= 39) {
		x = 344.0f + loc_num - 36;
		y = -162.75f + 128.5f * (loc_num - 36);
	}
	else {
		return Vec3(0, 0, 0);
	}

	return Vec3(x, y, z);
}

void SceneManager::CreateMapObjectsUI(int object_type, int loc_type, int loc_num)
{
	if (loc_type == OT_CORRIDOR)
	{
		Vec3 aislePos = CalculateAisleUIPosition(loc_num);

		//복도에 배치되는 오브젝트들
		if (object_type == OT_KEYCARD)
		{
			shared_ptr<GameObject> sphere = make_shared<GameObject>();
			sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
			sphere->AddComponent(make_shared<Transform>());
			sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.02, WINDOW_WIDTH * 0.02, 500.f));
			sphere->GetTransform()->SetLocalPosition(aislePos);
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
				meshRenderer->SetMesh(mesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
				shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"KeyCard", L"..\\Resources\\Texture\\PR_Keycard_MapICON.png");
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				material->SetTexture(0, texture);
				meshRenderer->SetMaterial(material);
			}
			sphere->AddComponent(meshRenderer);

			sphere->GetTransform()->SetObjectType(OT_UI_MAPOBJECTS);
			sphere->GetTransform()->SetObjectID(666);

			mainGameScene->AddGameObject(sphere);
		}
		else if (object_type == OT_TERMINAL)
		{
			shared_ptr<GameObject> sphere = make_shared<GameObject>();
			sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
			sphere->AddComponent(make_shared<Transform>());
			sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.02, WINDOW_WIDTH * 0.02, 500.f));
			sphere->GetTransform()->SetLocalPosition(aislePos);
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
				meshRenderer->SetMesh(mesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
				shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Console", L"..\\Resources\\Texture\\PR_Console_MapICON.png");
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				material->SetTexture(0, texture);
				meshRenderer->SetMaterial(material);
			}
			sphere->AddComponent(meshRenderer);

			sphere->GetTransform()->SetObjectType(OT_UI_MAPOBJECTS);
			sphere->GetTransform()->SetObjectID(666);

			mainGameScene->AddGameObject(sphere);
		}
	}
	else if (loc_type == OT_ROOM)
	{
		Vec3 mapPos = CalculateMapUIPosition(loc_num);
		//맵에 배치되는 오브젝트들
		if (object_type == OT_RESURRECTION_PAD)
		{
			shared_ptr<GameObject> sphere = make_shared<GameObject>();
			sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
			sphere->AddComponent(make_shared<Transform>());
			sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.04, WINDOW_WIDTH * 0.04, 500.f));
			sphere->GetTransform()->SetLocalPosition(mapPos);
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
				meshRenderer->SetMesh(mesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
				shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"RevivePad", L"..\\Resources\\Texture\\PR_RevivePad_MapICON.png");
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				material->SetTexture(0, texture);
				meshRenderer->SetMaterial(material);
			}
			sphere->AddComponent(meshRenderer);

			sphere->GetTransform()->SetObjectType(OT_UI_MAPOBJECTS);
			sphere->GetTransform()->SetObjectID(666);

			mainGameScene->AddGameObject(sphere);
		}
		else if (object_type == OT_RABBITFOOT)
		{
			shared_ptr<GameObject> sphere = make_shared<GameObject>();
			sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
			sphere->AddComponent(make_shared<Transform>());
			sphere->GetTransform()->SetLocalScale(Vec3(WINDOW_WIDTH * 0.04, WINDOW_WIDTH * 0.04, 500.f));
			sphere->GetTransform()->SetLocalPosition(mapPos);
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
				meshRenderer->SetMesh(mesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
				shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"RabbitFoot", L"..\\Resources\\Texture\\PR_RabbitFoot_MapICON.png");
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				material->SetTexture(0, texture);
				meshRenderer->SetMaterial(material);
			}
			sphere->AddComponent(meshRenderer);

			sphere->GetTransform()->SetObjectType(OT_UI_MAPOBJECTS);
			sphere->GetTransform()->SetObjectID(666);

			mainGameScene->AddGameObject(sphere);
		}
	}
}

void SceneManager::SetKeyCardPosition(int x, int y, int keyCardNum)
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_UI_KEYCARD)
			continue;

		if (gameObject->GetTransform()->GetObjectID() == keyCardNum)
		{
			Vec3 pos = gameObject->GetTransform()->GetLocalPosition();

			pos.x = x;
			pos.y = y;

			gameObject->GetTransform()->SetLocalPosition(pos);
		}
	}
}

shared_ptr<GameObject> SceneManager::CreateBoxObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ)
{
	shared_ptr<GameObject> cube = make_shared<GameObject>();
	cube->AddComponent(make_shared<Transform>());
	cube->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	cube->GetTransform()->SetObjectType(object_type);
	cube->GetTransform()->SetObjectID(object_id);
	cube->GetTransform()->SetLocalPosition(Vec3(x, y, z));
	cube->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));
	cube->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
	cube->SetCheckFrustum(false);
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(cubeMesh);
	}
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
		shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, texture);
		material->SetTexture(1, texture2);
		meshRenderer->SetMaterial(material);
	}
	cube->AddComponent(meshRenderer);

	MyGameObject obj;
	obj.m_ObjectType = object_type;
	obj.m_ObjectID = object_id;
	obj.m_ObjectLocation = Vec3(x, y, z);
	obj.m_animationID = animation_id;
	obj.m_Direction = Vec3(dirX, dirY, dirZ);

	vp_ObjectManager.push_back(obj);
	_otherPlayer.push_back(cube);

	return cube;
	//scene->AddGameObject(cube);
}

void SceneManager::CreatePlayerObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ)
{
	MyGameObject obj;
	obj.m_ObjectType = object_type;
	obj.m_ObjectID = object_id;
	obj.m_ObjectLocation = Vec3(x, y, z);
	obj.m_animationID = animation_id;
	obj.m_Direction = Vec3(dirX, dirY, dirZ);

	vp_ObjectManager.push_back(obj);

	//{
	//	//플레이어의 애니메이션 모델이 복합적으로 있는 모델을 불러오는 함수
	//	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadPlayerModel(L"..\\Resources\\FBX\\Player2\\Player_Walk.fbx");
	//	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetName(L"Player");
	//		gameObject->SetCheckFrustum(false);
	//		gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));
	//		gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	//		gameObject->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));
	//		gameObject->GetTransform()->SetObjectType(object_type);
	//		gameObject->GetTransform()->SetObjectID(object_id);
	//		gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);

	//		// 각 게임 오브젝트에 독립적인 머티리얼 설정
	//		for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
	//		{
	//			shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
	//			gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
	//		}

	//		//히트박스
	//		gameObject->AddComponent(make_shared<BoxCollider>());
	//		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(5.f, 40.f, 5.f));
	//		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(x, y + 40.f, z));
	//		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);

	//		_otherPlayer.push_back(gameObject);
	//		mainGameScene->AddGameObject(gameObject);

	//	}
	//}

	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBlueTeamModel(L"..\\Resources\\FBX\\PlayerBlue\\IDLE_2\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Blue_Player");
			gameObject->SetCheckFrustum(false);

			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_id);

			gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			//gameObject->AddComponent(make_shared<TestDragon>());
			gameObject->AddComponent(make_shared<OtherPlayerScript>());

			//히트박스
			gameObject->AddComponent(make_shared<BoxCollider>());
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(5.f, 40.f, 5.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(x, y + 40.f, z));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);

			_otherPlayer.push_back(gameObject);
			mainGameScene->AddGameObject(gameObject);
		}
	}
}

void SceneManager::CreateOtherPlayerGunObject(int GunType_type, int object_id)
{
	//PO소환
	//오브젝트 타입은 OT_OTHER_PLAYER_SUB으로
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_PO_GunModel(L"..\\Resources\\FBX\\PlayerGun_PO\\IDLE_2\\PO01_Player_Single_IShoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"PO");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetObjectType(OT_OTHER_PLAYER_SUB);
			gameObject->GetTransform()->SetObjectID(object_id);
			gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}

			_otherPlayer.push_back(gameObject);
			mainGameScene->AddGameObject(gameObject);
		}
	}

	if (GunType_type == GT_AR)
	{
		//오브젝트 타입은 OT_OTHER_PLAYER_MAIN으로

		//돌격소총
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_AR_GunModel(L"..\\Resources\\FBX\\PlayerGun_AR\\IDLE_2\\AR01_Player_Single_IShoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"AR");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_OTHER_PLAYER_MAIN);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}

				_otherPlayer.push_back(gameObject);
				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
	else if (GunType_type == GT_SG)
	{
		//샷건
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SG_GunModel(L"..\\Resources\\FBX\\PlayerGun_SG\\IDLE_2\\SG01_Player_Single_IShoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"SG");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_OTHER_PLAYER_MAIN);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}

				_otherPlayer.push_back(gameObject);
				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
	else if (GunType_type == GT_SM)
	{
		//기관단총
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SMG_GunModel(L"..\\Resources\\FBX\\PlayerGun_SM\\IDLE_2\\SMG01_Player_Single_IShoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"SMG");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_OTHER_PLAYER_MAIN);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalPosition(Vec3(1200.f, 40.f, 1200.f));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}
				
				_otherPlayer.push_back(gameObject);
				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
	else if (GunType_type == GT_SR)
	{
		//스나이퍼
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SR_GunModel(L"..\\Resources\\FBX\\PlayerGun_SR\\IDLE_2\\SR01_Player_Single_IShoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"SR");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_OTHER_PLAYER_MAIN);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalPosition(Vec3(1250.f, 40.f, 1200.f));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}
				_otherPlayer.push_back(gameObject);
				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
}

shared_ptr<GameObject> SceneManager::GetOtherPlayerMainGun(int id)
{
	vector<shared_ptr<GameObject>> gameObjects = _otherPlayer;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectID() == id)
		{
			if (gameObject->GetTransform()->GetObjectType() == OT_OTHER_PLAYER_MAIN)
			{
				return gameObject;
			}
		}
	}
}

shared_ptr<GameObject> SceneManager::GetOtherPlayerSubGun(int id)
{
	vector<shared_ptr<GameObject>> gameObjects = _otherPlayer;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectID() == id)
		{
			if (gameObject->GetTransform()->GetObjectType() == OT_OTHER_PLAYER_SUB)
			{
				return gameObject;
			}
		}
	}
}

void SceneManager::CreateHeadCoreObject(int object_id)
{
	vector<shared_ptr<GameObject>> gameObjects = _otherPlayer;

	Vec3 pos;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectID() == object_id)
		{
			pos = gameObject->GetTransform()->GetLocalPosition();
		}
	}

	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\HeadCore_Blue.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"HeadCore_Blue");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(pos);
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			gameObject->GetTransform()->SetObjectType(OT_HEADCORE);
			gameObject->GetTransform()->SetObjectID(object_id);

			//히트박스
			gameObject->AddComponent(make_shared<BoxCollider>());
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(meshData2->GetAABBExtents().x, meshData2->GetAABBExtents().y, meshData2->GetAABBExtents().z));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(pos);
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);

			_otherPlayer.push_back(gameObject);

			mainGameScene->AddGameObject(gameObject);
		}
	}
}

void SceneManager::RevivePlayerObject(int object_id)
{
	vector<shared_ptr<GameObject>> gameObjects = _otherPlayer;

	Vec3 pos;
	Vec3 dir;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectID() == object_id)
		{
			pos = gameObject->GetTransform()->GetLocalPosition();
			dir = gameObject->GetTransform()->GetLocalRotation();
		}
	}

	{
		//플레이어의 애니메이션 모델이 복합적으로 있는 모델을 불러오는 함수
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadPlayerModel(L"..\\Resources\\FBX\\Player2\\Player_Walk.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Player");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(pos);
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, dir.y - 3.14f, 0.f));
			gameObject->GetTransform()->SetObjectType(OT_PLAYER);
			gameObject->GetTransform()->SetObjectID(object_id);
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);

			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}

			//히트박스
			gameObject->AddComponent(make_shared<BoxCollider>());
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(5.f, 40.f, 5.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(pos.x, pos.y + 40.f, pos.z));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);

			_otherPlayer.push_back(gameObject);
			mainGameScene->AddGameObject(gameObject);

		}
	}
}

void SceneManager::CreatePlayerHandObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ)
{
	//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Player6\\dddd.fbx");

	//vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	//for (auto& gameObject : gameObjects)
	//{
	//	gameObject->SetName(L"Me");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->GetTransform()->SetObjectType(object_type);
	//	gameObject->GetTransform()->SetObjectID(object_id);
	//	gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));		//0.f, 45.f, 100.f
	//	gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
	//	gameObject->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));



	//	gameObject->AddComponent(make_shared<BoxCollider>());
	//	std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(5.f, 20.f, 5.f));
	//	std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(x, y + 40.f, z));
	//	std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);
	//	mainGameScene->AddGameObject(gameObject);
	//}

	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBlueHandModel(L"..\\Resources\\FBX\\Player_Hand_Blue\\IDLE\\Blue_FP_Shoot.fbx");
	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	for (auto& gameObject : gameObjects)
	{
		gameObject->SetName(L"Me");
		gameObject->SetCheckFrustum(false);
		gameObject->GetTransform()->SetObjectType(object_type);
		gameObject->GetTransform()->SetObjectID(object_id);
		gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));
		//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
		gameObject->GetTransform()->SetLocalScale(Vec3(0.03f, 0.03f, 0.03f));
		gameObject->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));
		gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
		// 각 게임 오브젝트에 독립적인 머티리얼 설정
		for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
		{
			shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
			gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
		}

		gameObject->AddComponent(make_shared<BoxCollider>());
		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(5.f, 20.f, 5.f));
		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(x, y + 40.f, z));
		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);
		mainGameScene->AddGameObject(gameObject);
	}

}

void SceneManager::CreatePlayerGunObject(int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ)
{

	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_PO_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_PO\\IDLE\\P01_FP_Shoot.fbx");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Me_Gun_Sub");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetObjectType(OT_UI_SUB_WEAPON_3D);
			gameObject->GetTransform()->SetObjectID(object_id);
			gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));
			//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}
			mainGameScene->AddGameObject(gameObject);
		}
	}

	//0이면 기관단총	(GT_SM				0)
	//1이면 산탄총		(GT_SG				1)
	//2이면 돌격소총	(GT_AR				2)
	//3이면 저격소총	(GT_SR				3)
	if (GetMainWeapon_type() == 0)
	{
		//AR_FPS
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_AR_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_AR\\IDLE\\AR01_FP_Shoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Me_Gun_Main");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_UI_MAIN_WEAPON_3D);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}
				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
	else if (GetMainWeapon_type() == 1)
	{
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SG_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_SG\\IDLE\\SG01_FP_Shoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Me_Gun_Main");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_UI_MAIN_WEAPON_3D);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}
				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
	else if (GetMainWeapon_type() == 2)
	{
		//SMG_FPS
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SMG_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_SMG\\IDLE\\SMG01_FP_Shoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Me_Gun_Main");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_UI_MAIN_WEAPON_3D);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}
				gameObject->AddComponent(make_shared<TestDragon>());
				mainGameScene->AddGameObject(gameObject);
			}
		}

	}
	else if (GetMainWeapon_type() == 3)
	{

		//SR_FPS
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_SR_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_SR\\IDLE\\SR01_FP_Shoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Me_Gun_Main");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_UI_MAIN_WEAPON_3D);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}
				gameObject->AddComponent(make_shared<TestDragon>());
				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
	else
	{
		//아무것도 고르지 않았다면 디폴트로 AR을 소환하도록 한다.
		SetMainWeapon_Type(0);
		//AR_FPS
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->Load_AR_FPS(L"..\\Resources\\FBX\\Player_Hand_GUN_AR\\IDLE\\AR01_FP_Shoot.fbx");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Me_Gun_Main");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetObjectType(OT_UI_MAIN_WEAPON_3D);
				gameObject->GetTransform()->SetObjectID(object_id);
				gameObject->GetTransform()->SetLocalPosition(Vec3(OUT_OF_RENDER, OUT_OF_RENDER, OUT_OF_RENDER));
				//gameObject->GetTransform()->SetLocalScale(Vec3(0.13f, 0.13f, 0.13f));
				gameObject->GetTransform()->SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
				gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				// 각 게임 오브젝트에 독립적인 머티리얼 설정
				for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
				{
					shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
					gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
				}
				mainGameScene->AddGameObject(gameObject);
			}
		}
	}

}

void SceneManager::CreatePlayerHeadCoreObject(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ)
{

	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\HeadCore_Blue.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"HeadCore_Blue");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_id);
			gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));

			gameObject->AddComponent(make_shared<BoxCollider>());
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(5.f, 20.f, 5.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(x, y, z));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);

			mainGameScene->AddGameObject(gameObject);
		}
	}
}

void SceneManager::ChangeObjectMovement(int object_id, float x, float y, float z, float dirX, float dirY, float dirZ, int animationID)
{
	for (auto& otherPlayer : _otherPlayer)
	{
		if (otherPlayer->GetTransform()->GetObjectID() == object_id)
		{
			if (otherPlayer->GetTransform()->GetObjectType() != OT_OTHER_PLAYER_MAIN
				&& otherPlayer->GetTransform()->GetObjectType() != OT_OTHER_PLAYER_SUB)
			{

				otherPlayer->GetTransform()->SetLocalPosition(Vec3(x, y, z));
				otherPlayer->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));
			}
		}
	}
}

void SceneManager::ChangeObjectAnimation(int object_id, int animationID)
{

#pragma region New_Animation_State
	
	if (animationID != -1)
	{
		for (auto& otherPlayer : _otherPlayer)
		{
			if (otherPlayer->GetTransform()->GetObjectID() == object_id)
			{
				if (otherPlayer->GetTransform()->GetObjectType() != OT_HEADCORE
					&& otherPlayer->GetTransform()->GetObjectType() != OT_OTHER_PLAYER_MAIN
					&& otherPlayer->GetTransform()->GetObjectType() != OT_OTHER_PLAYER_SUB)
				{
					if (animationID == AT_IDLE)
					{
						
						otherPlayer->GetAnimator()->Play(0);
						//otherPlayer->GetAnimator()->AddToSequence(1);
					}
					if (animationID == AT_WALKING)
					{
						
						otherPlayer->GetAnimator()->Play(5);
						//otherPlayer->GetAnimator()->AddToSequence(6);
					}
					if (animationID == AT_RUNNING)
					{
						
						otherPlayer->GetAnimator()->Play(10);
						//otherPlayer->GetAnimator()->AddToSequence(11);
					}
					if (animationID == AT_CHANGE)
					{
						uint32 state = otherPlayer->GetAnimator()->GetLastAddedAnimationType();
						
						if (state == AT_IDLE)
						{
							/*otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(2);
							otherPlayer->GetAnimator()->AddToSequence(0);*/
						}
						else if (state == AT_WALKING)
						{
							/*otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(2);
							otherPlayer->GetAnimator()->AddToSequence(0);*/
						}
						else if(state == AT_RUNNING)
						{
							/*otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(2);
							otherPlayer->GetAnimator()->AddToSequence(0);*/
						}
					}
					if (animationID == AT_RELOADING)
					{
						uint32 state = otherPlayer->GetAnimator()->GetLastAddedAnimationType();

						if (state == AT_IDLE)
						{
							otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(3);
							otherPlayer->GetAnimator()->AddToSequence(0);
						}
						else if (state == AT_WALKING)
						{
							otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(8);
							otherPlayer->GetAnimator()->AddToSequence(5);
						}
						else if (state == AT_RUNNING)
						{
							otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(13);
							otherPlayer->GetAnimator()->AddToSequence(10);
						}
					}
					if (animationID == AT_SHOOTING)
					{
						uint32 state = otherPlayer->GetAnimator()->GetLastAddedAnimationType();

						if (state == AT_IDLE)
						{
							/*otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(2);
							otherPlayer->GetAnimator()->AddToSequence(0);*/
						}
						else if (state == AT_WALKING)
						{
							/*otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(2);
							otherPlayer->GetAnimator()->AddToSequence(0);*/
						}
						else if (state == AT_RUNNING)
						{
							/*otherPlayer->GetAnimator()->ClearSequence();
							otherPlayer->GetAnimator()->AddToSequence(2);
							otherPlayer->GetAnimator()->AddToSequence(0);*/
						}
					}
				}
			}
		}
	}
#pragma endregion
}

void SceneManager::CreateAisle(float aisleX, float aisleY, float aisleZ, float aisleScale, int type, int ID)
{
#pragma region Aisle
	Vec3 aislePosition = Vec3(aisleX, aisleY, aisleZ);	// 0, 0, 210
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleCeiling.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleCeiling.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleCeiling");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleCeilingPipe.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleCeilingPipe.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleCeilingPipe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleFloor.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleFloor001.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleFloor");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			gameObject->GetTransform()->SetObjectType(type);
			gameObject->GetTransform()->SetObjectID(ID);
			mainGameScene->AddGameObject(gameObject);
		}
	}

	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AislePipe.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AislePipe.bin");


		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AislePipe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}

	float AisleWallPos[2][3] = {
		{40.f + aisleX, 40.f, 0.f + aisleZ},
		{-40.f + aisleX, 40.f, 0.f + aisleZ}
	};
	float AisleWallScale[2][3] = {
		{15, 85, 253},
		{15, 85, 253},
	};

	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleWall.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleWall.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleWall");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));

			gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
			gameObject->GetTransform()->SetObjectID(ID);

			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(AisleWallScale[0]) * 0.5f);
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(AisleWallPos[0]));

			mainGameScene->AddGameObject(gameObject);
		}
	}

	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AislePipe.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AislePipe.bin");


		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AislePipe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 3.14f, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleWall.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleWall.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleWall");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 3.14f, 0.f));

			gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
			gameObject->GetTransform()->SetObjectID(ID);

			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(AisleWallScale[1]) * 0.5f);
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(AisleWallPos[1]));

			mainGameScene->AddGameObject(gameObject);
		}
	}
	
	
#pragma endregion


}

void SceneManager::CreateAisle2(float aisleX, float aisleY, float aisleZ, float aisleScale, int type, int ID)
{
#pragma region Aisle
	Vec3 aislePosition = Vec3(aisleX, aisleY, aisleZ);	// 0, 0, 210
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleCeiling.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleCeiling.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleCeiling");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 1.57, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleCeilingPipe.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleCeilingPipe.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleCeilingPipe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 1.57, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleFloor.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleFloor001.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleFloor");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 1.57, 0.f));
			gameObject->GetTransform()->SetObjectType(type);
			gameObject->GetTransform()->SetObjectID(ID);
			mainGameScene->AddGameObject(gameObject);
		}
	}

	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AislePipe.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AislePipe.bin");


		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AislePipe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 1.57, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}

	float AisleWallPos[2][3] = {
		{0.f + aisleX, 40.f, -40.f + aisleZ},
		{0.f + aisleX, 40.f, 40.f + aisleZ}
	};
	float AisleWallScale[2][3] = {
		{253, 85, 15},
		{253, 85, 15},
	};


	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleWall.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleWall.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleWall");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 1.57, 0.f));

			gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
			gameObject->GetTransform()->SetObjectID(ID);

			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(AisleWallScale[0]) * 0.5f);
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(AisleWallPos[0]));
			mainGameScene->AddGameObject(gameObject);
		}
	}

	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AislePipe.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AislePipe.bin");


		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AislePipe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 3.14f + 1.57, 0.f));
			mainGameScene->AddGameObject(gameObject);
		}
	}
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleWall.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AisleWall.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleWall");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 3.14f + 1.57, 0.f));

			gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
			gameObject->GetTransform()->SetObjectID(ID);

			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(AisleWallScale[1]) * 0.5f);
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(AisleWallPos[1]));
			mainGameScene->AddGameObject(gameObject);
		}
	}




#pragma endregion

}

Vec3 SceneManager::FindAislePosition(int aisleNum)
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& gameObject : gameObjects) 
	{
		if (gameObject->GetTransform()->GetObjectType() == OT_CORRIDOR)
		{
			if (gameObject->GetTransform()->GetObjectID() == aisleNum)
			{
				return gameObject->GetTransform()->GetLocalPosition();
			}
		}
	}
}

Vec3 SceneManager::FindRoomPosition(int roomNum)
{
	//방 위치 확인 코드
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() == OT_ROOM)
		{
			if (gameObject->GetTransform()->GetObjectID() == roomNum)
			{
				return gameObject->GetTransform()->GetLocalPosition();
			}
		}
	}
}

void SceneManager::CreateGameObject(int aisleNum, int object_type, int object_ID)
{
	Vec3 aislePos = FindAislePosition(aisleNum);
	Vec3 roomPos = FindRoomPosition(aisleNum);

	if (object_type == OT_KEYCARD)
	{
		aislePos.y += 20.f;

		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\CardKey.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"CardKey");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePos);
			gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_ID);


			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성

			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(30.f, 30.f, 30.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(aislePos);

			//gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			mainGameScene->AddGameObject(gameObject);
		}
	}
	else if (object_type == OT_TERMINAL)
	{
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Console.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"Console");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePos);
			gameObject->GetTransform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_ID);


			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
			
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(30.f, 30.f, 30.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(aislePos);

			//gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			mainGameScene->AddGameObject(gameObject);
		}
	}
	else if (object_type == OT_RABBITFOOT)//토끼발 추가 코드
	{

		roomPos.y += 5.f;
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Object.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();


		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"RabbitFoot");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(roomPos);
			gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_ID);


			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성

			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(30.f, 30.f, 30.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(roomPos);

			//gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			mainGameScene->AddGameObject(gameObject);
		}
	}
	else if (object_type == OT_RESURRECTION_PAD)//부활패드 추가
	{

		roomPos.y += 5.f;
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\RevivePad.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();


		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"RevivalPad");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(roomPos);
			gameObject->GetTransform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_ID);


			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성

			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(30.f, 30.f, 30.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(roomPos);

			//gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			mainGameScene->AddGameObject(gameObject);
		}
	}
	else if (object_type == OT_EXIT)
	{
		roomPos.y += 5.f;

		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Exit.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"Exit");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(roomPos));
			gameObject->GetTransform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));

			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_ID);

			mainGameScene->AddGameObject(gameObject);
		}
	}
	else if(object_type == OT_AMMOBOX)
	{
		roomPos.y += 5.f;

		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\AmmoBox.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"AmmoBox");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(roomPos);
			gameObject->GetTransform()->SetLocalScale(Vec3(5.f, 5.f, 5.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			
			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_ID);

			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성

			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(30.f, 30.f, 30.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(roomPos);

			mainGameScene->AddGameObject(gameObject);
		}
	}
	else if (object_type == OT_MEDIKIT)
	{
		roomPos.y += 7.f;

		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Medkit.bin");

		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"Medkit");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(roomPos);
			gameObject->GetTransform()->SetLocalScale(Vec3(3.f, 3.f, 3.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			
			gameObject->GetTransform()->SetObjectType(object_type);
			gameObject->GetTransform()->SetObjectID(object_ID);

			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성

			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(30.f, 30.f, 30.f));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(roomPos);

			mainGameScene->AddGameObject(gameObject);
		}
	}
}

void SceneManager::CreateMap(float mapX, float mapY, float mapZ, float aisleScale, int type, int ID)
{

#pragma region Map
	Vec3 mapPosition = Vec3(mapX, mapY, mapZ); //0, 0, 0

	//FLOOR------------------------------------
	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Floor\\Floor.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Floor.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		shared_ptr<Material> material;

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Floor");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(mapPosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, 0.f, 0.f));
			gameObject->GetTransform()->SetObjectType(type);
			gameObject->GetTransform()->SetObjectID(ID);

			mainGameScene->AddGameObject(gameObject);
		}
	}

	{
		//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Floor\\Floor.fbx");
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Floor.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		shared_ptr<Material> material;

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Floor");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(mapPosition.x, mapPosition.y + 375, mapPosition.z));
			gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
			gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f - 3.14f, 0.f, 0.f));

			mainGameScene->AddGameObject(gameObject);
		}
	}

	//GATE------------------------------------
	float Gate001Pos[4][3] = {
		{-165 + mapX, 35, 55 + mapZ},
		{55 + mapX, 35, 165 + mapZ},
		{165 + mapX, 35, -55 + mapZ},
		{-55 + mapX, 35, -165 + mapZ}
	};
	float Gate001Scale[4][3] = {
		{22,75,20},
		{20,75,22},
		{22,75,20},
		{20,75,22},
	};

	float Gate001_2Pos[4][3] =
	{
		{-165 + mapX, 35, -55 + mapZ},
		{-55 + mapX, 35, 165 + mapZ},
		{165 + mapX, 35, 55 + mapZ},
		{55 + mapX, 35, -165 + mapZ}
	};


	for (int i = 0; i < 4; i++)
	{

		{
			//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Gate\\Gate001.fbx");
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Gate001.bin");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Gate001");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
				gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, i * 1.57f, 0.f));

				gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
				gameObject->GetTransform()->SetObjectID(ID);

				gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(Gate001Scale[i]) * 0.5f);
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(Gate001Pos[i]));

				mainGameScene->AddGameObject(gameObject);
			}
		}
		{
			//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Gate\\Gate001-2.fbx");
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Gate001-2.bin");
			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Gate001-2");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
				gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, i * 1.57f, 0.f));

				gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
				gameObject->GetTransform()->SetObjectID(ID);

				gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(Gate001Scale[i]) * 0.5f);
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(Gate001_2Pos[i]));


				mainGameScene->AddGameObject(gameObject);
			}
		}
		{
			//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Gate\\GCeiling001.fbx");
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\GCeiling001.bin");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"GCeiling001");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
				gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, i * 1.57f, 0.f));


				mainGameScene->AddGameObject(gameObject);
			}
		}
	}

	float RoomPipePos[4][3] = {
		{165 + mapX, 35, 165 + mapZ},
		{165 + mapX, 35, -165 + mapZ},
		{-165 + mapX, 35, -165 + mapZ},
		{-165 + mapX, 35, 165 + mapZ}
	};
	float RoomPipeScale[4][3] = {
		{110,75,110},
		{110,75,110},
		{110,75,110},
		{110,75,110},
	};


	//ROOMPIPE----------------------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		{
			//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\RoomPipe\\RoomPipe.fbx");
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\RoomPipe.bin");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"RoomPipe");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
				gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, i * 1.57f, 0.f));

				gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
				gameObject->GetTransform()->SetObjectID(ID);

				gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(RoomPipeScale[i]) * 0.5f);
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(RoomPipePos[i]));


				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
	
	float Wall001Pos[4][3] = {
		{-88 + mapX, 35, 185 + mapZ},
		{185 + mapX, 35, 88 + mapZ},
		{88 + mapX, 35, -185 + mapZ},
		{-185 + mapX, 35, -88 + mapZ}
	};
	float Wall001Scale[4][3] = {
		{45, 75, 15},
		{15, 75, 45},
		{45, 75, 15},
		{15, 75, 45},
	};
	float Wall003Pos[4][3] = {
		{88 + mapX, 35, 185 + mapZ},
		{185 + mapX, 35, -88 + mapZ},
		{-88 + mapX, 35, -185 + mapZ},
		{-185 + mapX, 35, 88 + mapZ}
	};
	//WALL----------------------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		{
			//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Wall\\Wall001.fbx");
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Wall001.bin");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Wall001");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
				gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, i * 1.57f, 0.f));

				gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
				gameObject->GetTransform()->SetObjectID(ID);

				gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(Wall001Scale[i]) * 0.5f);
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(Wall001Pos[i]));

				mainGameScene->AddGameObject(gameObject);
			}
		}
		{
			//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Wall\\Wall002.fbx");
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Wall001-2.bin");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Wall002");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
				gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, i * 1.57f, 0.f));


				mainGameScene->AddGameObject(gameObject);
			}
		}
		{
			//shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Wall\\Wall003.fbx");
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Wall001-3.bin");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Wall003");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
				gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57f, i * 1.57f, 0.f));

				gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
				gameObject->GetTransform()->SetObjectID(ID);

				gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(Wall001Scale[i]) * 0.5f);
				std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(Wall003Pos[i]));

				mainGameScene->AddGameObject(gameObject);
			}
		}
	}
#pragma endregion

}

void SceneManager::CreateOutDoor(float mapX, float mapY, float mapZ, float aisleScale, int ID)
{
	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Door001.bin");

	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();


	for (auto& gameObject : gameObjects)
	{
		gameObject->SetName(L"Door001");
		gameObject->SetCheckFrustum(false);
		gameObject->GetTransform()->SetLocalPosition(Vec3(mapX, mapY, mapZ));
		gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
		gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57, 0.f, 0.f));

		gameObject->GetTransform()->SetObjectType(OT_WALLAABB);
		gameObject->GetTransform()->SetObjectID(ID);

		gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(40, 75, 110) * 0.5);
		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(mapX, 40.f, mapZ));

		mainGameScene->AddGameObject(gameObject);
	}
}

void SceneManager::CreateOutDoor2(float mapX, float mapY, float mapZ, float aisleScale, int ID)
{
	shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Door001.bin");

	vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

	for (auto& gameObject : gameObjects2)
	{
		gameObject->SetName(L"Door001");
		gameObject->SetCheckFrustum(false);
		gameObject->GetTransform()->SetLocalPosition(Vec3(mapX, mapY, mapZ));
		gameObject->GetTransform()->SetLocalScale(Vec3(aisleScale, aisleScale, aisleScale));
		gameObject->GetTransform()->SetLocalRotation(Vec3(-1.57, 1.57f, 0.f));

		gameObject->GetTransform()->SetObjectType(99);
		gameObject->GetTransform()->SetObjectID(ID);

		gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(110, 75, 40) * 0.5);
		std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(mapX, 40.f, mapZ));

		//gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
		mainGameScene->AddGameObject(gameObject);
	}
}

void SceneManager::CreateAABBBox(Vec3 aabbPosition, Vec3 aabbScale)
{
	//내가 직접 설치하는 바운딩 박스
	shared_ptr<GameObject> cube = make_shared<GameObject>();
	cube->AddComponent(make_shared<Transform>());
	cube->GetTransform()->SetLocalPosition(aabbPosition);
	cube->GetTransform()->SetLocalScale(aabbScale);
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(cubeMesh);
	}
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"AABB");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		meshRenderer->SetMaterial(material);
	}
	cube->AddComponent(meshRenderer);
	cube->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
	std::dynamic_pointer_cast<BoxCollider>(cube->GetCollider())->SetExtents(aabbScale * .5f);
	std::dynamic_pointer_cast<BoxCollider>(cube->GetCollider())->SetCenter(aabbPosition);
	std::dynamic_pointer_cast<BoxCollider>(cube->GetCollider())->SetStatic(false);

	mainGameScene->AddGameObject(cube);
}

int SceneManager::RenderAABBBox(Vec3 aabbPosition, Vec3 aabbScale)
{
	shared_ptr<GameObject> cube = make_shared<GameObject>();
	cube->AddComponent(make_shared<Transform>());
	cube->GetTransform()->SetLocalPosition(aabbPosition);
	cube->GetTransform()->SetLocalScale(aabbScale);
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	{
		shared_ptr<Mesh> cubeMesh = GET_SINGLE(Resources)->LoadCubeMesh();
		meshRenderer->SetMesh(cubeMesh);
	}
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"AABB");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		meshRenderer->SetMaterial(material);
	}
	cube->AddComponent(meshRenderer);
	cube->GetTransform()->SetObjectType(OT_WALLAABB);		//바운딩 박스의 오브젝트타입 번호는99이다
	cube->GetTransform()->SetObjectID(boxNum);
	boxNum++;
	mainGameScene->AddGameObject(cube);

	return boxNum - 1;
}

void SceneManager::UpdateAABBBox(int boxNum, Vec3 pos, Vec3 scale, Vec3 rotation)
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_WALLAABB)
			continue;

		if (gameObject->GetTransform()->GetObjectID() != boxNum)
			continue;

		if (gameObject->GetTransform()->GetObjectID() == boxNum)
		{
			if(gameObject->GetTransform()->GetLocalScale() != scale)
				gameObject->GetTransform()->SetLocalScale(scale);

			if (gameObject->GetTransform()->GetWorldPosition() != pos)
				gameObject->GetTransform()->SetLocalPosition(pos);

			//if (gameObject->GetTransform()->GetLocalRotation() != rotation)
			//	gameObject->GetTransform()->SetLocalRotation(rotation);
		}
	}

}

void SceneManager::AddComputeShader(int threadX, int threadY, int threadZ)
{
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		// UAV 용 Texture 생성
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVHandle(), UAV_REGISTER::u0);

		// 쓰레드 그룹 (1 * 1024 * 1)
		material->Dispatch(threadX, threadY, threadZ);
	}
}

void SceneManager::CalculateHP(int damagedHP)
{
	int tensPlaceBeforeCalculate = (playerHP / 10) % 10;

	int tensPlaceAfterCalculate = (damagedHP / 10) % 10;
	playerHP = damagedHP;

	// 체력 감소
	if (tensPlaceBeforeCalculate > tensPlaceAfterCalculate)
	{
		int HPnum = tensPlaceAfterCalculate;
		auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
		for (int i = HPnum + 1; i < 11; i++)
		{
			for (auto& gameObject : gameObjects)
			{
				if (gameObject->GetTransform()->GetObjectType() != OT_UI_HP)
					continue;
				if (gameObject->GetTransform()->GetObjectID() != i)
					continue;
				if (gameObject->GetTransform()->GetObjectID() == i)
				{
					if (gameObject->GetTransform()->GetLocalPosition().x == OUT_OF_RENDER)
						continue;
					Vec3 pos = gameObject->GetTransform()->GetLocalPosition();

					pos.x = OUT_OF_RENDER;
					pos.y = OUT_OF_RENDER;

					gameObject->GetTransform()->SetLocalPosition(pos);
				}
			}
		}
	}//체력 증가
	else if (tensPlaceBeforeCalculate < tensPlaceAfterCalculate)
	{
		int HPnum = tensPlaceAfterCalculate / 10;
		auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

		for (int i = 1; i < HPnum + 1; i++)
		{
			for (auto& gameObject : gameObjects)
			{
				if (gameObject->GetTransform()->GetObjectType() != OT_UI_HP)
					continue;
				if (gameObject->GetTransform()->GetObjectID() != i)
					continue;
				if (gameObject->GetTransform()->GetObjectID() == i)
				{
					if (gameObject->GetTransform()->GetLocalPosition().x != OUT_OF_RENDER)
						continue;
					Vec3 pos = gameObject->GetTransform()->GetLocalPosition();
					pos.x = -(WINDOW_WIDTH / 2) + 510 + 65 * i;
					pos.y = (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100));
					gameObject->GetTransform()->SetLocalPosition(pos);
				}
			}
		}
	}
}

void SceneManager::CalculateBullet(int nowBullet)
{
	//이전 총알의 십의자리수
	int tensPlaceBeforeFire = (bullet / 10) % 10;
	//이전 총알의 일의자리수
	int onesPlaceBeforeBullet = bullet - tensPlaceBeforeFire * 10;

	//현재 총알의 십의자리수
	int tensPlaceAfterFire = (nowBullet / 10) % 10;
	//현재 총알의 일의자리수
	int onesPlaceAfterBullet = nowBullet - tensPlaceAfterFire * 10;

	bullet = nowBullet;

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_UI_CURRENTBULLET)
			continue;
		if (gameObject->GetTransform()->GetObjectID() == onesPlaceBeforeBullet)
		{
			Vec3 pos = gameObject->GetTransform()->GetLocalPosition();
			pos.x = OUT_OF_RENDER;
			pos.y = OUT_OF_RENDER;
			gameObject->GetTransform()->SetLocalPosition(pos);
		}
		if (gameObject->GetTransform()->GetObjectID() == onesPlaceAfterBullet)
		{
			Vec3 pos = gameObject->GetTransform()->GetLocalPosition();
			pos.x = -40;
			pos.y = 10 - (WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100));
			gameObject->GetTransform()->SetLocalPosition(pos);
		}
	}
	//십의 자리가 변경되었다면
	if (tensPlaceBeforeFire != tensPlaceAfterFire)
	{
		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetTransform()->GetObjectType() != OT_UI_CURRENTBULLET)
				continue;
			if (gameObject->GetTransform()->GetObjectID() == tensPlaceBeforeFire + 10)
			{
				Vec3 pos = gameObject->GetTransform()->GetLocalPosition();
				pos.x = OUT_OF_RENDER;
				pos.y = OUT_OF_RENDER;
				gameObject->GetTransform()->SetLocalPosition(pos);
			}
			if (gameObject->GetTransform()->GetObjectID() == tensPlaceAfterFire + 10)
			{
				Vec3 pos = gameObject->GetTransform()->GetLocalPosition();
				pos.x = -90;
				pos.y = 10 - (WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100));
				gameObject->GetTransform()->SetLocalPosition(pos);
			}
		}
	}

	
}

void SceneManager::SetBullet(int BulletCount)
{
	bullet = BulletCount;


	//현재 총알의 십의자리수
	int tensPlaceBulletCount = (BulletCount / 10) % 10;
	//현재 총알의 일의자리수
	int onesPlaceBulletCount = BulletCount - tensPlaceBulletCount * 10;

	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_UI_CURRENTBULLET)
			continue;
		if (gameObject->GetTransform()->GetObjectID() == onesPlaceBulletCount)
		{
			Vec3 pos = gameObject->GetTransform()->GetLocalPosition();
			pos.x = -40;
			pos.y = 10 - (WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100));
			gameObject->GetTransform()->SetLocalPosition(pos);
		}
		if (gameObject->GetTransform()->GetObjectID() == tensPlaceBulletCount + 10)
		{
			Vec3 pos = gameObject->GetTransform()->GetLocalPosition();
			pos.x = -90;
			pos.y = 10 - (WINDOW_HEIGHT / 2) + (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100));
			gameObject->GetTransform()->SetLocalPosition(pos);
		}
	}
}

void SceneManager::SetRabbitFootUI()
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() != OT_UI_RABBITFOOT)
			continue;
		if (gameObject->GetTransform()->GetObjectID() == 1)
		{
			Vec3 pos = gameObject->GetTransform()->GetLocalPosition();
			pos.x = -150;
			pos.y = (WINDOW_HEIGHT / 2) - (WINDOW_HEIGHT / (WINDOW_HEIGHT / 100)) - 130;//이곳에 좌표 입력
			gameObject->GetTransform()->SetLocalPosition(pos);
		}
	}
}

void SceneManager::SetPlayerLocation(float x, float y, float z, float dirx, float diry, float dirz)
{
	auto& gameObjects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetTransform()->GetObjectType() == OT_PLAYER_CAMERA)
		{
			gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));
			gameObject->GetTransform()->SetLocalRotation(Vec3(dirx, diry, dirz));
		}
	}
}

void SceneManager::CreateMovingObject(float x, float y, float z, float dirx, float diry, float dirz, int obj_id, int obj_type)
{
	float obj_Size = 50.f;

	if(obj_type == OT_GRINDER)
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Crusher_No_Blade.bin");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Crusher");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));
			gameObject->GetTransform()->SetLocalScale(Vec3(obj_Size, obj_Size, obj_Size));
			gameObject->GetTransform()->SetLocalRotation(Vec3(dirx, diry, dirz));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);

			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}

			gameObject->GetTransform()->SetObjectType(OT_GRINDER);
			gameObject->GetTransform()->SetObjectID(obj_id);

			gameObject->AddComponent(make_shared<CrusherScript>());

			_otherPlayer.push_back(gameObject);


			mainGameScene->AddGameObject(gameObject);
		}
	}
	else if (obj_type == OT_LASER)
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Laser.bin");
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Laser");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));
			gameObject->GetTransform()->SetLocalScale(Vec3(obj_Size, obj_Size, obj_Size));
			gameObject->GetTransform()->SetLocalRotation(Vec3(dirx, diry, dirz));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);

			// 각 게임 오브젝트에 독립적인 머티리얼 설정
			for (uint32 i = 0; i < gameObject->GetMeshRenderer()->GetMaterialCount(); i++)
			{
				shared_ptr<Material> clonedMaterial = gameObject->GetMeshRenderer()->GetMaterial(i)->Clone();
				gameObject->GetMeshRenderer()->SetMaterial(clonedMaterial, i);
			}

			gameObject->AddComponent(make_shared<BoxCollider>());	// 바운딩 박스 생성
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetExtents(Vec3(50.f, (meshData->GetAABBExtents().y * obj_Size), (meshData->GetAABBExtents().z * obj_Size)));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetCenter(Vec3(x, y, z));
			std::dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider())->SetStatic(false);

			gameObject->GetTransform()->SetObjectType(OT_LASER);
			gameObject->GetTransform()->SetObjectID(obj_id);

			_otherPlayer.push_back(gameObject);


			mainGameScene->AddGameObject(gameObject);
		}
	}
}
