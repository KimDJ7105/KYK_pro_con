#pragma once

struct MaterialInfo	//색 정보
{
	Vec4 diffuse;
	Vec4 ambient;
	Vec4 specualr;
	wstring name;
	wstring diffuseTexName;
	wstring specularTexName;
};

struct MeshInfo
{
	wstring name;
	vector<Vertex> vertices;
	vector<MaterialInfo> materials;
	//vector<BoneWeight> boneWeights;	일단은 Mesh를 띄우는걸 목표로 하기에 주석처리
	bool hasAnimation;
};

class BinaryLoader
{
public:
	BinaryLoader();
	~BinaryLoader();

public:
	void LoadBinary(const wstring& path);
};

