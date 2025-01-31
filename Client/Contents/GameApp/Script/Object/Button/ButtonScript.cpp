#include "pch.h"
#include "ButtonScript.h"

void ButtonScript::Start()
{
    isPressed = false;
}

void ButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    if (!isPressed)
    {
        isPressed = true;
        OnButtonPressed();
    }
}



void ButtonScript::OnButtonPressed()
{
}
