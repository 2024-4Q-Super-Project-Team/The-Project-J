#include "pch.h"
#include "RaisePlatformButtonScript.h"

RaisePlatformButtonScript::RaisePlatformButtonScript(Object* _owner)
    : ButtonScript(_owner), wall(nullptr)
{
}

void RaisePlatformButtonScript::Start()
{
    AudioInit();

    Rigidbody* rb = gameObject->AddComponent<Rigidbody>();
    rb->SetIsRealStatic(false);
    std::wstring currentTag = gameObject->GetTag();

    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(0, 3, 0));
        boxCol->SetExtents(Vector3(100, 1, 100));
        buttonPosition = gameObject->transform->GetWorldPosition();
    }

    else if (currentTag.find(L"_2") != std::wstring::npos)
    {
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(0, 0, -20));
        boxCol->SetRotation(Vector3(0, 0, 0));
        boxCol->SetExtents(Vector3(190, 10, 130));
    }

    // 벽 찾기
    std::wstring wallTag = GetWallTag();
    auto objects = FindObjectsWithTag(wallTag);
    if (!objects.empty())
    {
        for (auto obj : objects)
        {
            if (obj != gameObject)
            {
                wall = obj;
                break;
            }
        }
    }

    auto boxes = FindObjectsWithTag(L"Box_02");
    if (!boxes.empty())
    {
        for (auto obj : boxes)
        {
            box = obj;
        }
    }
}

void RaisePlatformButtonScript::Update()
{
    if (box)
    {
        Vector3 box2Position = box->transform->GetWorldPosition();
        float distance = (box2Position - buttonPosition).Length();

        if (distance < activationDistance)
        {
            if (!isWithinDistance)
            {
                isWithinDistance = true;
                OnButtonPressed();
            }
        }

        else
        {
            if (isWithinDistance)
            {
                isWithinDistance = false;
                OnButtonReleased();
            }
        }
    }
}

void RaisePlatformButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
}

void RaisePlatformButtonScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
}

void RaisePlatformButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
}

void RaisePlatformButtonScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
}

void RaisePlatformButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
}

void RaisePlatformButtonScript::OnButtonPressed()
{
    if (wall)
    {
        Transform* platformTransform = wall->transform;
        Vector3 startPosition = platformTransform->position;
        Vector3 endPosition = startPosition;
        endPosition.y += 40.0f;
        platformTransform->MoveTo(endPosition, 2.0f, Dotween::EasingEffect::OutSine);
    }
}

void RaisePlatformButtonScript::OnButtonReleased()
{
    if (wall)
    {
        Transform* platformTransform = wall->transform;
        Vector3 startPosition = platformTransform->position;
        Vector3 endPosition = startPosition;
        endPosition.y += -40.0f;
        platformTransform->MoveTo(endPosition, 2.0f, Dotween::EasingEffect::OutSine);
    }
}

bool RaisePlatformButtonScript::CanInteract(Object* _object)
{
    return true;
}

std::wstring RaisePlatformButtonScript::GetWallTag()
{
    // 태그가 _1 로 끝나는 오브젝트의 태그를 _2로 바꿔서 반환
    // 연결된 벽을 _2로 끝나게 지으면 된다. (앞은 같게)
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