#include "pch.h"
#include "AudioResource.h"

AudioResource::AudioResource(ResourceHandle _handle)
	: Resource(_handle)
{
    SetEID("Audio : " + Helper::ToString(_handle.GetKey()));
	mAudioClip = new AudioClip(_handle.GetPath());
	mAudioClip->Create();
	// JSON_TODO : 핸들의 Path경로에 Json파일이 있는지 확인
	// 있으면 해당 JSON의 값을 쓰고
	// 없으면 JSON을 만들고 기본 값 사용

    SetLoop(mUseLoop);
    SetSoundMode(mListenMode);
}

AudioResource::~AudioResource()
{
	SAFE_RELEASE(mAudioClip);
}

void AudioResource::SetLoop(BOOL _useLoop)
{
	if (mAudioClip)
	{
		mAudioClip->SetLoop(_useLoop);
        mUseLoop = _useLoop;
	}
}

void AudioResource::SetSoundMode(eAudioListenMode _soundMode)
{
	if (mAudioClip)
	{
		mAudioClip->SetSoundMode(_soundMode);
		mListenMode = _soundMode;
	}
}

void AudioResource::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    std::string name = Helper::ToString(GetKey());

    switch (_viewerType)
    {
    case EditorViewerType::DEFAULT:
    {
        ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_RESOURCE);
        auto flags = ImGuiSelectableFlags_AllowDoubleClick;
        if (ImGui::Selectable(GetEID(), false, flags))
        {
            if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                EditorManager::GetInspectorViewer()->SetFocusObject(this);
            }
        }
        EditorItemState state;
        state.mResourcePtr = this;
        state.mName = name;
        EditorDragNDrop::SendDragAndDropData(GetEID(), state);
        EDITOR_COLOR_POP(1);
        break;
    }
    case EditorViewerType::HIERARCHY:
    {
        break;
    }
    case EditorViewerType::INSPECTOR:
    {
        Resource::EditorRendering(_viewerType);
        // EDITOR_TODO : 사운드 관련 정보 출력
        break;
    }
    default:
        break;
    }
}
