#include "pch.h"
#include "Animator.h"
#include "Timer.h"
#include "Resources.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "StructuredBuffer.h"
#include <algorithm>

using std::min;
using std::max;

Animator::Animator() : Component(COMPONENT_TYPE::ANIMATOR)
{
    _computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeAnimation");
    _boneFinalMatrix = make_shared<StructuredBuffer>();
}

Animator::~Animator()
{
}

void Animator::FinalUpdate()
{
    _updateTime += DELTA_TIME;

    const AnimClipInfo& animClip = _animClips->at(_clipIndex);
    if (_updateTime >= animClip.duration)
    {
        _updateTime = 0.f;
        if (_isPlayingSequence)
        {
            if (!_sequenceQueue.empty())
            {
                _clipIndex = _sequenceQueue.front();
                _sequenceQueue.pop();
            }
            else
            {
                _clipIndex = _lastClipIndex; // 마지막 클립 인덱스 재생
            }
        }
    }

    const int32 ratio = static_cast<int32>(animClip.frameCount / animClip.duration);
    _frame = static_cast<int32>(_updateTime * ratio);
    _frame = min(_frame, animClip.frameCount - 1);
    _nextFrame = min(_frame + 1, animClip.frameCount - 1);
    _frameRatio = (_updateTime * ratio) - _frame; // _frameRatio 계산 수정

    PushData();
}

void Animator::SetAnimClip(const vector<AnimClipInfo>* animClips)
{
    _animClips = animClips;
}

void Animator::PushData()
{
    uint32 boneCount = static_cast<uint32>(_bones->size());
    if (_boneFinalMatrix->GetElementCount() < boneCount)
        _boneFinalMatrix->Init(sizeof(Matrix), boneCount);

    shared_ptr<Mesh> mesh = GetGameObject()->GetMeshRenderer()->GetMesh();
    mesh->GetBoneFrameDataBuffer(_clipIndex)->PushComputeSRVData(SRV_REGISTER::t8);
    mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);

    _boneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);

    _computeMaterial->SetInt(0, boneCount);
    _computeMaterial->SetInt(1, _frame);
    _computeMaterial->SetInt(2, _nextFrame);
    _computeMaterial->SetFloat(0, _frameRatio);

    uint32 groupCount = (boneCount / 256) + 1;
    _computeMaterial->Dispatch(groupCount, 1, 1);

    _boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

void Animator::Play(uint32 idx)
{
    if (idx != _clipIndex)
    {
        assert(idx < _animClips->size());
        _clipIndex = idx;
        _updateTime = 0.f;
        _lastClipIndex = idx; // 마지막 실행한 클립 인덱스 업데이트
        _isPlayingSequence = false;
        ClearSequence(); // 큐를 비움
    }
}

void Animator::PlaySequence(const vector<uint32>& sequence)
{
    assert(!sequence.empty());

    ClearSequence(); // 큐를 비움

    for (uint32 idx : sequence)
    {
        assert(idx < _animClips->size());
        _sequenceQueue.push(idx);
    }

    _clipIndex = _sequenceQueue.front();
    _sequenceQueue.pop();
    _updateTime = 0.f;
    _isPlayingSequence = true;
}

void Animator::AddToSequence(uint32 idx)
{
    assert(idx < _animClips->size());
    _sequenceQueue.push(idx);
    _lastAddedClipIndex = idx; // 마지막 추가된 클립 인덱스 업데이트

    // 만약 마지막 애니메이션이 반복 중이고 큐가 비어있었다면, 새 애니메이션을 바로 시작
    if (!_isPlayingSequence && _sequenceQueue.size() == 1)
    {
        _clipIndex = _sequenceQueue.front();
        _sequenceQueue.pop();
        _updateTime = 0.f;
        _isPlayingSequence = true;
    }
}

void Animator::ClearSequence()
{
    while (!_sequenceQueue.empty())
    {
        _sequenceQueue.pop();
    }
    _isPlayingSequence = false;
}

uint32 Animator::GetLastAddedAnimationType() const
{
    return _lastAddedClipIndex;
}

float Animator::GetAnimationEndTime(uint32 idx) const
{
    assert(idx < _animClips->size());

    float endTime = 0.f;
    if (_isPlayingSequence)
    {
        std::queue<uint32> tempQueue = _sequenceQueue;
        while (!tempQueue.empty())
        {
            uint32 currentIdx = tempQueue.front();
            tempQueue.pop();
            endTime += _animClips->at(currentIdx).duration;
        }
    }

    return endTime;
}