#include "pch.h"
#include "BinaryLoader.h"
#include "Mesh.h"
#include "Resources.h"
#include "Shader.h"
#include "Material.h"

BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken)
{
	BYTE nStrLength = 0;
	UINT nReads = 0;
	nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pstrToken, sizeof(char), nStrLength, pInFile);
	pstrToken[nStrLength] = '\0';

	return(nStrLength);
}


BinaryLoader::BinaryLoader()
{
}

BinaryLoader::~BinaryLoader()
{
}

void BinaryLoader::LoadBinary(const wstring& path)
{
	_resourceDirectory = path;
	
	AddMeshData();
	AddBonesData();
	AddAnimClipsData();
	AddAnimNames();

	
	//������ ä������ ������ ������� Texture�� Material�� ������ش�.
	CreateTextures();
	CreateMaterials();
}

void BinaryLoader::CreateTextures()
{
	for (size_t i = 0; i < _meshes.size(); i++)
	{
		for (size_t j = 0; j < _meshes[i].materials.size(); j++)
		{
			// DiffuseTexture
			{
				wstring relativePath = _meshes[i].materials[j].diffuseTexName.c_str();
				wstring filename = fs::path(relativePath).filename();
				wstring fullPath = _resourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
					GET_SINGLE(Resources)->Load<Texture>(filename, fullPath);
			}

			// NormalTexture
			{
				wstring relativePath = _meshes[i].materials[j].normalTexName.c_str();
				wstring filename = fs::path(relativePath).filename();
				wstring fullPath = _resourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
					GET_SINGLE(Resources)->Load<Texture>(filename, fullPath);
			}

			// SpecularTexture
			{
				wstring relativePath = _meshes[i].materials[j].specularTexName.c_str();
				wstring filename = fs::path(relativePath).filename();
				wstring fullPath = _resourceDirectory + L"\\" + filename;
				if (filename.empty() == false)
					GET_SINGLE(Resources)->Load<Texture>(filename, fullPath);
			}
		}
	}
}

void BinaryLoader::CreateMaterials()
{
	for (size_t i = 0; i < _meshes.size(); i++)
	{
		for (size_t j = 0; j < _meshes[i].materials.size(); j++)
		{
			shared_ptr<Material> material = make_shared<Material>();
			wstring key = _meshes[i].materials[j].name;
			material->SetName(key);
			material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));

			{
				wstring diffuseName = _meshes[i].materials[j].diffuseTexName.c_str();
				wstring filename = fs::path(diffuseName).filename();
				wstring key = filename;
				shared_ptr<Texture> diffuseTexture = GET_SINGLE(Resources)->Get<Texture>(key);
				if (diffuseTexture)
					material->SetTexture(0, diffuseTexture);
			}

			{
				wstring normalName = _meshes[i].materials[j].normalTexName.c_str();
				wstring filename = fs::path(normalName).filename();
				wstring key = filename;
				shared_ptr<Texture> normalTexture = GET_SINGLE(Resources)->Get<Texture>(key);
				if (normalTexture)
					material->SetTexture(1, normalTexture);
			}

			{
				wstring specularName = _meshes[i].materials[j].specularTexName.c_str();
				wstring filename = fs::path(specularName).filename();
				wstring key = filename;
				shared_ptr<Texture> specularTexture = GET_SINGLE(Resources)->Get<Texture>(key);
				if (specularTexture)
					material->SetTexture(2, specularTexture);
			}

			// �ϳ��� �𵨷� �������� �ϰ�;��(0, 1)
			material->SetInt(0, 1);

			// �ϳ��� �ϰ�������� 0, 0
			//material->SetInt(0, 0);

			GET_SINGLE(Resources)->Add<Material>(material->GetName(), material);
		}
	}
}

void BinaryLoader::FillBoneWeight(BinaryMeshInfo* meshInfo)
{
	const int32 size = static_cast<int32>(meshInfo->boneWeights.size());
	for (int32 v = 0; v < size; v++)
	{
		BinaryBoneWeight& boneWeight = meshInfo->boneWeights[v];
		boneWeight.Normalize();

		float animBoneIndex[4] = {};
		float animBoneWeight[4] = {};

		const int32 weightCount = static_cast<int32>(boneWeight.boneWeights.size());
		for (int32 w = 0; w < weightCount; w++)
		{
			animBoneIndex[w] = static_cast<float>(boneWeight.boneWeights[w].first);
			animBoneWeight[w] = static_cast<float>(boneWeight.boneWeights[w].second);
		}

		memcpy(&meshInfo->vertices[v].indices, animBoneIndex, sizeof(Vec4));
		memcpy(&meshInfo->vertices[v].weights, animBoneWeight, sizeof(Vec4));
	}
}

void BinaryLoader::AddMeshData()
{
	_meshes.push_back(BinaryMeshInfo());
	BinaryMeshInfo& meshInfo = _meshes.back();
	meshInfo.name;		//<Mesh>:
	for (auto& a : meshInfo.vertices)
	{
		a.pos;			//<Positions>:
		a.uv;			//<TextureCoords0>:
		a.normal;		//<Normals>:
		a.tangent;		//<Tangents>:
		a.weights;		//<BoneWeights>:
		a.indices;		//<BoneIndices>:
	}
	meshInfo.indices;	//<SubMesh>:
	for (auto& a : meshInfo.materials)
	{
		a.diffuse;		//<AlbedoColor>:
		a.ambient;		//<EmissiveColor>:
		a.specular;		//<SpecularColor>:
		a.name;			//<MaterialName>:
		a.diffuseTexName;	//<AlbedoMap>:
		a.normalTexName;	//<NormalMap>:
		a.specularTexName;	//<SpecularMap>:
	}
	meshInfo.boneWeights;	//??? 1Ȥ�� 2�� ����� �����ȴ�.
	meshInfo.hasAnimation;	//���� ���� ������ϳ�....
}

void BinaryLoader::AddBonesData()
{
	_bones.push_back(shared_ptr<BinaryBoneInfo>());
	shared_ptr<BinaryBoneInfo>& boneInfo = _bones.back();
	boneInfo->boneName;		//<BoneNames>:
	boneInfo->parentIndex;	//<ParentIndex>:
	boneInfo->matOffset;	//<BoneOffsets>:
}

void BinaryLoader::AddAnimClipsData()
{
	_animClips.push_back(shared_ptr<BinaryAnimClipInfo>());
	shared_ptr<BinaryAnimClipInfo>& animInfo = _animClips.back();
	animInfo->name;			//<AnimationClipName>:
	animInfo->startTime = 0.0f;	//0.0f����
	animInfo->endTime;		//<AnimationSet>:
	animInfo->mode = eFrames30;			//eFrame30����
	for (auto& a : animInfo->keyFrames)
	{
		a.push_back(BinaryKeyFrameInfo(Matrix(), 5));	//<TransformMatrix>: , <Frame>:
	}
}

void BinaryLoader::AddAnimNames()
{
	_animNames.push_back(string());
	string& animNameInfo = _animNames.back();
	animNameInfo;			//<AnimationClipName>:
}

