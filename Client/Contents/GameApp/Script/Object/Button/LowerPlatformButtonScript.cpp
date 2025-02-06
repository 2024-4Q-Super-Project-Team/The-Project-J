#include "pch.h"
#include "LowerPlatformButtonScript.h"

LowerPlatformButtonScript::LowerPlatformButtonScript(Object* _owner)
    : ButtonScript(_owner), platform(nullptr)
{
}

void LowerPlatformButtonScript::Start()
{
    Transform* platformTransform = gameObject->transform;
    auto child = platformTransform->GetChild();

    std::wstring currentTag = gameObject->GetTag();

    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        gameObject->AddComponent<Rigidbody>();
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(0, 1, 0));
        boxCol->SetExtents(Vector3(100, 1, 100));
    }

    // ȸ���� �θ�(pivot�� ȸ����Ų��)
    // �ڽĿ� ���� Rigidbody�� Collider�� ���� ȸ���ؾ� �Ѵ�.
    // �ڽĿ��� ���� ������ �� ȸ���� ���� ����??
    // �׷� �׳� �θ� ȸ������ �ڽİ� ��ġ���� �ʴ°� �ƴѰ�... �� ������..

    else if (currentTag.find(L"_2") != std::wstring::npos)
    {
        //child->gameObject->AddComponent<Rigidbody>();
        //BoxCollider* boxCol = child->gameObject->AddComponent<BoxCollider>();
        //boxCol->SetPosition(Vector3(0.0f, 0.0f, 20.0f));
        //boxCol->SetExtents(Vector3(200.0f, 135.0f, 20.0f));

        gameObject->AddComponent<Rigidbody>();
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(0, 120, 30));
        boxCol->SetExtents(Vector3(130, 200, 20));
    }

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
        // ȸ��
        Transform* platformTransform = platform->transform;

        Vector3 forward = platformTransform->Forward();
        Vector3 up = platformTransform->Up();
        Vector3 right = platformTransform->Right();

        // 2. ���� up ���͸� ����Ͽ� ȸ���� ����
        // ������Ʈ�� y�� ����, collider�� z�������� ����.
        Vector3 rotationAxis;
        if (abs(up.y) > 0.9f)
        {
            rotationAxis = right;
        }
        else
        {
            rotationAxis = forward;
        }

        float rotationAngle = XM_PIDIV2; // 90�� ȸ�� 
        platformTransform->Rotate90(3.5f, rotationAxis, rotationAngle, Dotween::EasingEffect::OutBounce);
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
