#pragma once
#include <string.h>

using namespace std;


extern BYTE ReadStringFromFile(FILE* pInFile, char* pstrToken);
extern int ReadIntegerFromFile(FILE* pInFile);
extern float ReadFloatFromFile(FILE* pInFile);
extern wstring ConvertCharToWString(const char* source);

//���͸��� ���� ����
struct BinaryMaterialInfo
{
	Vec4			diffuse;
	Vec4			ambient;
	Vec4			specular;
	wstring			name;
	wstring			diffuseTexName;
	wstring			normalTexName;
	wstring			specularTexName;
};

struct BinaryBoneWeight
{
	using Pair = pair<int32, double>;
	vector<Pair> boneWeights;

	// ����ġ�� �ִ� 4����
	void AddWeights(uint32 index, double weight)
	{
		if (weight <= 0.f)
			return;

		auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
			[=](const Pair& p) { return p.second < weight; });

		if (findIt != boneWeights.end())
			boneWeights.insert(findIt, Pair(index, weight));
		else
			boneWeights.push_back(Pair(index, weight));

		// ����ġ�� �ִ� 4��
		if (boneWeights.size() > 4)
			boneWeights.pop_back();
	}

	// ���� 1�� ����
	void Normalize()
	{
		double sum = 0.f;
		std::for_each(boneWeights.begin(), boneWeights.end(), [&](Pair& p) { sum += p.second; });
		std::for_each(boneWeights.begin(), boneWeights.end(), [=](Pair& p) { p.second = p.second / sum; });
	}
};

// �޽��� ������ �޾ƿ�
struct BinaryMeshInfo
{
	wstring								name;	//�޽� �̸�
	vector<Vertex>						vertices;//������ ���� ����
	vector<vector<uint32>>				indices; //�ε��� ���۰� �������� �ɰ��� �� �־ �̷��� ���·�...
	vector<BinaryMaterialInfo>			materials;
	vector<BinaryBoneWeight>			boneWeights; // �� ����ġ
	bool								hasAnimation;//�������� 1�̻��̶��
};

struct BinaryKeyFrameInfo
{
	Matrix		matTransform;	//
	double		time;			//fKeyTime
};

struct BinaryBoneInfo
{
	wstring					boneName;//���� �̸�	//<Frame>:
	int32					parentIndex;//�θ���	
	Matrix					matOffset;//4x4���		//<Transform>:
};


struct BinaryAnimClipInfo
{
	wstring			name;//�ִϸ��̼� �̸� = pstrToken
	uint32			startTime;	//���۽ð� = 0
	uint32			endTime;	//nFramesPerSecond(������ �� ū�����ε� �� �׳� ������ ������ �����ߴ�.
	uint32			mode;		//nKeyFrames
	double			animeEndTime;
	vector<vector<BinaryKeyFrameInfo>>	keyFrames;//�� ���� * ������ ����->���⼭ �� 180�������� ���Ͱ� �� ���������
};

class BinaryLoader
{
public:
	BinaryLoader();
	~BinaryLoader();

public:
	void LoadBinary(const wstring& path);

public:
	int32 GetMeshCount() { return static_cast<int32>(_meshes.size()); }
	const BinaryMeshInfo& GetMesh(int32 idx) { return _meshes[idx]; }
	vector<shared_ptr<BinaryBoneInfo>>& GetBones() { return _bones; }
	vector<shared_ptr<BinaryAnimClipInfo>>& GetAnimClip() { return _animClips; }

private:
	/*void Import(const wstring& path);

	void ParseNode(FbxNode* root);
	void LoadMesh(FbxMesh* mesh);
	void LoadMaterial(FbxSurfaceMaterial* surfaceMaterial);*/

	/*void		GetNormal(FbxMesh* mesh, BinaryMeshInfo* container, int32 idx, int32 vertexCounter);
	void		GetTangent(FbxMesh* mesh, BinaryMeshInfo* container, int32 idx, int32 vertexCounter);
	void		GetUV(FbxMesh* mesh, BinaryMeshInfo* container, int32 idx, int32 vertexCounter);*/
	/*Vec4		GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName);
	wstring		GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty);*/

	void CreateTextures();
	void CreateMaterials();

	// Animation
	/*void LoadBones(FbxNode* node) { LoadBones(node, 0, -1); }
	void LoadBones(FbxNode* node, int32 idx, int32 parentIdx);
	void LoadAnimationInfo();

	void LoadAnimationData(FbxMesh* mesh, BinaryMeshInfo* meshInfo);
	void LoadBoneWeight(FbxCluster* cluster, int32 boneIdx, BinaryMeshInfo* meshInfo);
	void LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, BinaryMeshInfo* meshInfo);
	void LoadKeyframe(int32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, BinaryMeshInfo* container);

	int32 FindBoneIndex(string name);
	FbxAMatrix GetTransform(FbxNode* node);*/

	void FillBoneWeight(BinaryMeshInfo* meshInfo);

private:
	wstring									_resourceDirectory;

	vector<BinaryMeshInfo>					_meshes;
	vector<shared_ptr<BinaryBoneInfo>>		_bones;
	vector<shared_ptr<BinaryAnimClipInfo>>	_animClips;
	vector<wstring>							_animNames;

	vector<shared_ptr<float>> _boundsValues;

public:

	void AddMeshData();
	void AddBonesData();
	void AddAnimClipsData(int boneNum, int keyFrames, wstring animName, uint32 sTime, uint32 eTime, uint32 md, vector<float> time, Matrix* asd, double endAnimeTime);
	void AddAnimNames();


public:
	
	void LoadGeometryAndAnimationFromFile(const char* pstrFileName);
	void LoadFrameHierarchyFromFile(FILE* pInFile);

private:
	vector<int>parentContainer;

private:
	vector<UINT> m_nParentIndex;

private:
	char							m_pstrFrameName[64];
	XMFLOAT4X4						m_xmf4x4ToParent;// �θ���� �������� ��ȯ���
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
	Vec4* m_pvec4BoneIndices = NULL;	//���Į - ������ int�� 4���� �迭�ε� �ϴ� �̷��� �ߴ�.

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
	char							m_pstrDiffuseTexName[64] = {0};
	char							m_pstrNormalTexName[64] = {0};
	char							m_pstrSpecularTexName[64] = {0};
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

public:
	// _meshes ����� �����ϴ� GetMeshes() �޼��� �߰�
	vector<BinaryMeshInfo>& GetMeshes() {
		return _meshes;
	}

	// const ������ GetMeshes() �޼��� �߰�
	const vector<BinaryMeshInfo>& GetMeshes() const {
		return _meshes;
	}

	XMFLOAT3 GetAABBCenter() { return m_xmf3AABBCenter; }
	XMFLOAT3 GetAABBExtents() { return m_xmf3AABBExtents; }

};
