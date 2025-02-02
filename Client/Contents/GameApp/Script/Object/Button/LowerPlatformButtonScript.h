#pragma once
#include "ButtonScript.h"

class LowerPlatformButtonScript : public ButtonScript
{
public:
    _MONOBEHAVIOUR_INIT(LowerPlatformButtonScript);
public:
    virtual void OnButtonPressed() override;
private:
    SerializeField(Object*, platform); // 발판 오브젝트
};