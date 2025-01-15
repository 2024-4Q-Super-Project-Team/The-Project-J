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

		Vector3 Forward = gameObject->transform->Forward();
		Vector3 Up = gameObject->transform->Up();

		// Listener의 위치, 전방, 상단을 설정합니다.
		memcpy(&listenerPosition, &gameObject->transform->GetWorldPosition(), sizeof(Vector3));
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
