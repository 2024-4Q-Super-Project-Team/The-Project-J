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
	// ���� ������ �մϴ�.
	void SetLoop(BOOL _useLoop);
	// 3D ��ü ������ ����մϴ�. (�ظ��ؼ� ���X. ä�ο��� ���� ����)
	void SetSoundMode(eAudioListenMode _soundMode);
public:
	inline AudioClip* GetAudioClip() { return mAudioClip; }
public:
	AudioClip*			mAudioClip;
	BOOL				mUseLoop;		// ���� ��� ����
	eAudioListenMode	mListenMode;	// ���� ���
public:
	virtual json Serialize();
	virtual void Deserialize(json& j);
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
};