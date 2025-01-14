#pragma once

class AudioChannel;
class AudioGroup;

// �׷��� ����ϴ� Ŭ����. �׷��� ���� ������ �� �� �ִ�.
// ����Ʈ�� ���� �� ������ ������ ������
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