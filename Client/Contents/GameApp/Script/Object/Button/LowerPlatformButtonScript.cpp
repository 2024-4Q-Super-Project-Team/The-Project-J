#include "pch.h"
#include "LowerPlatformButtonScript.h"

LowerPlatformButtonScript::LowerPlatformButtonScript(Object* _owner)
    : ButtonScript(_owner), platform(nullptr)
{
}

void LowerPlatformButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
    // �������ڰ� �������� �Ѿ�����
    Object* interactingObject = _destination->GetOwner();
    if (isUp.val && CanInteract(interactingObject))
    {
        isUp.val = false;
        OnButtonPressed();
    }
}

void LowerPlatformButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    

}

void LowerPlatformButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
}

void LowerPlatformButtonScript::OnButtonPressed()
{
    Display::Console::Log("LowerPlatformButton Pressed!");

    if (platform)
    {
        // ���� ������
    }
}

void LowerPlatformButtonScript::OnButtonReleased()
{
}

bool LowerPlatformButtonScript::CanInteract(Object* _object)
{
    return true;
}
