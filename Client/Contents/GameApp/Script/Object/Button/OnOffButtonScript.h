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
	virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;
    virtual void OnTriggerExit(Collider* _origin, Collider* _destination) override;

    virtual void OnButtonPressed() override;
	virtual void OnButtonReleased() override;

public:
	void SetOtherButton(OnOffButtonScript* _otherButton) { otherButton = _otherButton; }

private:
    bool isProcessing;

protected:
    virtual bool CanInteract(Object* _object) override; // ��ȣ�ۿ� �� �� �ִ��� �Ǵ� (1P, 2P, ��ü ��)
private:
    OnOffButtonScript* otherButton;
    std::wstring GetOtherButtonTag();
};