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

struct BoneWeight
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
	vector<BoneWeight>					boneWeights; // 뼈 가중치
	bool								hasAnimation;
};

struct BinaryKeyFrameInfo
{
	XMMATRIX	matTransform;
	double		time;
};

struct BinaryBoneInfo
{
	wstring					boneName;//뼈의 이름
	int32					parentIndex;//부모노드
	XMMATRIX				matOffset;//4x4행렬
};

enum class EMode {
	eDefaultMode,
	eFrames24,
	eFrames30
};

struct BinaryAnimClipInfo
{
	wstring			name;//애니메이션 이름
	uint32			startTime;//시작시간
	uint32			endTime;	//마지막 frame의 숫자(걷기는 72가 나왔다)
	EMode			mode = EMode::eFrames30;		//eFrames30으로 고정
	vector<vector<BinaryKeyFrameInfo>>	keyFrames;//본 갯수 * 프레임 갯수
};

class BinaryLoader
{


private:
	wstring									_resourceDirectory;

	vector<BinaryMeshInfo>					_meshes;
	vector<shared_ptr<BinaryBoneInfo>>		_bones;
	vector<shared_ptr<BinaryAnimClipInfo>>	_animClips;
	vector<string>							_animNames;
};

