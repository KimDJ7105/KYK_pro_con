#pragma once

#include "FBXLoader.h"




class BinaryMeshData : FBXLoader
{
public:
	BinaryMeshData();
	~BinaryMeshData();

	void LoadMeshData(const wstring& path);

	int32 GetMeshCount() { return static_cast<int32>(_meshes.size()); }
	FbxMeshInfo& GetMesh(int32 idx) { return _meshes[idx]; }
	vector<FbxMeshInfo> GetMeshs() { return _meshes; }

public:
	void AddMeshData();

	void LoadGeometryAndAnimationFromFile(const char* pstrFileName);
	void LoadFrameHierarchyFromFile(FILE* pInFile);

private:

	vector<FbxMeshInfo>					_meshes;

	vector<int>parentContainer;

private:
	vector<UINT> m_nParentIndex;

private:
	char							m_pstrFrameName[64];
	XMFLOAT4X4						m_xmf4x4ToParent;
	vector<Matrix>					m_vmatToParent;

public:
	void LoadMeshFromFile(FILE* pInFile);

private:
	int								m_nVertices = 0;
	char							m_pstrMeshName[64] = { 0 };
	wstring							m_strMeshName;
	XMFLOAT3						m_xmf3AABBCenter = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3						m_xmf3AABBExtents = XMFLOAT3(0.0f, 0.0f, 0.0f);

	int m_iPositionNum = -1;
	XMFLOAT3* m_pxmf3Positions = NULL;
	Vec3* m_pvec3Positions = NULL;

	XMFLOAT4* m_pxmf4Colors = NULL;

	int m_iTextureCoords0Num = -1;
	XMFLOAT2* m_pxmf2TextureCoords0 = NULL;
	Vec2* m_pvec2TextureCoords0 = NULL;

	XMFLOAT2* m_pxmf2TextureCoords1 = NULL;

	int m_iNormalsNum = -1;
	XMFLOAT3* m_pxmf3Normals = NULL;
	Vec3* m_pvec33Normals = NULL;

	int m_iTangentsNum = -1;
	XMFLOAT3* m_pxmf3Tangents = NULL;
	Vec3* m_pvec3Tangents = NULL;

	XMFLOAT3* m_pxmf3BiTangents = NULL;
	int								m_nSubMeshes = 0;

	int* m_pnSubSetIndices = NULL;
	UINT** m_ppnSubSetIndices = NULL;

public:
	void LoadSkinInfoFromFile(FILE* pInFile);

private:
	int								m_nBonesPerVertex = 4;
	int								m_nSkinningBones = 0;
	char(*m_ppstrSkinningBoneNames)[64];
	XMFLOAT4X4* m_pxmf4x4BindPoseBoneOffsets = NULL;
	Matrix* m_pvec4x4BindPoseBoneOffsets = NULL;


	XMINT4* m_pxmn4BoneIndices = NULL;
	Vec4* m_pvec4BoneIndices = NULL;	//당근칼 - 원래는 int의 4개씩 배열인데 일단 이렇게 했다.

	XMFLOAT4* m_pxmf4BoneWeights = NULL;
	Vec4* m_pvec4BoneWeights = NULL;

public:
	void LoadMaterialsFromFile(FILE* pInFile);

private:
	int								m_nMaterials = 0;
	XMFLOAT4						m_xmf4AlbedoColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4						m_xmf4EmissiveColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4SpecularColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4						m_xmf4AmbientColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	vector<XMFLOAT4>						m_vAlbedoColor;
	vector<XMFLOAT4>						m_vEmissiveColor;
	vector<XMFLOAT4>						m_vSpecularColor;

	float							m_fGlossiness = 0.0f;
	float							m_fSmoothness = 0.0f;
	float							m_fSpecularHighlight = 0.0f;
	float							m_fMetallic = 0.0f;
	float							m_fGlossyReflection = 0.0f;
	char							m_pstrMaterialName[64] = { 0 };
	vector<wstring>							m_strMaterialName;

public:
	//void LoadTextureFromFile(FILE* pInFile);
	char* LoadTextureFromFile(FILE* pInFile);
	char							m_pstrDiffuseTexName[64] = { 0 };
	char							m_pstrNormalTexName[64] = { 0 };
	char							m_pstrSpecularTexName[64] = { 0 };
	vector<wstring>						m_strDiffuseTexName;
	vector<wstring>						m_strNormalTexName;
	vector<wstring>						m_strSpecularTexName;

	void LoadAnimationFromFile(FILE* pInFile);
	vector<wstring>			m_vstrAnimClipNames;
	int m_nAnimClipConut = 0;
	bool isAnimation = false;
	char							m_pstrAnimationClipName[64] = { 0 };
	vector<wstring> m_vstrFrameNames;

private:
	int								m_nBoneFrames = 0;
	XMFLOAT4X4** m_ppxmf4x4KeyFrameTransforms = NULL;

	Matrix* asd = NULL;
};

