#include "pch.h"
#include "LowerWallButtonScript.h"

#define PLAYER_SFX_WALL                 L"SFX_wall_down.mp3"

LowerWallButtonScript::LowerWallButtonScript(Object* _owner)
    : ButtonScript(_owner), wall(nullptr)
{
}

void LowerWallButtonScript::Start()
{
    AudioInit();

    gameObject->AddComponent<Rigidbody>();
    std::wstring currentTag = gameObject->GetTag();

    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(0, 5, 0));
        boxCol->SetExtents(Vector3(100, 7, 100));
        buttonPosition = gameObject->transform->GetWorldPosition();
    }

    else if (currentTag.find(L"_2") != std::wstring::npos)
    {
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(0, 0, -20));
        boxCol->SetRotation(Vector3(0, 0, 0));
        boxCol->SetExtents(Vector3(30, 300, 1000));
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

    auto boxes = FindObjectsWithTag(L"Box_01");
    if (!boxes.empty())
    {
        for (auto obj : boxes)
        {
            box = obj;
        }
    }
}

void LowerWallButtonScript::Update()
{
    if (box)
    {
        Vector3 box1Position = box->transform->GetWorldPosition();
        float distance = (box1Position - buttonPosition).Length();

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

void LowerWallButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
    PlayPressedSound();
    Display::Console::Log("EnterTrigger_LowerWall");

}

void LowerWallButtonScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
}

void LowerWallButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
}

void LowerWallButtonScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
}

void LowerWallButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
}

void LowerWallButtonScript::OnButtonPressed()
{
    if (wall)
    {
        Transform* platformTransform = wall->transform;
        Vector3 startPosition = platformTransform->position;
        Vector3 endPosition = startPosition;
        endPosition.y += -500.0f;
        platformTransform->MoveTo(endPosition, 2.0f, Dotween::EasingEffect::OutSine);
    }
}

void LowerWallButtonScript::OnButtonReleased()
{
    if (wall)
    {
        Transform* platformTransform = wall->transform;
        Vector3 startPosition = platformTransform->position;
        Vector3 endPosition = startPosition;
        endPosition.y += 500.0f;
        platformTransform->MoveTo(endPosition, 2.0f, Dotween::EasingEffect::OutSine);
    }
}

bool LowerWallButtonScript::CanInteract(Object* _object)
{
    return true;
}

std::wstring LowerWallButtonScript::GetWallTag()
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