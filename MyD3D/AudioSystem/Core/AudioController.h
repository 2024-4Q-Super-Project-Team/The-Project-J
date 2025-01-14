#pragma once

class AudioChannel;
class AudioGroup;

// ä���� ���, Ǯó�� �̿��� ä�� ��û�� ������ ä���� �ִ� Ŭ����
class AudioController
{
public:
	explicit AudioController();
	virtual ~AudioController();
public:
	static BOOL Initialize();
	static void Finalization();
public:
	static HRESULT QueryChannel(OUT AudioChannel** _ppChannel);
private:
	static INT mNumChannel;
	static std::vector<AudioChannel*>			mAllChannels;
	static std::unordered_set<AudioChannel*> 	mActiveChannels;
	static std::queue<AudioChannel*>			mAvailableChannels;
};

