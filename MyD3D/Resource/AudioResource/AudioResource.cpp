#include "pch.h"
#include "AudioResource.h"

AudioResource::AudioResource(std::wstring_view _path)
	: Resource(_path)
{
	mAudioClip = new AudioClip(_path.data());
	mAudioClip->Create();
}

AudioResource::~AudioResource()
{
	SAFE_RELEASE(mAudioClip);
}

void AudioResource::EditorRendering()
{

}
