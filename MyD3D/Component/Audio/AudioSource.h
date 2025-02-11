#pragma once
#include "Component/Component.h"
#include "Component/Renderer/Renderer.h"

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
    // Editor Only
    virtual void EditorUpdate() override;
    virtual void EditorGlobalUpdate() override;
    virtual void EditorRender() override;
public:
    void SetCurrentAudio(const std::wstring& _key);
    void SetCurrentAudio(const ResourceHandle& _handle);
    BOOL AddAudio(const std::wstring& _key, ResourceHandle _srcAudio);
public:
    bool IsPlaying();
    bool IsPaused();
    void Play(std::wstring_view _key); // ActiveAudio�� �ٲٰ� Play
    void Play();
    void Reset();
    void Pause();
    void Resume();
public:
    // ���� ������ �մϴ�.(�ظ��ؼ� Resource���� ������ ��)
    void SetLoop(bool _isLoop);
    // 3D ��ü ������ ����մϴ�
    void SetSurround(bool _isSuround);
public:
    virtual void _CALLBACK OnEnable() override;
    virtual void _CALLBACK OnDisable() override;
    virtual void _CALLBACK OnDestroy() override;
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j);
private:
    // ���� ��� ����, Ȥ�� ��� ���� �����a
    std::wstring    mActiveKey;
    ResourceHandle  mActiveHandle;
    AudioResource*  mActiveAudio; 
    // �ش� ����� ���ҽ��� ������ִ� ����
    AudioChannel*   mAudioChannel;
    // ���� ��ϵ� ������� �����ϴ� ���̺�
    std::unordered_map<std::wstring, ResourceHandle> mAudioTable;
public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;
    /////////////////////////////////////////////
    // Audio Popup
    /////////////////////////////////////////////
    ResourceHandle receiveHandle;
    bool isAddAudioPopup = false;
    void ShowAddAudioPopup();
};