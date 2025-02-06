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

    // 발판 찾기
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
    // 나무판자가 앞쪽으로 넘어진다
	Object* interactingObject = _destination->GetOwner();

    if (CanInteract(interactingObject))
    {
        if (isUp.val)
        {
            OnButtonPressed(); // 발판 내리기
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
        // 회전
        Transform* platformTransform = platform->transform;

        Vector3 forward = platformTransform->Forward();
        Vector3 up = platformTransform->Up();
        Vector3 right = platformTransform->Right();

        // 2. 발판 up 벡터를 사용하여 회전축 결정
        Vector3 rotationAxis;
        if (abs(forward.y) > 0.9f)
        {
            rotationAxis = right;
        }
        else
        {
            rotationAxis = up;
        }

        // 오브젝트는 z 기준, 콜라이더는 y축 기준으로 돈다.
        // 왜 시발롬아
        float rotationAngle = XM_PIDIV2; // 90도 회전 
        platformTransform->Rotate90(3.5f, rotationAxis, rotationAngle, Dotween::EasingEffect::OutBounce);
    }
}

//void LowerPlatformButtonScript::OnButtonPressed()
//{
//    Display::Console::Log("LowerPlatformButton Pressed!");
//
//    if (platform)
//    {
//        // 자식에 있는 피벗 오브젝트 찾고
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
//            // 1. pivot 포지션 기준으로 회전
//            Vector3 pivotPoint = platformTransform->LocalToWorld(pivotObject->transform->position);
//
//            Vector3 forward = platformTransform->Forward();
//            Vector3 up = platformTransform->Up();
//            Vector3 right = platformTransform->Right();
//
//            // 2. 발판 up 벡터를 사용하여 회전축 결정
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
//            float rotationAngle = -     XM_PIDIV2; // 90도 회전 
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
	// 태그가 _1 로 끝나는 오브젝트의 태그를 _2로 바꿔서 반환
    // 연결된 플랫폼을 _2로 끝나게 지으면 된다. (앞은 같게)
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
