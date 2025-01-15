#include "pch.h"
#include "AudioListener.h"

AudioListener* AudioListener::mMainListener = nullptr;

AudioListener::AudioListener(Object* _owner)
	: Component(_owner)
{
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

		memcpy(&listenerPosition, &gameObject->transform->GetWorldPosition(), sizeof(Vector3));
		memcpy(&listenerForward, &gameObject->transform->Forward(), sizeof(Vector3));
		memcpy(&listenerUp, &gameObject->transform->Up(), sizeof(Vector3));

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
