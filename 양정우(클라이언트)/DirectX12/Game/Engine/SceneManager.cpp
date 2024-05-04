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
#include "Resources.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "SphereCollider.h"
#include "MeshData.h"
#include "TestDragon.h"

#include "ObjectManager.h"

shared_ptr<Scene> scene = std::make_shared<Scene>();

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

void SceneManager::LoadScene(std::wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	_activeScene = LoadTestScene();

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

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetCollider() == nullptr)
			continue;

		// ViewSpace에서의 Ray 정의
		Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

		// WorldSpace에서의 Ray 정의
		rayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
		rayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);
		rayDir.Normalize();

		// WorldSpace에서 연산
		float distance = 0.f;
		if (gameObject->GetCollider()->Intersects(rayOrigin, rayDir, OUT distance) == false)
			continue;

		//우클릭시 HIT출력
		if (distance < minDistance)
		{
			minDistance = distance;
			picked = gameObject;
		}
	}

	return picked;
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

		scene->AddGameObject(camera);

		

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
		scene->AddGameObject(camera);
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

			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion

#pragma region Sphere(Object)
	//{
	//	shared_ptr<GameObject> obj = make_shared<GameObject>();
	//	obj->SetName(L"OBJ");
	//	obj->AddComponent(make_shared<Transform>());
	//	obj->AddComponent(make_shared<SphereCollider>()); // 이것을 추가함으로서 픽킹의 적용을 받는다.
	//	obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	//	obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 500.f));
	//	obj->SetStatic(false);	// false로 하여 그림자의 적용을 받는다
	//	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	//	{
	//		shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
	//		meshRenderer->SetMesh(sphereMesh);
	//	}
	//	{
	//		/*shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
	//		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Leather", L"..\\Resources\\Texture\\Leather.jpg");
	//		shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Texture\\Leather_Normal.jpg");
	//		shared_ptr<Material> material = make_shared<Material>();
	//		material->SetShader(shader);
	//		material->SetTexture(0, texture);
	//		material->SetTexture(1, texture2);
	//		meshRenderer->SetMaterial(material);*/
	//		// Resource.cpp GameObject부분으로 이동

	//		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
	//		//material->SetInt(0, 1);
	//		meshRenderer->SetMaterial(material->Clone());
	//		/*material->SetInt(0, 0);
	//		meshRenderer->SetMaterial(material->Clone());*/
	//	}

	//	std::dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetRadius(0.5f);
	//	std::dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));

	//	obj->AddComponent(meshRenderer);
	//	scene->AddGameObject(obj);
	//}
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

#pragma region UI(Sample)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(0, 0, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Monkey", L"..\\Resources\\Texture\\Aim.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		scene->AddGameObject(sphere);
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

		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Point Light
	{
		//shared_ptr<GameObject> light = make_shared<GameObject>();
		//light->AddComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 150.f));
		//light->AddComponent(make_shared<Light>());
		////light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0.f));
		//light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		//light->GetLight()->SetDiffuse(Vec3(0.f, 0.5f, 0.f));		// GREEN
		//light->GetLight()->SetAmbient(Vec3(0.f, 0.3f, 0.f));
		//light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.f));
		//light->GetLight()->SetLightRange(200.f);
		////light->GetLight()->SetLightAngle(XM_PI / 4);
		//scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Spot Light
	{
		//shared_ptr<GameObject> light = make_shared<GameObject>();
		//light->AddComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(75.f, 0.f, 150.f));
		//light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(-1.f, 0.f, 0.f));
		//light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
		//light->GetLight()->SetDiffuse(Vec3(0.0f, 0.f, 0.5f));		// Blue
		//light->GetLight()->SetAmbient(Vec3(0.f, 0.f, 0.1f));
		//light->GetLight()->SetSpecular(Vec3(0.f, 0.f, 0.1f));
		//light->GetLight()->SetLightRange(200.f);
		//light->GetLight()->SetLightAngle(XM_PI / 4);
		//scene->AddGameObject(light);
	}
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

#pragma region FBX Dragon
	{
		/*shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Dragon\\Dragon.fbx");
		

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Dragon");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 00.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			scene->AddGameObject(gameObject);
			gameObject->AddComponent(make_shared<TestDragon>());
		}*/

		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Dragon.bin");
		vector<shared_ptr<GameObject>> gameObjects2 = meshData2->Instantiate();

		for (auto& gameObject : gameObjects2)
		{
			gameObject->SetName(L"Dragon2");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 300.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			scene->AddGameObject(gameObject);
			gameObject->AddComponent(make_shared<TestDragon>());
		}
	}
#pragma endregion

#pragma region FBX Player
	{
		//흐름 2)즉 여기에서 meshData에 대한 내용을 채워넣어야 한다.
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Player\\Player_Walk.fbx");
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Player_Walk.bin");

		//흐름 1)여기에서 gameObjects에 들거아야하는 meshData에는 mesh(메시정보와 애니메이션 정보), material이 있다.
		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Player");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 40.f, 100.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(0.10f, 0.10f, 0.10f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 3.14f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			scene->AddGameObject(gameObject);
			gameObject->AddComponent(make_shared<TestDragon>());
		}
	}
#pragma endregion
	//CreateMap(0.f, 0.f, 0.f);
	
	/*{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Gate\\Gate001.fbx");
		shared_ptr<MeshData> meshData2 = GET_SINGLE(Resources)->LoadBinaryModel(L"..\\Resources\\Binary\\Floor.bin");

		vector<shared_ptr<GameObject>> gameObjects = meshData2->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Gate001");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(0.f, 50.f, 10.f));
			gameObject->GetTransform()->SetLocalScale(Vec3(50.f, 50.f, 50.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			gameObject->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			scene->AddGameObject(gameObject);
		}
	}*/


	return scene;
}

void SceneManager::CreateAvatar(int object_type, int object_id, float x, float y, float z, int animation_id, float dirX, float dirY, float dirZ)
{
	{
		scene->GetMainCamera()->GetTransform()->SetLocalPosition(Vec3(x, y, z));
		scene->GetMainCamera()->GetTransform()->SetObjectType(object_type);
		scene->GetMainCamera()->GetTransform()->SetObjectID(object_id);
		scene->GetMainCamera()->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));
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

	shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Player\\Player(No animation).fbx");

	vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

	for (auto& gameObject : gameObjects)
	{
		gameObject->SetName(L"Player");
		gameObject->SetCheckFrustum(false);
		gameObject->GetTransform()->SetObjectType(object_type);
		gameObject->GetTransform()->SetObjectID(object_id);
		gameObject->GetTransform()->SetLocalPosition(Vec3(x, y, z));		//0.f, 45.f, 100.f
		gameObject->GetTransform()->SetLocalScale(Vec3(0.15f, 0.15f, 0.15f));		//(0.15f, 0.15f, 0.15f)
		gameObject->GetTransform()->SetLocalRotation(Vec3(dirX, dirY, dirZ));
		_otherPlayer.push_back(gameObject);
		scene->AddGameObject(gameObject);
	}


}

void SceneManager::ChangeObjectMovement(int object_id, float x, float y, float z, float dirX, float dirY, float dirZ)
{
	for (auto& otherPlayer : vp_ObjectManager)
	{
		if (otherPlayer.m_ObjectID == object_id)
		{
			otherPlayer.m_ObjectLocation = Vec3(x, y, z);
			otherPlayer.m_Direction = Vec3(0.f, dirY, dirZ);
		}
	}

	for (auto& otherPlayer : _otherPlayer)
	{
		if (otherPlayer->GetTransform()->GetObjectID() == object_id)
		{
			otherPlayer->GetTransform()->SetLocalPosition(Vec3(x, y, z));
			otherPlayer->GetTransform()->SetLocalRotation(Vec3(0.f, dirY, dirZ));
		}
	}
}

void SceneManager::CreateAisle(float aisleX, float aisleY, float aisleZ)
{
	
#pragma region Aisle
	Vec3 aislePosition = Vec3(aisleX, aisleY, aisleZ);	// 0, 0, 210
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleCeiling.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleCeiling");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			scene->AddGameObject(gameObject);
		}
	}
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleCeilingPipe.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleCeilingPipe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			scene->AddGameObject(gameObject);
		}
	}
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleFloor.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleFloor");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			scene->AddGameObject(gameObject);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AislePipe.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"AislePipe");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(aislePosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 3.14f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
	}
	
	for (int i = 0; i < 2; i++)
	{
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleWall.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"AisleWall");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(aislePosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 3.14f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
	}
	
	
#pragma endregion


}

void SceneManager::CreateAisle2(float aisleX, float aisleY, float aisleZ)
{

#pragma region Aisle
	Vec3 aislePosition = Vec3(aisleX, aisleY, aisleZ);	// 0, 0, 210
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleCeiling.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleCeiling");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 0.f));
			scene->AddGameObject(gameObject);
		}
	}
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleCeilingPipe.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleCeilingPipe");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 0.f));
			scene->AddGameObject(gameObject);
		}
	}
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleFloor.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"AisleFloor");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(aislePosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 1.57f, 0.f));
			scene->AddGameObject(gameObject);
		}
	}

	for (int i = 0; i < 2; i++)
	{
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AislePipe.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"AislePipe");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(aislePosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 3.14f + 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
	}

	for (int i = 0; i < 2; i++)
	{
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Aisle\\AisleWall.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"AisleWall");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(aislePosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 3.14f + 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
	}


#pragma endregion


}

void SceneManager::CreateMap(float mapX, float mapY, float mapZ)
{

#pragma region Map
	Vec3 mapPosition = Vec3(mapX, mapY, mapZ); //0, 0, 0

	//FLOOR------------------------------------
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Floor\\Floor.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		shared_ptr<Material> material;

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Floor");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(mapPosition);
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
			scene->AddGameObject(gameObject);
			//material = gameObject->GetMeshRenderer()->GetMaterial();
			//material->SetInt(0, 1);
			//gameObject->GetMeshRenderer()->SetMaterial(material);
		}
	}
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Floor\\Floor.fbx");

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		shared_ptr<Material> material;

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetName(L"Floor");
			gameObject->SetCheckFrustum(false);
			gameObject->GetTransform()->SetLocalPosition(Vec3(mapPosition.x, mapPosition.y + 250.f, mapPosition.z));
			gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			gameObject->GetTransform()->SetLocalRotation(Vec3(3.14f, 0.f, 0.f));
			scene->AddGameObject(gameObject);
			//material = gameObject->GetMeshRenderer()->GetMaterial();
			//material->SetInt(0, 1);
			//gameObject->GetMeshRenderer()->SetMaterial(material);
		}
	}
	
	//GATE------------------------------------
	
	for (int i = 0; i < 4; i++)
	{
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Gate\\Gate001.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Gate001");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Gate\\Gate001-2.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Gate001-2");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Gate\\GCeiling001.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"GCeiling001");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
	}

	


	//ROOMPIPE----------------------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\RoomPipe\\RoomPipe.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"RoomPipe");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
	}
	
	

	//WALL----------------------------------------------------------
	for (int i = 0; i < 4; i++)
	{
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Wall\\Wall001.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Wall001");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Wall\\Wall002.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Wall002");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(L"..\\Resources\\FBX\\Map\\Wall\\Wall003.fbx");

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetName(L"Wall003");
				gameObject->SetCheckFrustum(false);
				gameObject->GetTransform()->SetLocalPosition(mapPosition);
				gameObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				gameObject->GetTransform()->SetLocalRotation(Vec3(0.f, i * 1.57f, 0.f));
				scene->AddGameObject(gameObject);
			}
		}
	}
#pragma endregion

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