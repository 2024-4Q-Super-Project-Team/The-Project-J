#include "pch.h"
#include "Animation.h"

AnimationResource::AnimationResource(ResourceHandle _handle)
    : Resource(_handle)
    , mTotalFrame(0.0f)
    , mFramePerSecond(0.0f)
{
    SetID("Animation : " + Helper::ToString(_handle.GetKey()));
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

void AnimationResource::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	std::string name = Helper::ToString(GetKey());

    switch (_viewerType)
    {
    case EditorViewerType::DEFAULT:
    {
        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
        auto flags = ImGuiSelectableFlags_AllowDoubleClick;
        if (ImGui::Selectable(GetID(), false, flags))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                EditorManager::GetInspectorViewer()->SetFocusObject(this);
            }
        }
        EditorItemState state;
        state.mResourcePtr = this;
        state.mName = name;
        EditorDragNDrop::SendDragAndDropData(GetID(), state);
        EDITOR_COLOR_POP(1);
        break;
    }
    case EditorViewerType::HIERARCHY:
    {
        break;
    }
    case EditorViewerType::INSPECTOR:
    {
        ImGui::Text("Total Frame : %f", mTotalFrame);
        ImGui::Text("Frame Per Second : %f", mFramePerSecond);
        ImGui::Separator();
        ImGui::Text("Channel Count : %d", mChannels.size());
        break;
    }
    default:
        break;
    }
}

AnimationNode::AnimationNode(std::wstring_view _name, AnimationResource* _pAnimResource)
    : mName(_name), mOwner(_pAnimResource)
{
}
AnimationNode::~AnimationNode()
{
}