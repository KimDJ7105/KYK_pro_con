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
		// �帧 5) ���⼭ �ϴ� mesh�� ���� ������ ä�����ϰ��ִ�.
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

shared_ptr<MeshData> MeshData::LoadFromBinary(const wstring& path)
{
	BinaryLoader loader;
	loader.LoadBinary(path);

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	//�޽õ�� ��
	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		// �帧 5) ���⼭ �ϴ� mesh�� ���� ������ ä�����ϰ��ִ�.
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
			gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);

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

