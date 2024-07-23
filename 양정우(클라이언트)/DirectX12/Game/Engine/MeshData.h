#pragma once
#include "Object.h"

using std::shared_ptr;
using std::vector;
using std::wstring;

class Mesh;
class Material;
class GameObject;

struct MeshRenderInfo
{
	shared_ptr<Mesh>				mesh;
	vector<shared_ptr<Material>>	materials;
};

class MeshData : public Object
{
public:
	MeshData();
	virtual ~MeshData();

public:
	static shared_ptr<MeshData> LoadFromFBX(const wstring& path);
	static shared_ptr<MeshData> LoadFromBinary(const wstring& path);
	static shared_ptr<MeshData> LoadPlayerModel(const wstring& keyname);
	static shared_ptr<MeshData> LoadGunModel(const wstring& keyname);

	virtual void Load(const wstring& path);
	virtual void Save(const wstring& path);

	vector<shared_ptr<GameObject>> Instantiate();

private:
	shared_ptr<Mesh>				_mesh;
	vector<shared_ptr<Material>>	_materials;

	vector<MeshRenderInfo> _meshRenders;

	XMFLOAT3 AABBCenter;
	XMFLOAT3 AABBExtents;

public:
	XMFLOAT3 GetAABBCenter(){ return AABBCenter; }
	XMFLOAT3 GetAABBExtents() { return AABBExtents; }


private:
	shared_ptr<Mesh> m_Mesh;
	vector<shared_ptr<Material>>	m_Materials;
	vector<MeshRenderInfo> m_MeshRenderers;

public:
	static shared_ptr<MeshData> LoadBlueTeamModel(const wstring& keyname);
	static shared_ptr<MeshData> LoadGreenTeamModel(const wstring& keyname);
	static shared_ptr<MeshData> LoadPurpleTeamModel(const wstring& keyname);
	static shared_ptr<MeshData> LoadRedTeamModel(const wstring& keyname);
};
