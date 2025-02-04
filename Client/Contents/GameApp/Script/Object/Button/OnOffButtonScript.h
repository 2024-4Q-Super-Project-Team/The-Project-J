#pragma once
#include "ButtonScript.h"

class OnOffButtonScript : public ButtonScript
{
public:
	void Start() override;
public:
    explicit OnOffButtonScript(Object* _owner);
    virtual ~OnOffButtonScript() = default;
public:
	virtual void OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination) override;
    virtual void OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination) override;

    virtual void OnButtonPressed() override;
	virtual void OnButtonReleased() override;
public:
	void SetOtherButton(OnOffButtonScript* _otherButton) { otherButton = _otherButton; }

private:
    bool isProcessing;

protected:
    virtual bool CanInteract(Object* _object) override; // 상호작용 할 수 있는지 판단 (1P, 2P, 물체 등)
private:
    OnOffButtonScript* otherButton;
    std::wstring GetOtherButtonTag();
    void MoveButton(float yOffset, float duration);

};