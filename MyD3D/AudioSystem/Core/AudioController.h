#pragma once

class AudioChannel;
class AudioGroup;

// 채널을 들고, 풀처럼 이용해 채널 요청이 들어오면 채널을 주는 클래스
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

