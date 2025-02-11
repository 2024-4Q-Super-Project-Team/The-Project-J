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
	SetCurrentAudio(mActiveHandle);
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
}

void AudioSource::EditorGlobalUpdate()
{
	for (auto& [key, handle] : mAudioTable)
	{
		gameObject->GetOwnerWorld()->
			mNeedResourceHandleTable.insert(handle.GetKey());
	}
}

void AudioSource::EditorRender()
{
}

void AudioSource::SetCurrentAudio(const std::wstring& _key)
{
	if (mActiveKey == _key)
		return;
	auto itr = mAudioTable.find(_key);
	if (FIND_SUCCESS(itr, mAudioTable))
	{
		mActiveKey = _key;
		SetCurrentAudio(itr->second);
	}
}

void AudioSource::SetCurrentAudio(const ResourceHandle& _handle)
{
	if (_handle.GetResourceType() == eResourceType::AudioResource)
	{
		auto pResource = ResourceManager::GetResource<AudioResource>(_handle);
		mActiveAudio = pResource;
		mActiveHandle = _handle;
	}
}

BOOL AudioSource::AddAudio(const std::wstring& _key, ResourceHandle _srcAudio)
{
	auto itr = mAudioTable.find(_key);
	if (FIND_FAILED(itr, mAudioTable))
	{
		mAudioTable[_key] = _srcAudio;
		// ActiveAudio�� ������ ���ǻ� �־��ش�.
		if (mActiveAudio == nullptr)
		{
			mActiveKey = _key;
			mActiveAudio = ResourceManager::GetResource<AudioResource>(_srcAudio);
		}
		return TRUE;
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
		Reset();
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Play(pAudioClip);
	}
}

void AudioSource::Reset()
{
	if (mActiveAudio)
	{
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Reset();
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

void AudioSource::Resume()
{
	if (mActiveAudio)
	{
		auto pAudioClip = mActiveAudio->GetAudioClip();
		mAudioChannel->Resume();
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
	ret["current audio key"] = Helper::ToString(mActiveKey);
	ret["current audio handle"] = mActiveHandle.Serialize();

	json tableJson = json::array(); // JSON �迭�� �ʱ�ȭ

	for (auto& audio : mAudioTable)
	{
		json entry;
		entry["key"] = Helper::ToString(audio.first);
		entry["handle"] = audio.second.Serialize();

		tableJson.push_back(entry); // �迭�� �߰�
	}

	ret["table"] = tableJson; // �迭�� ���� JSON ��ü�� �߰�

	return ret;
}

void AudioSource::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if(j.contains("current audio key"))
		mActiveKey = Helper::ToWString(j["current audio key"].get<std::string>());
	
	if(j.contains("current audio handle"))
		mActiveHandle.Deserialize(j["current audio handle"]);

	if (j.contains("table"))
	{
		json tableJson = j["table"];
		for (json& audioJson : tableJson)
		{
			if (audioJson.contains("key"))
			{
				std::wstring key = Helper::ToWString(audioJson["key"].get<std::string>());
				if (audioJson.contains("handle"))
				{
					ResourceHandle handle;
					handle.Deserialize(audioJson["handle"]);
					mAudioTable[key] = handle;
				}
			}
		}
	}
}

void AudioSource::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	{
		ImGui::Text("Current Audio");
		std::string widgetID = "NULL Audio";
		if (mActiveAudio)
		{
			mActiveAudio->EditorRendering(EditorViewerType::DEFAULT);
			widgetID = mActiveAudio->GetEID();
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
			ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
			EDITOR_COLOR_POP(1);
		}
	}
	ImGui::Separator();
	{
		ImGui::Text("Audio List");
		for (auto itr = mAudioTable.begin(); itr != mAudioTable.end();)
		{
			bool isDelete = false;
			const std::wstring& key = itr->first;
			const ResourceHandle& handle = itr->second;
			std::string str = Helper::ToString(key) + " : " + Helper::ToString(handle.GetKey());
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_RESOURCE);
			ImGui::Selectable((str).c_str(), false, ImGuiSelectableFlags_Highlight);
			EDITOR_COLOR_POP(1);
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup(("AudioPopup" + Helper::ToString(key)).c_str());
			}
			if (ImGui::BeginPopup(("AudioPopup" + Helper::ToString(key)).c_str()))
			{
				if (ImGui::MenuItem("Set Active Audio")) {
					SetCurrentAudio(itr->second);
				}
				if (ImGui::MenuItem("Delete Audio")) {
					isDelete = true;
					itr = mAudioTable.erase(itr);
				}
				ImGui::EndPopup();
			}
			if (!isDelete)
				++itr;
		}
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_ADDABLE);
		ImGui::Selectable("Add Audio", false, ImGuiSelectableFlags_Highlight);
		EDITOR_COLOR_POP(1);
		if (EditorDragNDrop::ReceiveDragAndDropResourceData<AudioResource>("Add Audio", &receiveHandle))
		{
			isAddAudioPopup = true;
		}
	}
	ImGui::Separator();
	{
		if (ImGui::Button("Play", ImVec2(50, 30)))
		{
			Play();
		}
		if (ImGui::Button("Pause", ImVec2(50, 30)))
		{
			Pause();
		}
		if (ImGui::Button("Resume", ImVec2(50, 30)))
		{
			Resume();
		}
		//if (ImGui::Button("Play", ImVec2(80, 50)))
		//{
		//	Play();
		//}
	}
	ShowAddAudioPopup();
}

void AudioSource::ShowAddAudioPopup()
{
	std::string id = "Add Audio";
	if (isAddAudioPopup)
	{
		ImGui::OpenPopup(id.c_str());
		isAddAudioPopup = false;
	}
	if (ImGui::BeginPopupModal(id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Key : ");
		static char Name[128] = "";
		ImGui::InputText("##AddAudioKey", Name, IM_ARRAYSIZE(Name));
		ImGui::Separator();

		ImGui::Text(std::string("Audio : " + Helper::ToString(receiveHandle.GetKey())).c_str());
		ImGui::Separator();

		const char* defaultName = "";
		if (ImGui::Button(("OK##" + id).c_str()) || Input::IsKeyDown(Key::ENTER))
		{
			std::wstring newName = Helper::ToWString(std::string(Name));
			AddAudio(newName, receiveHandle);
			ImGui::CloseCurrentPopup();
			strcpy_s(Name, defaultName);
		}
		ImGui::SameLine();
		if (ImGui::Button(("NO##" + id).c_str()) || Input::IsKeyDown(Key::ESCAPE))
		{
			ImGui::CloseCurrentPopup();
			strcpy_s(Name, defaultName);
		}
		ImGui::EndPopup();
	}
}
