#pragma once
#include <string.h>

using namespace std;

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

struct BoneWeight
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
	vector<BoneWeight>					boneWeights; // �� ����ġ
	bool								hasAnimation;
};

struct BinaryKeyFrameInfo
{
	XMMATRIX	matTransform;
	double		time;
};

struct BinaryBoneInfo
{
	wstring					boneName;//���� �̸�
	int32					parentIndex;//�θ���
	XMMATRIX				matOffset;//4x4���
};

enum class EMode {
	eDefaultMode,
	eFrames24,
	eFrames30
};

struct BinaryAnimClipInfo
{
	wstring			name;//�ִϸ��̼� �̸�
	uint32			startTime;//���۽ð�
	uint32			endTime;	//������ frame�� ����(�ȱ�� 72�� ���Դ�)
	EMode			mode = EMode::eFrames30;		//eFrames30���� ����
	vector<vector<BinaryKeyFrameInfo>>	keyFrames;//�� ���� * ������ ����
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

