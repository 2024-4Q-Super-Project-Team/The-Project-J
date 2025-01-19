#include "pch.h"
#include "AudioSource.h"
#include "Resource/AudioResource/AudioResource.h"

AudioSource::AudioSource(Object* _owner)
	: Component(_owner)
	, mActiveAudio(nullptr)
	, mAudioChannel(new AudioChannel)
{
}

AudioSource::~AudioSource()
{
}

void AudioSource::Start()
{
}

void AudioSource::Tick()
{
}

void AudioSource::FixedUpdate()
{
}

void AudioSource::PreUpdate()
{
}

void AudioSource::Update()
{
	if (mAudioChannel)
	{
		mAudioChannel->SetPosition(gameObject->transform->GetWorldPosition());
	}
}

void AudioSource::PostUpdate()
{
}

void AudioSource::PreRender()
{
}

void AudioSource::Render()
{
}

void AudioSource::Draw(Camera* _camera)
{
}

void AudioSource::PostRender()
{
}

void AudioSource::SetCurrentAudio(const std::wstring& _key)
{
	std::shared_ptr<AudioResource> pAudio = GetAudioFromTable(_key);
	if (pAudio)
	{
		mActiveAudio = pAudio.get();
	}
}

BOOL AudioSource::AddAudio(const std::wstring& _key, std::shared_ptr<AudioResource> _srcAudio)
{
	auto itr = mAudioTable.find(_key);
	if (FIND_FAILED(itr, mAudioTable))
	{
		mAudioTable[_key] = _srcAudio;
		// ActiveAudio가 없으면 편의상 넣어준다.
		if (mActiveAudio == nullptr)
		{
			mActiveAudio = _srcAudio.get();
		}
	}
	return FALSE;
}

std::shared_ptr<AudioResource> AudioSource::GetAudioFromTable(const std::wstring& _key)
{
	auto itr = Helper::FindMap(_key, mAudioTable);
	if (itr)
	{
		return *itr;
	}
	return nullptr;
}

bool AudioSource::IsPlaying()
{
	if (mAudioChannel)
	{
		return mAudioChannel->IsPlaying();
	}
}

bool AudioSource::IsPaused()
{
	if (mAudioChannel)
	{
		return mAudioChannel->IsPaused();
	}
}

void AudioSource::Play(std::wstring_view _key)
{
	SetCurrentAudio(_key.data());
	if (mActiveAudio)
	{
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Play(pAudioClip);
	}
}

void AudioSource::Play()
{
	if (mActiveAudio)
	{
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Play(pAudioClip);
	}
}

void AudioSource::Pause()
{
	if (mActiveAudio)
	{
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Pause();
	}
}

void AudioSource::SetLoop(bool _isLoop)
{
	mAudioChannel->SetLoop(_isLoop);
}

void AudioSource::SetSurround(bool _isSuround)
{
	mAudioChannel->SetSurround(_isSuround);
}

void AudioSource::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	if (ImGui::TreeNodeEx(("AudioSource" + uid).c_str(), EDITOR_FLAG_MAIN))
	{
		if (mActiveAudio)
		{
			ImGui::Text("Active Audio : %s", Helper::ToString(mActiveAudio->GetKey()).c_str());
		}
		else
		{
			ImGui::Text("Active Audio : NULL");
		}
		ImGui::Separator();
		// 콤보박스로 바꾸기
		ImGui::Text("Audio Table");
		for (auto& audio : mAudioTable)
		{
			ImGui::Text("%s", Helper::ToString(audio.first).c_str());
		}
		ImGui::TreePop();
	}
}
