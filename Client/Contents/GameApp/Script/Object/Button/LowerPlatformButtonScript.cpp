#include "pch.h"
#include "LowerPlatformButtonScript.h"

LowerPlatformButtonScript::LowerPlatformButtonScript(Object* _owner)
    : ButtonScript(_owner), platform(nullptr)
{
}

void LowerPlatformButtonScript::Start()
{
    gameObject->AddComponent<Rigidbody>();
    std::wstring currentTag = gameObject->GetTag();

    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(0, 1, 0));
        boxCol->SetExtents(Vector3(100, 1, 100));
    }

    else if (currentTag.find(L"_2") != std::wstring::npos)
    {
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(-10, 0, 20));
        boxCol->SetRotation(Vector3(0, 0, 0));
        boxCol->SetExtents(Vector3(130, 190, 30));
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
        Vector3 rotationAxis;
        if (abs(forward.y) > 0.9f)
        {
            rotationAxis = right;
        }
        else
        {
            rotationAxis = up;
        }

        // ������Ʈ�� z ����, �ݶ��̴��� y�� �������� ����.
        // �� �ù߷Ҿ�
        float rotationAngle = XM_PIDIV2; // 90�� ȸ�� 
        platformTransform->Rotate90(3.5f, rotationAxis, rotationAngle, Dotween::EasingEffect::OutBounce);
    }
}

//void LowerPlatformButtonScript::OnButtonPressed()
//{
//    Display::Console::Log("LowerPlatformButton Pressed!");
//
//    if (platform)
//    {
//        // �ڽĿ� �ִ� �ǹ� ������Ʈ ã��
//        Transform* platformTransform = platform->transform;
//        Object* pivotObject = nullptr;
//        auto children = platformTransform->GetChildren();
//        for (auto child : children)
//        {
//            if (child->gameObject->GetTag() == L"pivot")
//            {
//                pivotObject = child->gameObject;
//                break;
//            }
//        }
//
//        if (pivotObject)
//        {
//            // 1. pivot ������ �������� ȸ��
//            Vector3 pivotPoint = platformTransform->LocalToWorld(pivotObject->transform->position);
//
//            Vector3 forward = platformTransform->Forward();
//            Vector3 up = platformTransform->Up();
//            Vector3 right = platformTransform->Right();
//
//            // 2. ���� up ���͸� ����Ͽ� ȸ���� ����
//            Vector3 rotationAxis;
//            if (abs(forward.y) > 0.9f)
//            {
//                rotationAxis = right;
//            }
//            else
//            {
//                rotationAxis = up;
//            }
//
//            float rotationAngle = -     XM_PIDIV2; // 90�� ȸ�� 
//            platformTransform->RotateByPivot(pivotPoint, rotationAxis, rotationAngle, 3.5f, Dotween::EasingEffect::OutBounce);
//        }
//        else
//        {
//            Display::Console::Log("Pivot object not found!");
//        }
//    }
//}


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
