#include "pch.h"
#include "MeshData.h"
#include "FBXLoader.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Animator.h"

#include "BinaryLoader.h"

MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

shared_ptr<MeshData> MeshData::LoadFromFBX(const wstring& path)
{
	FBXLoader loader;
	loader.LoadFbx(path);

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, path);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

shared_ptr<MeshData> MeshData::LoadPlayerModel(const wstring& keyname)
{
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player2\\Player_Walk.fbx");
	{
		FBXLoader loader3;
		loader3.LoadFbx(L"..\\Resources\\FBX\\Player3\\Player_Walk.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader3.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	} // run 범위 종료, run 객체는 소멸됨

	{
		FBXLoader loader4;
		loader4.LoadFbx(L"..\\Resources\\FBX\\Player4\\Player_Walk.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader4.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	} // shoot 범위 종료, shoot 객체는 소멸됨

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}
//===============================================================================
// Team Player 
shared_ptr<MeshData> MeshData::LoadBlueTeamModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\IDLE_1\\Blue_Player_Single_IShoot.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\IDLE_2\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Change_IDLE\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Reload_IDLE\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Single_Shoot_IDLE\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Walk_1\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Walk_2\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Change_Walk\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Reload_Walk\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Single_Shoot_Walk\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Run_1\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Run_2\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Change_Run\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Reload_Run\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Single_Shoot_Run\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerBlue\\Death\\Blue_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

shared_ptr<MeshData> MeshData::LoadGreenTeamModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\IDLE_1\\Green_Player_Single_IShoot.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\IDLE_2\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Change_IDLE\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Reload_IDLE\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Single_Shoot_IDLE\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Walk_1\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Walk_2\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Change_Walk\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Reload_Walk\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Single_Shoot_Walk\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Run_1\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Run_2\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Change_Run\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Reload_Run\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Single_Shoot_Run\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGreen\\Death\\Green_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

shared_ptr<MeshData> MeshData::LoadPurpleTeamModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\IDLE_1\\Purple_Player_Single_IShoot.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\IDLE_2\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Change_IDLE\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Reload_IDLE\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Single_Shoot_IDLE\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Walk_1\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Walk_2\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Change_Walk\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Reload_Walk\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Single_Shoot_Walk\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Run_1\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Run_2\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Change_Run\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Reload_Run\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Single_Shoot_Run\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerPurple\\Death\\Purple_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

shared_ptr<MeshData> MeshData::LoadRedTeamModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\IDLE_1\\Red_Player_Single_IShoot.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\IDLE_2\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Change_IDLE\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Reload_IDLE\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Single_Shoot_IDLE\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Walk_1\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Walk_2\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Change_Walk\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Reload_Walk\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Single_Shoot_Walk\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Run_1\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Run_2\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Change_Run\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Reload_Run\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Single_Shoot_Run\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerRed\\Death\\Red_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}
//===============================================================================
// Guns
//AR
shared_ptr<MeshData> MeshData::Load_AR_GunModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\IDLE_1\\AR01_Player_Single_IShoot.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\IDLE_2\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Change_IDLE\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Reload_IDLE\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Single_Shoot_IDLE\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Walk_1\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Walk_2\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Change_Walk\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Reload_Walk\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Single_Shoot_Walk\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Run_1\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Run_2\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Change_Run\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Reload_Run\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Single_Shoot_Run\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_AR\\Death\\AR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

//PO
shared_ptr<MeshData> MeshData::Load_PO_GunModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\IDLE_1\\P01_Player_Single_IShoot.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\IDLE_2\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Change_IDLE\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Reload_IDLE\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Single_Shoot_IDLE\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Walk_1\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Walk_2\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Change_Walk\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Reload_Walk\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Single_Shoot_Walk\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Run_1\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Run_2\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Change_Run\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Reload_Run\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Single_Shoot_Run\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_PO\\Death\\P01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

//SG
shared_ptr<MeshData> MeshData::Load_SG_GunModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\IDLE_1\\SG01_Player_Single_IShoot.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\IDLE_2\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Change_IDLE\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Reload_IDLE\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Single_Shoot_IDLE\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Walk_1\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Walk_2\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Change_Walk\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Reload_Walk\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Single_Shoot_Walk\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Run_1\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Run_2\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Change_Run\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Reload_Run\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Single_Shoot_Run\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SG\\Death\\SG01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

//SMG
shared_ptr<MeshData> MeshData::Load_SMG_GunModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\IDLE_1\\SMG02_Run_Reload.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\IDLE_2\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Change_IDLE\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Reload_IDLE\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Single_Shoot_IDLE\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Walk_1\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Walk_2\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Change_Walk\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Reload_Walk\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Single_Shoot_Walk\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Run_1\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Run_2\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Change_Run\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Reload_Run\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Single_Shoot_Run\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SM\\Death\\SMG02_Run_Reload.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

//SR
shared_ptr<MeshData> MeshData::Load_SR_GunModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\IDLE_1\\SR01_Player_Single_IShoot.fbx");
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\IDLE_2\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Change_IDLE\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Reload_IDLE\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Single_Shoot_IDLE\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader1.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// WALK
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Walk_1\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Walk_2\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Change_Walk\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Reload_Walk\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Single_Shoot_Walk\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Run
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Run_1\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Run_2\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Change_Run\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Reload_Run\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Single_Shoot_Run\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Death
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\PlayerGun_SR\\Death\\SR01_Player_Single_IShoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

//===============================================================================
// Player Hands

shared_ptr<MeshData> MeshData::LoadBlueHandModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Blue\\IDLE\\Blue_FP_Shoot.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Blue\\Shoot\\Blue_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Blue\\Reload\\Blue_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Blue\\Change\\Blue_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	

	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

shared_ptr<MeshData> MeshData::LoadGreenHandModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Green\\IDLE\\Green_FP_Shoot.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Green\\Shoot\\Green_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Green\\Reload\\Green_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Green\\Change\\Green_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}


	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

shared_ptr<MeshData> MeshData::LoadPurpleHandModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Purple\\IDLE\\Purple_FP_Shoot.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Purple\\Shoot\\Purple_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Purple\\Reload\\Purple_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Purple\\Change\\Purple_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}


	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

shared_ptr<MeshData> MeshData::LoadRedHandModel(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Red\\IDLE\\Red_FP_Shoot.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Red\\Shoot\\Red_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Red\\Reload\\Red_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_Red\\Change\\Red_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}


	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

//===============================================================================
// Player Guns
// AR
shared_ptr<MeshData> MeshData::Load_AR_FPS(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_AR\\IDLE\\AR01_FP_Shoot.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_AR\\Shoot\\AR01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_AR\\Reload\\AR01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_AR\\Change\\AR01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}


	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

// PO
shared_ptr<MeshData> MeshData::Load_PO_FPS(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_PO\\IDLE\\P01_FP_Shoot.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_PO\\Shoot\\P01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_PO\\Reload\\P01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_PO\\Change\\P01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}


	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

// SG
shared_ptr<MeshData> MeshData::Load_SG_FPS(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SG\\IDLE\\SG01_FP_Shoot.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SG\\Shoot\\SG01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SG\\Reload\\SG01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SG\\Change\\SG01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}


	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

// SMG
shared_ptr<MeshData> MeshData::Load_SMG_FPS(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SMG\\IDLE\\SMG02_Hand_IDLE.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SMG\\Shoot\\SMG02_Hand_IDLE.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SMG\\Reload\\SMG02_Hand_IDLE.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SMG\\Change\\SMG02_Hand_IDLE.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}


	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

// SR
shared_ptr<MeshData> MeshData::Load_SR_FPS(const wstring& keyname)
{
	// IDLE
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SR\\IDLE\\SR01_FP_Shoot.fbx");

	// Shoot
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SR\\Shoot\\SR01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Reload
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SR\\Reload\\SR01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}
	// Change
	{
		FBXLoader loader1;
		loader1.LoadFbx(L"..\\Resources\\FBX\\Player_Hand_GUN_SR\\Change\\SR01_FP_Shoot.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader1.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	}


	//계속해서 모델을 추가.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}

//===============================================================================
shared_ptr<MeshData> MeshData::LoadGunModel(const wstring& keyname)
{
	FBXLoader loader;
	loader.LoadFbx(L"..\\Resources\\FBX\\Player_Gun2\\test01.fbx");
	{
		FBXLoader loader3;
		loader3.LoadFbx(L"..\\Resources\\FBX\\Player_Gun3\\test01.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> runAnim = loader3.GetAnimClip();
		for (const auto& clip : runAnim) {
			loader.AddAnimClip(clip);
		}
	} // run 범위 종료, run 객체는 소멸됨

	{
		FBXLoader loader4;
		loader4.LoadFbx(L"..\\Resources\\FBX\\Player_Gun4\\test01.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader4.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	} // shoot 범위 종료, shoot 객체는 소멸됨

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);
			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}


shared_ptr<MeshData> MeshData::LoadFromBinary(const wstring& path)
{
	BinaryLoader loader;
	loader.LoadBinary(path);



	shared_ptr<MeshData> meshData = make_shared<MeshData>();
	meshData->AABBCenter = loader.GetAABBCenter();
	meshData->AABBExtents = loader.GetAABBExtents();

	//메시덩어리 수
	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromBinary(&loader.GetMesh(i), loader);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);


			materials.push_back(material);
		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}


void MeshData::Load(const wstring& _strFilePath)
{
	// TODO
}

void MeshData::Save(const wstring& _strFilePath)
{
	// TODO
}

vector<shared_ptr<GameObject>> MeshData::Instantiate()
{
	vector<shared_ptr<GameObject>> v;

	for (MeshRenderInfo& info : _meshRenders)
	{
		//메시 셋
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->AddComponent(make_shared<Transform>());
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(info.mesh);

		//머터리얼
		for (uint32 i = 0; i < info.materials.size(); i++)
		{
			//gameObject->GetMeshRenderer()->SetMaterial(info.materials[i]->Clone(), i);
			gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);
		}

		//만약 메시정보 내부에 애니메이션 정보가 있다면.
		if (info.mesh->IsAnimMesh())
		{
			//에니메이션 셋
			shared_ptr<Animator> animator = make_shared<Animator>();
			gameObject->AddComponent(animator);
			animator->SetBones(info.mesh->GetBones());
			animator->SetAnimClip(info.mesh->GetAnimClip());
		}

		v.push_back(gameObject);
	}

	//해당 정보를 담은 gameObject정보를 리턴
	//즉 게임 오브젝트에는 메시, 머터리얼, 애니메이션 정보가 있다.
	return v;
}

