#pragma once
#include "AudioSystem/Interface/IAudio.h"

class AudioGroup;

enum class eAudioListenMode
{
	DEFAULT,
	SOUND_2D,
	SOUND_3D,
};

class AudioClip : public IAudioResource
{
public:
	explicit AudioClip(std::wstring _wPath, bool _isLoop = true);
	virtual ~AudioClip();
public:
	virtual HRESULT Create() override;
	virtual void	Release() override;
public:
	// ����� �׷��� �����մϴ�.
	void SetGroup(AudioGroup* _pGroup);
	// ���� ������ �մϴ�.
	void SetLoop(BOOL _isLoop);
	// 3D ��ü ������ ����մϴ�. (�ظ��ؼ� ���X. ä�ο��� ���� ����)
	void SetSoundMode(eAudioListenMode _soundMode);
public:
	std::wstring		mPath;
	AudioGroup*			mGroup;
	FMOD::Sound*		mSound;
public:
	inline auto GetSound() { return mSound; }
	inline auto GetGroup() { return mGroup; }
};

