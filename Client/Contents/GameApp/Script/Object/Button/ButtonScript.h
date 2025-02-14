#pragma once

class ButtonScript : public MonoBehaviour
{
public:
    explicit ButtonScript(Object* _owner);
    virtual ~ButtonScript() = default;

public:
    void AudioInit();
    void Start() override;
    virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;
	virtual void OnTriggerExit(Collider* _origin, Collider* _destination) override;

    virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) override;

    virtual void OnButtonPressed();
	virtual void OnButtonReleased();
    virtual void MoveButton(float yOffset, float duration);

public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
protected:
    SerializeField(bool, isUp, true);
    AudioSource* mAudioSource;
    //bool isPressed;
protected:
    virtual bool CanInteract(Object* _object); // 상호작용 할 수 있는지 판단 (1P, 2P, 물체 등)
    void PlayPressedSound();
};

