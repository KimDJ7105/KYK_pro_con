#pragma once
#include <string.h>

using namespace std;

//머터리얼 관련 정보
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

	// 가중치는 최대 4개로
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

		// 가중치는 최대 4개
		if (boneWeights.size() > 4)
			boneWeights.pop_back();
	}

	// 합을 1로 보정
	void Normalize()
	{
		double sum = 0.f;
		std::for_each(boneWeights.begin(), boneWeights.end(), [&](Pair& p) { sum += p.second; });
		std::for_each(boneWeights.begin(), boneWeights.end(), [=](Pair& p) { p.second = p.second / sum; });
	}
};

// 메쉬의 정보를 받아옴
struct BinaryMeshInfo
{
	wstring								name;	//메쉬 이름
	vector<Vertex>						vertices;//정점에 대한 정보
	vector<vector<uint32>>				indices; //인덱스 버퍼가 여러개로 쪼개질 수 있어서 이러한 형태로...
	vector<BinaryMaterialInfo>			materials;
	vector<BinaryBoneWeight>			boneWeights; // 뼈 가중치
	bool								hasAnimation;//프레임이 1이상이라면
};

struct BinaryKeyFrameInfo
{
	Matrix		matTransform;
	double		time;
};

struct BinaryBoneInfo
{
	wstring					boneName;//뼈의 이름
	int32					parentIndex;//부모노드
	Matrix					matOffset;//4x4행렬
};

enum EMode {
	eDefaultMode = 0,	//디폴트 프레임
	eFrames24 = 24,		//24프레임
	eFrames30 = 30		//30프레임
};

struct BinaryAnimClipInfo
{
	wstring			name;//애니메이션 이름
	uint32			startTime;	//시작시간
	uint32			endTime;	//마지막 frame의 숫자(걷기는 72가 나왔다)
	EMode			mode = EMode::eFrames30;		//eFrames30으로 고정
	vector<vector<BinaryKeyFrameInfo>>	keyFrames;//본 갯수 * 프레임 갯수
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
	//void Import(const wstring& path);

	//void ParseNode(FbxNode* root);
	//void LoadMesh(FbxMesh* mesh);
	//void LoadMaterial(FbxSurfaceMaterial* surfaceMaterial);

	//void		GetNormal(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	//void		GetTangent(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	//void		GetUV(FbxMesh* mesh, FbxMeshInfo* container, int32 idx, int32 vertexCounter);
	//Vec4		GetMaterialData(FbxSurfaceMaterial* surface, const char* materialName, const char* factorName);
	//wstring		GetTextureRelativeName(FbxSurfaceMaterial* surface, const char* materialProperty);

	//void CreateTextures();
	//void CreateMaterials();

	//// Animation
	//void LoadBones(FbxNode* node) { LoadBones(node, 0, -1); }
	//void LoadBones(FbxNode* node, int32 idx, int32 parentIdx);
	//void LoadAnimationInfo();

	//void LoadAnimationData(FbxMesh* mesh, FbxMeshInfo* meshInfo);
	//void LoadBoneWeight(FbxCluster* cluster, int32 boneIdx, FbxMeshInfo* meshInfo);
	//void LoadOffsetMatrix(FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* meshInfo);
	//void LoadKeyframe(int32 animIndex, FbxNode* node, FbxCluster* cluster, const FbxAMatrix& matNodeTransform, int32 boneIdx, FbxMeshInfo* container);

	//int32 FindBoneIndex(string name);
	//FbxAMatrix GetTransform(FbxNode* node);

	//void FillBoneWeight(FbxMesh* mesh, FbxMeshInfo* meshInfo);

private:
	wstring									_resourceDirectory;

	vector<BinaryMeshInfo>					_meshes;
	vector<shared_ptr<BinaryBoneInfo>>		_bones;
	vector<shared_ptr<BinaryAnimClipInfo>>	_animClips;
	vector<string>							_animNames;
};

