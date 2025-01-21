#pragma once
#include "Resource/Resource.h"

class AnimationNode;

// �ϳ��� ����� ������ �ִϸ��̼� ����ü
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
// �ִϸ��̼��� �����ϴ� �� ����� ������
// ä���� ����� �̸��� ���ƾ��Ѵ�.
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
    AnimationResource* mOwner;		// ���� �ִϸ��̼� ����
};