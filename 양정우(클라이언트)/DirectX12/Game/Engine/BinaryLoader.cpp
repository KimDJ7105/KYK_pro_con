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

void BinaryLoader::AddMeshData()
{
	_meshes.push_back(BinaryMeshInfo());
	BinaryMeshInfo& meshInfo = _meshes.back();
	meshInfo.name;
	for (auto& a : meshInfo.vertices)
	{
		a.pos;
		a.uv;
		a.normal;
		a.tangent;
		a.weights;
		a.normal;
	}
	meshInfo.indices;
	for (auto& a : meshInfo.materials)
	{
		a.diffuse;
		a.ambient;
		a.specular;
		a.name;
		a.diffuseTexName;
		a.normalTexName;
		a.specularTexName;
	}
	meshInfo.boneWeights;
	meshInfo.hasAnimation;
}

void BinaryLoader::AddBonesData()
{
	_bones.push_back(shared_ptr<BinaryBoneInfo>());
	shared_ptr<BinaryBoneInfo>& boneInfo = _bones.back();
	boneInfo->boneName;
	boneInfo->parentIndex;
	boneInfo->matOffset;
}

void BinaryLoader::AddAnimClipsData()
{
	_animClips.push_back(shared_ptr<BinaryAnimClipInfo>());
	shared_ptr<BinaryAnimClipInfo>& animInfo = _animClips.back();
	animInfo->name;
	animInfo->startTime;
	animInfo->endTime;
	animInfo->mode;
	for (auto& a : animInfo->keyFrames)
	{
		a.push_back(BinaryKeyFrameInfo(Matrix(), 5));
	}
	
}

void BinaryLoader::AddAnimNames()
{
	_animNames.push_back(string());
	string& animNameInfo = _animNames.back();
	animNameInfo;
}




