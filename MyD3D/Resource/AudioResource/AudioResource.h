#pragma once
#include "Resource/Resource.h"

class AudioClip;

class AudioResource
	: public Resource
{
public:
	RESOURCE_TYPE(AudioResource);
	explicit AudioResource(ResourceHandle _handle);
	virtual ~AudioResource();
public:
	inline AudioClip* GetAudioClip() { return mAudioClip; }
public:
	AudioClip* mAudioClip;
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};