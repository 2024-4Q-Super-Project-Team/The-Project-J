#pragma once
#include "ButtonScript.h"

class RaiseOtherButtonScript : public ButtonScript
{
public:
    _MONOBEHAVIOUR_INIT(RaiseOtherButtonScript);
public:
    virtual void OnButtonPressed() override;
private:
    SerializeField(Object*, otherButton);
};