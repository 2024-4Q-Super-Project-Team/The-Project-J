#include "pch.h"
#include "AudioResource.h"

AudioResource::AudioResource(ResourceHandle _handle)
	: Resource(_handle)
{
	mAudioClip = new AudioClip(_handle.GetPath());
	mAudioClip->Create();
}

AudioResource::~AudioResource()
{
	SAFE_RELEASE(mAudioClip);
}

void AudioResource::EditorRendering(EditorViewerType _viewerType)
{

}
