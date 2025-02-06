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

    // 회전은 부모(pivot을 회전시킨다)
    // 자식에 딸린 Rigidbody와 Collider가 같이 회전해야 한다.
    // 자식에다 둘을 넣으면 왜 회전을 같이 안함??
    // 그럼 그냥 부모 회전축이 자식과 일치하지 않는거 아닌감... 잘 돌려봐..

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
        // 오브젝트는 y축 기준, collider는 z기준으로 돈다.
        Vector3 rotationAxis;
        if (abs(up.y) > 0.9f)
        {
            rotationAxis = right;
        }
        else
        {
            rotationAxis = forward;
        }

        float rotationAngle = XM_PIDIV2; // 90도 회전 
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
