#pragma once
#include "Component/Component.h"

class AudioResource;
class AudioChannel;

class AudioSource
	: public Component
{
public:
    explicit AudioSource(Object* _owner);
    virtual ~AudioSource();
public:
    virtual void Start() override {}
    virtual void Tick() override {}
    virtual void FixedUpdate() override {}
    virtual void PreUpdate() override {}
    virtual void Update() override {}
    virtual void PostUpdate() override {}
    virtual void PreRender() override {}
    virtual void Render() override {}
    virtual void Draw(Camera* _camera) override {}
    virtual void PostRender() override {}
public:
    void SetCurrentAudio(const std::wstring& _key);
    BOOL AddAudioFromTable(const std::wstring& _key, std::shared_ptr<AudioResource> _srcAudio);
    std::shared_ptr<AudioResource> GetAudioFromTable(const std::wstring& _key);
public:
    bool IsPlaying();
    bool IsPaused();
    // ActiveAudio�� �ٲٰ� Play
    void Play(std::wstring_view _key);
    void Play();
    void Pause();
public:
    virtual json Serialize() override;
    //virtual json Deserialize() override;
private:
    // ���� ��� ����, Ȥ�� ��� ���� �����
    AudioResource* mActiveAudio; 
    // �ش� ����� ���ҽ��� ������ִ� ����
    AudioChannel* mAudioChannel;
    // ���� ��ϵ� ������� �����ϴ� ���̺�
    std::unordered_map<std::wstring, std::shared_ptr<AudioResource>> mAudioTable;
};