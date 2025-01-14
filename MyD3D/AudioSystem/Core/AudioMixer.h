#pragma once

class AudioChannel;
class AudioGroup;

// 그룹을 통괄하는 클래스. 그룹의 볼륨 조절을 할 수 있다.
// 이펙트도 넣을 수 있으면 넣으면 좋을듯
class AudioMixer
{
public:
	explicit AudioMixer() = default;
	virtual ~AudioMixer() = default;
public:
	static BOOL Initialize();
	static void Finalization();
public:
	static AudioGroup* GetMasterGroup();
	static AudioGroup* AddGroup(const std::wstring& _wGroupName);
	static AudioGroup* GetGroup(const std::wstring& _wGroupName);
private:
	static AudioGroup* mMasterGroup;
};