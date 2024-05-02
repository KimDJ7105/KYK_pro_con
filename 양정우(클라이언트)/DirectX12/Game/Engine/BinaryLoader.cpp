#include "pch.h"
#include "BinaryLoader.h"
#include "Mesh.h"
#include "Resources.h"
#include "Shader.h"
#include "Material.h"

#include <locale>
#include <codecvt>
#include <string>

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

std::wstring ConvertCharToWString(const char* source) {
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, source, -1, nullptr, 0);
	if (requiredSize == 0) {
		// 변환 실패
		return L"";
	}

	std::wstring result(requiredSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, source, -1, &result[0], requiredSize);
	return result;
}

BinaryLoader::BinaryLoader()
{
}

BinaryLoader::~BinaryLoader()
{
	//데이터 집어넣을때 썻던 애들
	if (m_pxmf3Positions) delete[] m_pxmf3Positions;
	if (m_pvec3Positions) delete[] m_pvec3Positions;

	if (m_pxmf4Colors) delete[] m_pxmf4Colors;

	if (m_pxmf3Normals) delete[] m_pxmf3Normals;
	if (m_pvec33Normals) delete[] m_pvec33Normals;

	if (m_pxmf3Tangents) delete[] m_pxmf3Tangents;
	if (m_pvec3Tangents) delete[] m_pvec3Tangents;


	if (m_pxmf3BiTangents) delete[] m_pxmf3BiTangents;
	if (m_pxmf2TextureCoords0) delete[] m_pxmf2TextureCoords0;
	if (m_pvec2TextureCoords0) delete[] m_pvec2TextureCoords0;
	if (m_pxmf2TextureCoords1) delete[] m_pxmf2TextureCoords1;

	if (m_nSubMeshes > 0)
	{
		if (m_pnSubSetIndices) delete[] m_pnSubSetIndices;
		if (m_ppnSubSetIndices) delete[] m_ppnSubSetIndices;
	}

	if (m_ppstrSkinningBoneNames) delete[] m_ppstrSkinningBoneNames;
	if (m_pxmf4x4BindPoseBoneOffsets) delete[] m_pxmf4x4BindPoseBoneOffsets;

	if (m_pxmn4BoneIndices) delete[] m_pxmn4BoneIndices;
	if (m_pvec4BoneIndices) delete[] m_pvec4BoneIndices;

	if (m_pxmf4BoneWeights) delete[] m_pxmf4BoneWeights;
	if (m_pvec4BoneWeights) delete[] m_pvec4BoneWeights;

}

void BinaryLoader::LoadBinary(const wstring& path)
{
	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string utf8Path(utf8Length, 0);
	WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, &utf8Path[0], utf8Length, nullptr, nullptr);

	// 2. UTF-8 문자열을 char*로 변환
	const char* charPath = utf8Path.c_str();

	_resourceDirectory = path;

	LoadGeometryAndAnimationFromFile(charPath);

	AddMeshData();
	//AddBonesData();
	//AddAnimClipsData();
	//AddAnimNames();

	
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
	meshInfo.name = m_strMeshName; // <Mesh>:

	// 정점 추가
	for (int i = 0; i < m_iPositionNum; i++) {
		meshInfo.vertices.push_back(Vertex());
		meshInfo.vertices.back().pos = m_pvec3Positions[i]; // <Positions>:
	}

	// 텍스처 좌표 추가
	for (int i = 0; i < m_iTextureCoords0Num; i++) {
		meshInfo.vertices[i].uv = m_pvec2TextureCoords0[i]; // <TextureCoords0>:
	}

	// 법선 추가
	for (int i = 0; i < m_iNormalsNum; i++) {
		meshInfo.vertices[i].normal = m_pvec33Normals[i]; // <Normals>:
	}

	// 탄젠트 추가
	for (int i = 0; i < m_iTangentsNum; i++) {
		meshInfo.vertices[i].tangent = m_pvec3Tangents[i]; // <Tangents>:
	}

	// 뼈 가중치 추가
	for (int i = 0; i < m_nVertices; i++) {
		meshInfo.vertices[i].weights = m_pvec4BoneWeights[i]; // <BoneWeights>:
	}

	// 뼈 인덱스 추가
	for (int i = 0; i < m_nVertices; i++) {
		meshInfo.vertices[i].indices = m_pvec4BoneIndices[i]; // <BoneIndices>:
	}

	for (int i = 0; i < m_nSubMeshes; i++) {
		// 서브메시의 인덱스 배열을 가져옴
		uint32* subMeshIndices = m_ppnSubSetIndices[i];
		// 해당 배열의 크기를 가져옴
		int subMeshIndexCount = m_pnSubSetIndices[i];

		// 새로운 서브메시 인덱스 벡터를 생성하여 meshInfo.indices에 추가
		std::vector<uint32> subMeshVector;
		for (int j = 0; j < subMeshIndexCount; j++) {
			subMeshVector.push_back(subMeshIndices[j]);
		}
		meshInfo.indices.push_back(subMeshVector);	//<SubMesh>:
	}


	for (auto& a : meshInfo.materials)
	{
		a.diffuse = m_xmf4AlbedoColor;		//<AlbedoColor>:
		a.ambient = m_xmf4EmissiveColor;		//<EmissiveColor>:
		a.specular = m_xmf4SpecularColor;		//<SpecularColor>:
		a.name = m_strMaterialName;			//<MaterialName>:
		a.diffuseTexName = m_strDiffuseTexName;	//<AlbedoMap>:
		a.normalTexName = m_strNormalTexName;	//<NormalMap>:
		a.specularTexName = m_strSpecularTexName;	//<SpecularMap>:
	}


	meshInfo.boneWeights;	//??? 1혹은 2로 사이즈가 고정된다.
	meshInfo.hasAnimation;	//내가 직접 해줘야하나....
}

void BinaryLoader::AddBonesData()
{
	for (int i = 0; i < m_nSkinningBones; i++)
	{
		shared_ptr<BinaryBoneInfo> boneInfo = make_shared<BinaryBoneInfo>();

		// 이름 설정
		boneInfo->boneName = ConvertCharToWString(m_ppstrSkinningBoneNames[i]);		//<BoneNames>:

		boneInfo->parentIndex;	//<ParentIndex>:
		boneInfo->matOffset;	//<BoneOffsets>:

		// _bones에 추가
		_bones.push_back(boneInfo);
	}
	
}

void BinaryLoader::AddAnimClipsData()
{
	_animClips.push_back(shared_ptr<BinaryAnimClipInfo>());
	shared_ptr<BinaryAnimClipInfo>& animInfo = _animClips.back();
	animInfo->name;			//<AnimationClipName>:
	animInfo->startTime;	//0.0f고정
	animInfo->endTime;		//<AnimationSet>:
	animInfo->mode;			//eFrame30고정
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

void BinaryLoader::LoadGeometryAndAnimationFromFile(const char* pstrFileName)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, pstrFileName, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				LoadFrameHierarchyFromFile(pInFile);
				::ReadStringFromFile(pInFile, pstrToken); //"</Hierarchy>"
			}
			else if (!strcmp(pstrToken, "<Animation>:"))
			{
				LoadAnimationFromFile(pInFile);
			}
			else if (!strcmp(pstrToken, "</Animation>:"))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}

void BinaryLoader::LoadFrameHierarchyFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nFrame = 0, nTextures = 0;


	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Frame>:"))
		{
			nFrame = ::ReadIntegerFromFile(pInFile);
			nTextures = ::ReadIntegerFromFile(pInFile);

			::ReadStringFromFile(pInFile, m_pstrFrameName);
		}
		else if (!strcmp(pstrToken, "<Transform>:"))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
		}
		else if (!strcmp(pstrToken, "<TransformMatrix>:"))
		{
			nReads = (UINT)::fread(&m_xmf4x4ToParent, sizeof(float), 16, pInFile);
		}
		else if (!strcmp(pstrToken, "<ParentIndex>:"))
		{
			//당근칼-요주의
			int a = ::ReadIntegerFromFile(pInFile);
			/*if (a > 0)
			{
				for (int i = 0; i < a; i++)
				{
					nReads = (UINT)::fread(asd, sizeof(UINT), a, pInFile);
				}
			}*/
			
		}
		else if (!strcmp(pstrToken, "<Mesh>:"))
		{
			LoadMeshFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<SkinningInfo>:"))
		{
			LoadSkinInfoFromFile(pInFile);

			::ReadStringFromFile(pInFile, pstrToken); //<Mesh>:
			if (!strcmp(pstrToken, "<Mesh>:")) LoadMeshFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<Materials>:"))
		{
			LoadMaterialsFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = ::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					LoadFrameHierarchyFromFile(pInFile);
#ifdef _WITH_DEBUG_FRAME_HIERARCHY
					TCHAR pstrDebug[256] = { 0 };
					_stprintf_s(pstrDebug, 256, "(Frame: %p) (Parent: %p)\n"), pChild, pGameObject);
					OutputDebugString(pstrDebug);
#endif
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
}

void BinaryLoader::LoadMeshFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	UINT nReads = (UINT)::fread(&m_nVertices, sizeof(int), 1, pInFile);

	::ReadStringFromFile(pInFile, m_pstrMeshName);

	m_strMeshName = ConvertCharToWString(m_pstrMeshName);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&m_xmf3AABBCenter, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&m_xmf3AABBExtents, sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Positions>:"))
		{
			nReads = (UINT)::fread(&nPositions, sizeof(int), 1, pInFile);
			m_iPositionNum = nPositions;
			if (nPositions > 0)
			{
				//m_pxmf3Positions = new XMFLOAT3[nPositions];
				m_pvec3Positions = new Vec3[nPositions];
				//nReads = (UINT)::fread(m_pxmf3Positions, sizeof(XMFLOAT3), nPositions, pInFile);
				nReads = (UINT)::fread(m_pvec3Positions, sizeof(Vec3), nPositions, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Colors>:"))
		{
			nReads = (UINT)::fread(&nColors, sizeof(int), 1, pInFile);
			if (nColors > 0)
			{
				m_pxmf4Colors = new XMFLOAT4[nColors];
				nReads = (UINT)::fread(m_pxmf4Colors, sizeof(XMFLOAT4), nColors, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords0>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			m_iTextureCoords0Num = nTextureCoords;
			if (nTextureCoords > 0)
			{
				//m_pxmf2TextureCoords0 = new XMFLOAT2[nTextureCoords];
				//nReads = (UINT)::fread(m_pxmf2TextureCoords0, sizeof(XMFLOAT2), nTextureCoords, pInFile);
				m_pvec2TextureCoords0 = new Vec2[nTextureCoords];
				nReads = (UINT)::fread(m_pvec2TextureCoords0, sizeof(Vec2), nTextureCoords, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords1>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				m_pxmf2TextureCoords1 = new XMFLOAT2[nTextureCoords];
				nReads = (UINT)::fread(m_pxmf2TextureCoords1, sizeof(XMFLOAT2), nTextureCoords, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nReads = (UINT)::fread(&nNormals, sizeof(int), 1, pInFile);
			m_iNormalsNum = nNormals;
			if (nNormals > 0)
			{
				//m_pxmf3Normals = new XMFLOAT3[nNormals];
				//nReads = (UINT)::fread(m_pxmf3Normals, sizeof(XMFLOAT3), nNormals, pInFile);
				m_pvec33Normals = new Vec3[nNormals];
				nReads = (UINT)::fread(m_pvec33Normals, sizeof(Vec3), nNormals, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<Tangents>:"))
		{
			nReads = (UINT)::fread(&nTangents, sizeof(int), 1, pInFile);
			m_iTangentsNum = nTangents;
			if (nTangents > 0)
			{
				//m_pxmf3Tangents = new XMFLOAT3[nTangents];
				//nReads = (UINT)::fread(m_pxmf3Tangents, sizeof(XMFLOAT3), nTangents, pInFile);
				m_pvec3Tangents = new Vec3[nTangents];
				nReads = (UINT)::fread(m_pvec3Tangents, sizeof(Vec3), nTangents, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<BiTangents>:"))
		{
			nReads = (UINT)::fread(&nBiTangents, sizeof(int), 1, pInFile);
			if (nBiTangents > 0)
			{
				m_pxmf3BiTangents = new XMFLOAT3[nBiTangents];
				nReads = (UINT)::fread(m_pxmf3BiTangents, sizeof(XMFLOAT3), nBiTangents, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			nReads = (UINT)::fread(&(m_nSubMeshes), sizeof(int), 1, pInFile);
			if (m_nSubMeshes > 0)
			{
				m_pnSubSetIndices = new int[m_nSubMeshes];
				m_ppnSubSetIndices = new UINT * [m_nSubMeshes];
				for (int i = 0; i < m_nSubMeshes; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int nIndex = 0;
						
						nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile); //i
						nReads = (UINT)::fread(&(m_pnSubSetIndices[i]), sizeof(int), 1, pInFile);
						if (m_pnSubSetIndices[i] > 0)
						{
							m_ppnSubSetIndices[i] = new UINT[m_pnSubSetIndices[i]];
							nReads = (UINT)::fread(m_ppnSubSetIndices[i], sizeof(UINT), m_pnSubSetIndices[i], pInFile);

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

void BinaryLoader::LoadSkinInfoFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	::ReadStringFromFile(pInFile, m_pstrMeshName);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<BonesPerVertex>:"))
		{
			m_nBonesPerVertex = ::ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&m_xmf3AABBCenter, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&m_xmf3AABBExtents, sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<BoneNames>:"))
		{
			m_nSkinningBones = ::ReadIntegerFromFile(pInFile);
			if (m_nSkinningBones > 0)
			{
				m_ppstrSkinningBoneNames = new char[m_nSkinningBones][64];
				for (int i = 0; i < m_nSkinningBones; i++)
				{
					::ReadStringFromFile(pInFile, m_ppstrSkinningBoneNames[i]);
				}
			}
		}
		else if (!strcmp(pstrToken, "<BoneOffsets>:"))
		{
			m_nSkinningBones = ::ReadIntegerFromFile(pInFile);
			if (m_nSkinningBones > 0)
			{
				m_pxmf4x4BindPoseBoneOffsets = new XMFLOAT4X4[m_nSkinningBones];
				nReads = (UINT)::fread(m_pxmf4x4BindPoseBoneOffsets, sizeof(XMFLOAT4X4), m_nSkinningBones, pInFile);

				for (int i = 0; i < m_nSkinningBones; i++)
				{
				}
			}
		}
		else if (!strcmp(pstrToken, "<BoneIndices>:"))
		{

			m_nVertices = ::ReadIntegerFromFile(pInFile);
			if (m_nVertices > 0)
			{
				/*m_pxmn4BoneIndices = new XMINT4[m_nVertices];
				nReads = (UINT)::fread(m_pxmn4BoneIndices, sizeof(XMINT4), m_nVertices, pInFile);*/
				m_pvec4BoneIndices = new MyInt4[m_nVertices];
				nReads = (UINT)::fread(m_pvec4BoneIndices, sizeof(MyInt4), m_nVertices, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "<BoneWeights>:"))
		{

			m_nVertices = ::ReadIntegerFromFile(pInFile);
			if (m_nVertices > 0)
			{
				/*m_pxmf4BoneWeights = new XMFLOAT4[m_nVertices];
				nReads = (UINT)::fread(m_pxmf4BoneWeights, sizeof(XMFLOAT4), m_nVertices, pInFile);*/
				m_pvec4BoneWeights = new Vec4[m_nVertices];
				nReads = (UINT)::fread(m_pvec4BoneWeights, sizeof(Vec4), m_nVertices, pInFile);
			}
		}
		else if (!strcmp(pstrToken, "</SkinningInfo>"))
		{
			break;
		}
	}
}

void BinaryLoader::LoadMaterialsFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nMaterial = 0;
	UINT nReads = 0;

	m_nMaterials = ReadIntegerFromFile(pInFile);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);

		if (!strcmp(pstrToken, "<Material>:"))
		{
			nMaterial = ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<MaterialName>:"))
		{
			//당근칼-요주의
			::ReadStringFromFile(pInFile, m_pstrMaterialName);
			m_strMaterialName = ConvertCharToWString(m_pstrMaterialName);
		}
		else if (!strcmp(pstrToken, "<AlbedoColor>:"))
		{
			nReads = (UINT)::fread(&m_xmf4AlbedoColor, sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<EmissiveColor>:"))
		{
			nReads = (UINT)::fread(&m_xmf4EmissiveColor, sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularColor>:"))
		{
			nReads = (UINT)::fread(&m_xmf4SpecularColor, sizeof(float), 4, pInFile);
		}
		else if (!strcmp(pstrToken, "<Glossiness>:"))
		{
			nReads = (UINT)::fread(&m_fGlossiness, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Smoothness>:"))
		{
			nReads = (UINT)::fread(&m_fSmoothness, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Metallic>:"))
		{
			nReads = (UINT)::fread(&m_fSpecularHighlight, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<SpecularHighlight>:"))
		{
			nReads = (UINT)::fread(&m_fMetallic, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<GlossyReflection>:"))
		{
			nReads = (UINT)::fread(&m_fGlossyReflection, sizeof(float), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoMap>:"))
		{
			//m_pstrDiffuseTexName = LoadTextureFromFile(pInFile);
			::ReadStringFromFile(pInFile, m_pstrDiffuseTexName);
			m_strDiffuseTexName = ConvertCharToWString(m_pstrDiffuseTexName);
		}
		else if (!strcmp(pstrToken, "<SpecularMap>:"))
		{
			//m_pstrSpecularTexName = LoadTextureFromFile(pInFile);
			::ReadStringFromFile(pInFile, m_pstrSpecularTexName);
			m_strSpecularTexName = ConvertCharToWString(m_pstrSpecularTexName);
		}
		else if (!strcmp(pstrToken, "<NormalMap>:"))
		{
			//m_pstrNormalTexName = LoadTextureFromFile(pInFile);
			::ReadStringFromFile(pInFile, m_pstrNormalTexName);
			m_strNormalTexName = ConvertCharToWString(m_pstrNormalTexName);
		}
		else if (!strcmp(pstrToken, "<MetallicMap>:"))
		{
			LoadTextureFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<EmissionMap>:"))
		{
			LoadTextureFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<DetailAlbedoMap>:"))
		{
			LoadTextureFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<DetailNormalMap>:"))
		{
			LoadTextureFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "</Materials>"))
		{
			break;
		}
	}
}

char* BinaryLoader::LoadTextureFromFile(FILE* pInFile)
{
	//char pstrTextureName[64] = { '\0' };
	char* pstrTextureName = new char[64];
	BYTE nStrLength = 64;
	UINT nReads = (UINT)::fread(&nStrLength, sizeof(BYTE), 1, pInFile);
	nReads = (UINT)::fread(pstrTextureName, sizeof(char), nStrLength, pInFile);
	pstrTextureName[nStrLength] = '\0';

	bool bDuplicated = false;
	
	return pstrTextureName;
}

void BinaryLoader::LoadAnimationFromFile(FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nAnimationSets = 0;

	for ( ; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<AnimationSets>:"))
		{
			nAnimationSets = ::ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<FrameNames>:"))
		{
			m_nBoneFrames = ::ReadIntegerFromFile(pInFile); 
			for (int j = 0; j < m_nBoneFrames; j++)
			{
				::ReadStringFromFile(pInFile, pstrToken);

#ifdef _WITH_DEBUG_SKINNING_BONE
				TCHAR pstrDebug[256] = { 0 };
				TCHAR pwstrAnimationBoneName[64] = { 0 };
				TCHAR pwstrBoneCacheName[64] = { 0 };
				size_t nConverted = 0;
				mbstowcs_s(&nConverted, pwstrAnimationBoneName, 64, pstrToken, _TRUNCATE);
				mbstowcs_s(&nConverted, pwstrBoneCacheName, 64, pLoadedModel->m_ppBoneFrameCaches[j]->m_pstrFrameName, _TRUNCATE);
				_stprintf_s(pstrDebug, 256, _T("AnimationBoneFrame:: Cache(%s) AnimationBone(%s)\n"), pwstrBoneCacheName, pwstrAnimationBoneName);
				OutputDebugString(pstrDebug);
#endif
			}
		}
		else if (!strcmp(pstrToken, "<AnimationClipName>:"))
		{
			//당근칼-요주의
			::ReadStringFromFile(pInFile, m_pstrAnimationClipName);
		}
		else if (!strcmp(pstrToken, "<AnimationSet>:"))
		{
			int nAnimationSet = ::ReadIntegerFromFile(pInFile);

			::ReadStringFromFile(pInFile, pstrToken); //Animation Set Name

			float fLength = ::ReadFloatFromFile(pInFile);
			int nFramesPerSecond = ::ReadIntegerFromFile(pInFile);
			int nKeyFrames = ::ReadIntegerFromFile(pInFile);
			for (int i = 0; i < nKeyFrames; i++)
			{
				::ReadStringFromFile(pInFile, pstrToken);
				if (!strcmp(pstrToken, "<Transforms>:"))
				{

					int nKey = ::ReadIntegerFromFile(pInFile); //i
					float fKeyTime = ::ReadFloatFromFile(pInFile);

#ifdef _WITH_ANIMATION_SRT
					m_pfKeyFrameScaleTimes[i] = fKeyTime;
					m_pfKeyFrameRotationTimes[i] = fKeyTime;
					m_pfKeyFrameTranslationTimes[i] = fKeyTime;
					nReads = (UINT)::fread(pAnimationSet->m_ppxmf3KeyFrameScales[i], sizeof(XMFLOAT3), pLoadedModel->m_pAnimationSets->m_nBoneFrames, pInFile);
					nReads = (UINT)::fread(pAnimationSet->m_ppxmf4KeyFrameRotations[i], sizeof(XMFLOAT4), pLoadedModel->m_pAnimationSets->m_nBoneFrames, pInFile);
					nReads = (UINT)::fread(pAnimationSet->m_ppxmf3KeyFrameTranslations[i], sizeof(XMFLOAT3), pLoadedModel->m_pAnimationSets->m_nBoneFrames, pInFile);
#else
					nReads = (UINT)::fread(m_ppxmf4x4KeyFrameTransforms[i], sizeof(XMFLOAT4X4), m_nBoneFrames, pInFile);
#endif
				}
			}
		}
		else if (!strcmp(pstrToken, "</AnimationSets>"))
		{
			break;
		}
	}
}

