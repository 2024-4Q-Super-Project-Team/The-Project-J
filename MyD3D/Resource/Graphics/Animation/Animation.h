#pragma once
#include "Resource/Resource.h"

class AnimationNode;

// 하나의 모션을 가지는 애니메이션 구조체
class AnimationResource 
    : public Resource
{
public:
    RESOURCE_TYPE(Animation);
    explicit AnimationResource(std::wstring_view _name);
    virtual ~AnimationResource();
public:
    inline void     SetFramePerSecond(float _fps) { mFramePerSecond = _fps; }
    inline void     SetTotalFrame(float _totalFrame) { mTotalFrame = _totalFrame; }
    inline float    GetTotalFrame() { return mTotalFrame; }
    inline float    GetFramePerSecond() { return mFramePerSecond; }
    inline float    GetAnimationTotalTime() { return mTotalFrame / mFramePerSecond; }
    AnimationNode*  GetChannel(std::wstring_view _key);
    BOOL            AddChannel(AnimationNode* _pChannel);
private:
    float mTotalFrame;
    float mFramePerSecond;
    std::unordered_map<std::wstring, AnimationNode*> mChannels;
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};
// 애니메이션을 구성하는 각 노드의 정보들
// 채널은 노드의 이름과 같아야한다.
class AnimationNode
{
public:
    template <typename TYPE>
    class KeyResource
    {
    public:
        KeyResource(TYPE _val, float _time)
            : Value(_val), Time(_time) {
        }
        ~KeyResource() = default;
        TYPE		Value;
        float		Time;
    };
public:
    explicit AnimationNode(std::wstring_view _name, AnimationResource* _pAnimResource);
    virtual ~AnimationNode();
public:
    inline const std::wstring& GetName() { return mName; }
public:
    std::vector<KeyResource<Vector3>>		mPositionKeys;
    std::vector<KeyResource<Quaternion>>	mRotationKeys;
    std::vector<KeyResource<Vector3>>		mScalingKeys;
private:
    std::wstring mName;
    AnimationResource* mOwner;		// 속한 애니메이션 정보
};