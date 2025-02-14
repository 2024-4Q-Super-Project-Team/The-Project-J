#include "pch.h"
#include "AudioListener.h"

AudioListener* AudioListener::mMainListener = nullptr;

AudioListener::AudioListener(Object* _owner)
	: Component(_owner)
{
	SetEID("AudioListener");
	mType = eComponentType::AUDIO_LISTENER;
}

AudioListener::~AudioListener()
{
}

void AudioListener::Start()
{
}

void AudioListener::Tick()
{
	if (mMainListener == nullptr)
	{
		mMainListener = this;

		FMOD_VECTOR listenerPosition = { 0,0,0 };
		FMOD_VECTOR listenerVelocity = { 0,0,0 };
		FMOD_VECTOR listenerForward = { 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR listenerUp = { 0.0f, 1.0f, 0.0f };

		Vector3 Position = gameObject->transform->GetWorldPosition();
		Vector3 Forward = gameObject->transform->Forward();
		Vector3 Up = gameObject->transform->Up();

		// Listener의 위치, 전방, 상단을 설정합니다.
		memcpy(&listenerPosition, &Position, sizeof(Vector3));
		memcpy(&listenerForward, &Forward, sizeof(Vector3));
		memcpy(&listenerUp, &Up, sizeof(Vector3));

		FMOD_CHECK(AudioHub::GetCoreSystem()->set3DListenerAttributes(
			0,
			&listenerPosition,
			&listenerVelocity,
			&listenerForward,
			&listenerUp)
		);
	}
}

void AudioListener::FixedUpdate()
{
}

void AudioListener::PreUpdate()
{
}

void AudioListener::Update()
{
}

void AudioListener::PostUpdate()
{
}

void AudioListener::PreRender()
{
}

void AudioListener::Render()
{
}

void AudioListener::Draw(Camera* _camera)
{
}

void AudioListener::PostRender()
{
	mMainListener = nullptr;
}

void AudioListener::EditorUpdate()
{
}

void AudioListener::EditorRender()
{
}

json AudioListener::Serialize()
{
	json ret;
	ret["id"] = GetId();
	ret["name"] = "AudioListener";
	return json();
}

void AudioListener::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());
}

void AudioListener::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	ImGui::Text("Distance Attenuation Scale : ");
	ImGui::DragFloat((uid + "Distance Attenuation Scale").c_str(), &mDistanceAttenuationScale, 0.01f, 0.0f, 1.0f);
}
