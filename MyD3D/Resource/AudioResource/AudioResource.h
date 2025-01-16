#pragma once
#include "Resource/Resource.h"

class AudioClip;

class AudioResource
	: public Resource
{
public:
	RESOURCE_TYPE(AudioResource);
	explicit AudioResource(std::wstring_view _path);
	virtual ~AudioResource();
public:
	inline AudioClip* GetAudioClip() { return mAudioClip; }
public:
	AudioClip* mAudioClip;
public:
	virtual void EditorRendering() override;
};