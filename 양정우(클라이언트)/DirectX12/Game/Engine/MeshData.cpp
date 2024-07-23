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

		// Material ã�Ƽ� ����
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
	} // run ���� ����, run ��ü�� �Ҹ��

	{
		FBXLoader loader4;
		loader4.LoadFbx(L"..\\Resources\\FBX\\Player4\\Player_Walk.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader4.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	} // shoot ���� ����, shoot ��ü�� �Ҹ��

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material ã�Ƽ� ����
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

	//����ؼ� ���� �߰�.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material ã�Ƽ� ����
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

	//����ؼ� ���� �߰�.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material ã�Ƽ� ����
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

	//����ؼ� ���� �߰�.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material ã�Ƽ� ����
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

	//����ؼ� ���� �߰�.......

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material ã�Ƽ� ����
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
	} // run ���� ����, run ��ü�� �Ҹ��

	{
		FBXLoader loader4;
		loader4.LoadFbx(L"..\\Resources\\FBX\\Player_Gun4\\test01.fbx");
		vector<shared_ptr<FbxAnimClipInfo>> shootAnim = loader4.GetAnimClip();
		for (const auto& clip : shootAnim) {
			loader.AddAnimClip(clip);
		}
	} // shoot ���� ����, shoot ��ü�� �Ҹ��

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromFBX(&loader.GetMesh(i), loader, keyname);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material ã�Ƽ� ����
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

	//�޽õ�� ��
	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		shared_ptr<Mesh> mesh = Mesh::CreateFromBinary(&loader.GetMesh(i), loader);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		// Material ã�Ƽ� ����
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
		//�޽� ��
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->AddComponent(make_shared<Transform>());
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(info.mesh);

		//���͸���
		for (uint32 i = 0; i < info.materials.size(); i++)
		{
			//gameObject->GetMeshRenderer()->SetMaterial(info.materials[i]->Clone(), i);
			gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);
		}

		//���� �޽����� ���ο� �ִϸ��̼� ������ �ִٸ�.
		if (info.mesh->IsAnimMesh())
		{
			//���ϸ��̼� ��
			shared_ptr<Animator> animator = make_shared<Animator>();
			gameObject->AddComponent(animator);
			animator->SetBones(info.mesh->GetBones());
			animator->SetAnimClip(info.mesh->GetAnimClip());
		}

		v.push_back(gameObject);
	}

	//�ش� ������ ���� gameObject������ ����
	//�� ���� ������Ʈ���� �޽�, ���͸���, �ִϸ��̼� ������ �ִ�.
	return v;
}

