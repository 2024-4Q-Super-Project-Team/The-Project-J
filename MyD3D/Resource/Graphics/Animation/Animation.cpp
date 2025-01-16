#include "pch.h"
#include "Animation.h"

AnimationResource::AnimationResource(std::wstring_view _name)
    : Resource(_name)
    , mTotalFrame(0.0f)
    , mFramePerSecond(0.0f)
{
}
AnimationResource::~AnimationResource()
{
}
AnimationNode* AnimationResource::GetChannel(std::wstring_view _key)
{
    auto itr = mChannels.find(_key.data());
    if (itr != mChannels.end())
    {
        return itr->second;
    }
    return nullptr;
}

BOOL AnimationResource::AddChannel(AnimationNode* _pChannel)
{
    if (GetChannel(_pChannel->GetName()) == nullptr)
    {
        mChannels[_pChannel->GetName()] = _pChannel;
        return TRUE;
    }
    return FALSE;
}

void AnimationResource::EditorRendering()
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	std::string name = Helper::ToString(mName);

    EDITOR_COLOR_RESOURCE;
	if (ImGui::TreeNodeEx(("Animation : " + name + uid).c_str(), EDITOR_FLAG_RESOURCE))
	{
		ImGui::Text("Total Frame : %f", mTotalFrame);
		ImGui::Text("Frame Per Second : %f", mFramePerSecond);
		ImGui::Separator();
        ImGui::Text("Channel Count : %d", mChannels.size());
		ImGui::TreePop();
	}
    EDITOR_COLOR_POP(1);
}

AnimationNode::AnimationNode(std::wstring_view _name, AnimationResource* _pAnimResource)
    : mName(_name), mOwner(_pAnimResource)
{
}
AnimationNode::~AnimationNode()
{
}