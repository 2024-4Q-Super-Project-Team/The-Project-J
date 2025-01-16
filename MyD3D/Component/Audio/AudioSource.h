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
    virtual void Start() override;
    virtual void Tick() override;
    virtual void FixedUpdate() override;
    virtual void PreUpdate() override;
    virtual void Update() override;
    virtual void PostUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Draw(Camera* _camera) override;
    virtual void PostRender() override;
public:
    void SetCurrentAudio(const std::wstring& _key);
    BOOL AddAudio(const std::wstring& _key, std::shared_ptr<AudioResource> _srcAudio);
    std::shared_ptr<AudioResource> GetAudioFromTable(const std::wstring& _key);
public:
    bool IsPlaying();
    bool IsPaused();
    void Play(std::wstring_view _key); // ActiveAudio를 바꾸고 Play
    void Play();
    void Pause();
public:
    // 루프 설정을 합니다.(왠만해선 Resource에서 설정할 것)
    void SetLoop(bool _isLoop);
    // 3D 입체 음향을 사용합니다
    void SetSurround(bool _isSuround);
public:
    virtual json Serialize() override { return json(); }
    virtual void Deserialize(json& j) override {}
private:
    // 현재 재생 예정, 혹은 재생 중인 오디오
    AudioResource* mActiveAudio; 
    // 해당 오디오 리소스를 재생해주는 공간
    AudioChannel* mAudioChannel;
    // 현재 등록된 오디오을 관리하는 테이블
    std::unordered_map<std::wstring, std::shared_ptr<AudioResource>> mAudioTable;
public:
	virtual void EditorRendering() override;
};