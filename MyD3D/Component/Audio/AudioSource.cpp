#include "pch.h"
#include "AudioSource.h"
#include "Resource/AudioResource/AudioResource.h"
#include "World/World.h"

AudioSource::AudioSource(Object* _owner)
	: Component(_owner)
	, mActiveAudio(nullptr)
	, mAudioChannel(new AudioChannel)
{
	SetEID("AudioSource");
	mType = eComponentType::AUDIO_SOURCE;
}

AudioSource::~AudioSource()
{
}

void AudioSource::Start()
{
	auto itr = Helper::FindMap(mActiveKey, mAudioTable);
	if (itr)
	{
		mActiveAudio = ResourceManager::GetResource<AudioResource>(*itr);
	}
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

void AudioSource::EditorUpdate()
{
	auto itr = Helper::FindMap(mActiveKey, mAudioTable);
	if (itr)
	{
		mActiveAudio = ResourceManager::GetResource<AudioResource>(*itr);
	}
	else
	{
		mActiveAudio = nullptr;
	}
}

void AudioSource::EditorGlobalUpdate()
{
	for (auto& [key, handle] : mAudioTable)
	{
		gameObject->GetOwnerWorld()->
			mNeedResourceHandleTable.insert(handle.GetParentkey());
	}
}

void AudioSource::EditorRender()
{
}

void AudioSource::SetCurrentAudio(const std::wstring& _key)
{
	auto itr = Helper::FindMap(_key, mAudioTable);
	if (itr)
	{
		mActiveKey = _key;
		mActiveAudio = ResourceManager::GetResource<AudioResource>(*itr);
	}
}

BOOL AudioSource::AddAudio(const std::wstring& _key, ResourceHandle _srcAudio)
{
	auto itr = mAudioTable.find(_key);
	if (FIND_FAILED(itr, mAudioTable))
	{
		mAudioTable[_key] = _srcAudio;
		// ActiveAudio가 없으면 편의상 넣어준다.
		if (mActiveAudio == nullptr)
		{
			mActiveKey = _key;
			mActiveAudio = ResourceManager::GetResource<AudioResource>(_srcAudio);
		}
	}
	return FALSE;
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

void _CALLBACK AudioSource::OnEnable()
{
	Start();
	return void _CALLBACK();
}

void _CALLBACK AudioSource::OnDisable()
{
	mActiveAudio = nullptr;
	return void _CALLBACK();
}

void _CALLBACK AudioSource::OnDestroy()
{
	return void _CALLBACK();
}

json AudioSource::Serialize()
{
	json ret;

	ret["id"] = GetId();
	ret["name"] = "AudioSource";

	ret["active audio"] = Helper::ToString(mActiveKey);

	json tableJson;
	for (auto& audio : mAudioTable)
	{
		tableJson["key"] = audio.first;
		tableJson["handle"] = audio.second.Serialize();
	}

	ret["table"] += tableJson;

	return ret;
}

void AudioSource::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	mActiveKey = Helper::ToWString(j["active audio"].get<std::string>());

	json tableJson = j["table"];
	for (json& audioJson : tableJson)
	{
		std::wstring key = Helper::ToWString(audioJson["key"].get<std::string>());
		ResourceHandle handle;
		handle.Deserialize(audioJson["handle"]);

		mAudioTable[key] = handle;
	}

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
