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
	void ClearSequence(); // ť�� ���� �Լ�
	uint32 GetLastAddedAnimationType() const; // ������ �߰��� �ִϸ��̼��� ������ �����ϴ� �Լ�
	float GetAnimationEndTime(uint32 idx) const; // �ִϸ��̼� ��ȣ�� �Է¹޾� ���� �ð��� �����ϴ� �Լ�

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
	shared_ptr<StructuredBuffer>	_boneFinalMatrix;  // Ư�� �������� ���� ���
	bool							_boneFinalUpdated = false;

	std::queue<uint32>               _sequenceQueue;     // �ִϸ��̼� ������ ť
	bool							_isPlayingSequence = false;
	uint32							_lastClipIndex = 0;		// ������ ������ Ŭ�� �ε���
	uint32							_lastAddedClipIndex = 0; // ������ �߰��� Ŭ�� �ε���
};