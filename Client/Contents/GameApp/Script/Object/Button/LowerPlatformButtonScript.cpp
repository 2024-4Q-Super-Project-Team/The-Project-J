#include "pch.h"
#include "LowerPlatformButtonScript.h"

LowerPlatformButtonScript::LowerPlatformButtonScript(Object* _owner)
    : ButtonScript(_owner), platform(nullptr)
{
}

void LowerPlatformButtonScript::Start()
{
    // ���� ã��
    std::wstring platformTag = GetPlatformTag();
    auto objects = FindObjectsWithTag(platformTag);
    if (!objects.empty())
    {
        for (auto obj : objects)
        {
            platform = obj;
        }
    }
}

void LowerPlatformButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
    
}

void LowerPlatformButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    // �������ڰ� �������� �Ѿ�����
	Object* interactingObject = _destination->GetOwner();
    if (CanInteract(interactingObject))
    {
        if (isUp.val)
        {
            OnButtonPressed(); // ���� ������
            isUp.val = false;
        }
    }


}

void LowerPlatformButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
}

void LowerPlatformButtonScript::OnButtonPressed()
{
    Display::Console::Log("LowerPlatformButton Pressed!");

    if (platform)
    {
        // ���� ������, Dotween Ȱ��
        

    }
}

void LowerPlatformButtonScript::OnButtonReleased()
{
}

bool LowerPlatformButtonScript::CanInteract(Object* _object)
{
    return true;
}

std::wstring LowerPlatformButtonScript::GetPlatformTag()
{
	// �±װ� _1 �� ������ ������Ʈ�� �±׸� _2�� �ٲ㼭 ��ȯ
    // ����� �÷����� _2�� ������ ������ �ȴ�. (���� ����)
    std::wstring currentTag = gameObject->GetTag();
    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        return currentTag.replace(currentTag.find(L"_1"), 2, L"_2");
    }
    else if (currentTag.find(L"_2") != std::wstring::npos)
    {
        return currentTag.replace(currentTag.find(L"_2"), 2, L"_1");
    }
    return L""; return std::wstring();
}
