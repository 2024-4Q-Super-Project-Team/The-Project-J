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
	// 루프 설정을 합니다.
	void SetLoop(BOOL _useLoop);
	// 3D 입체 음향을 사용합니다. (왠만해선 사용X. 채널에서 설정 가능)
	void SetSoundMode(eAudioListenMode _soundMode);
public:
	inline AudioClip* GetAudioClip() { return mAudioClip; }
public:
	AudioClip*			mAudioClip;
	BOOL				mUseLoop;		// 루프 재생 여부
	eAudioListenMode	mListenMode;	// 사운드 모드
public:
	virtual json Serialize();
	virtual void Deserialize(json& j);
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};