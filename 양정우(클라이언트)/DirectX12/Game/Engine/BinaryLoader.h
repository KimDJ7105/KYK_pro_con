#pragma once

struct MaterialInfo	//�� ����
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
	//vector<BoneWeight> boneWeights;	�ϴ��� Mesh�� ���°� ��ǥ�� �ϱ⿡ �ּ�ó��
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

