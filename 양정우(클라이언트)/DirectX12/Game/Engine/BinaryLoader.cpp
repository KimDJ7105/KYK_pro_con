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

int ReadIntegerFromFile(FILE* pInFile)
{
	int nValue = 0;
	UINT nReads = (UINT)::fread(&nValue, sizeof(int), 1, pInFile);
	return(nValue);
}

float ReadFloatFromFile(FILE* pInFile)
{
	float fValue = 0;
	UINT nReads = (UINT)::fread(&fValue, sizeof(float), 1, pInFile);
	return(fValue);
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
	
	const char* a = "..\\Resources\\FBX\\Map\\Gate\\Gate001.fbx";

	LoadModel(a);


	/*AddMeshData();
	AddBonesData();
	AddAnimClipsData();
	AddAnimNames();*/

	
	//위에서 채워넣은 정보를 기반으로 Texture와 Material을 만들어준다.
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

			// 하나의 모델로 여러개를 하고싶어요(0, 1)
			material->SetInt(0, 1);

			// 하나만 하고싶을때는 0, 0
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
	meshInfo.boneWeights;	//??? 1혹은 2로 사이즈가 고정된다.
	meshInfo.hasAnimation;	//내가 직접 해줘야하나....
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
	animInfo->startTime = 0.0f;	//0.0f고정
	animInfo->endTime;		//<AnimationSet>:
	animInfo->mode = eFrames30;			//eFrame30고정
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

void BinaryLoader::LoadModel(const char* pstrFileName)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	LoadMeshFromFile(pInFile);
}

void BinaryLoader::LoadMeshFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	int	m_nVertices = 0;
	UINT nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pInFile);
	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			UINT nReads = (UINT)::fread(&_boundsValues, sizeof(float), 1, pInFile);
			if (nReads == 1) { // 실제로 1개의 float 값을 읽었는지 확인
				// <Bounds> 뒤의 값을 처리하는 코드
			}
			else {
				// 파일에서 더 이상 데이터를 읽을 수 없는 경우, 또는 오류가 발생한 경우
				break; // 읽기를 멈춥니다.
			}
		}
		else if (!strcmp(pstrToken, "<Positions>:"))
		{
			nReads = (UINT)::fread(&nPositions, sizeof(int), 1, pInFile);
			if (nPositions > 0)
			{
				
			}
		}
		else if (!strcmp(pstrToken, "<Colors>:"))
		{
			nReads = (UINT)::fread(&nColors, sizeof(int), 1, pInFile);
			if (nColors > 0)
			{
				
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords0>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords1>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				
			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nReads = (UINT)::fread(&nNormals, sizeof(int), 1, pInFile);
			if (nNormals > 0)
			{
				
			}
		}
		else if (!strcmp(pstrToken, "<Tangents>:"))
		{
			nReads = (UINT)::fread(&nTangents, sizeof(int), 1, pInFile);
			if (nTangents > 0)
			{
				
			}
		}
		else if (!strcmp(pstrToken, "<BiTangents>:"))
		{
			nReads = (UINT)::fread(&nBiTangents, sizeof(int), 1, pInFile);
			if (nBiTangents > 0)
			{
				
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			int								m_nSubMeshes = 0;
			nReads = (UINT)::fread(&(m_nSubMeshes), sizeof(int), 1, pInFile);
			if (m_nSubMeshes > 0)
			{
				for (int i = 0; i < m_nSubMeshes; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int* m_pnSubSetIndices = NULL;
						int nIndex = 0;
						nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile); //i
						nReads = (UINT)::fread(&(m_pnSubSetIndices[i]), sizeof(int), 1, pInFile);
						if (m_pnSubSetIndices[i] > 0)
						{
							
						}
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Mesh>"))
		{
			break;
		}
	}
}



