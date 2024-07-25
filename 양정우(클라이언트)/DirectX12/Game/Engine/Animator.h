#pragma once
#include "Component.h"
#include "Mesh.h"
#include <queue>

class Material;
class StructuredBuffer;
class Mesh;

class Animator : public Component
{
public:
	Animator();
	virtual ~Animator();

public:
	void SetBones(const vector<BoneInfo>* bones) { _bones = bones; }
	void SetAnimClip(const vector<AnimClipInfo>* animClips);
	void PushData();

	int32 GetAnimCount() { return static_cast<uint32>(_animClips->size()); }
	int32 GetCurrentClipIndex() { return _clipIndex; }
	void Play(uint32 idx);

	void PlaySequence(const std::vector<uint32>& sequence);
	void AddToSequence(uint32 idx);
	void ClearSequence(); // 큐를 비우는 함수
	uint32 GetLastAddedAnimationType() const; // 마지막 추가된 애니메이션의 종류를 리턴하는 함수
	float GetAnimationEndTime(uint32 idx) const; // 애니메이션 번호를 입력받아 종료 시간을 리턴하는 함수

public:
	virtual void FinalUpdate() override;

private:
	const vector<BoneInfo>* _bones;
	const vector<AnimClipInfo>* _animClips;

	float							_updateTime = 0.f;
	int32							_clipIndex = 0;
	int32							_frame = 0;
	int32							_nextFrame = 0;
	float							_frameRatio = 0;

	shared_ptr<Material>			_computeMaterial;
	shared_ptr<StructuredBuffer>	_boneFinalMatrix;  // 특정 프레임의 최종 행렬
	bool							_boneFinalUpdated = false;

	std::queue<uint32>               _sequenceQueue;     // 애니메이션 시퀀스 큐
	bool							_isPlayingSequence = false;
	uint32							_lastClipIndex = 0;		// 마지막 실행한 클립 인덱스
	uint32							_lastAddedClipIndex = 0; // 마지막 추가된 클립 인덱스
};
