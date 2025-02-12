#include "pch.h"
#include "RaiseGroundButtonScript.h"

RaiseGroundButtonScript::RaiseGroundButtonScript(Object* _owner)
    : ButtonScript(_owner)
{
}

void RaiseGroundButtonScript::Start()
{
    AudioInit();

    std::wstring currentTag = gameObject->GetTag();

    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        Rigidbody* rb = gameObject->AddComponent<Rigidbody>();
        rb->SetIsRealStatic(false);
        BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        boxCol->SetPosition(Vector3(0, 10, 0));
        boxCol->SetExtents(Vector3(100, 1, 100));
    }

    //else if (currentTag.find(L"_2") != std::wstring::npos)
    //{
    //    BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
    //    boxCol->SetPosition(Vector3(0, 0, -20));
    //    boxCol->SetRotation(Vector3(0, 0, 0));
    //    boxCol->SetExtents(Vector3(190, 10, 130));
    //}

    // 벽 찾기
    std::vector<std::wstring> wallTags = GetWallTags();
    for (const auto& wallTag : wallTags)
    {
        auto objects = FindObjectsWithTag(wallTag);
        if (!objects.empty())
        {
            for (auto obj : objects)
            {
                if (obj != gameObject)
                {
                    walls.push_back(obj);
                    originalPositions.push_back(obj->transform->position);
                }
            }
        }
    }
}

void RaiseGroundButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
}

void RaiseGroundButtonScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
}

void RaiseGroundButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    Display::Console::Log("Enter_RaiseGround");
    PlayPressedSound();
    Object* interactingObject = _destination->GetOwner();
    if (CanInteract(interactingObject))
    {
        isUp.val = false;
        OnButtonPressed();
    }
}

void RaiseGroundButtonScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
    Object* interactingObject = _destination->GetOwner();
    if (CanInteract(interactingObject))
    {
        isUp.val = false;
    }
}

void RaiseGroundButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
    Display::Console::Log("Exit_LowerGround");
    Object* interactingObject = _destination->GetOwner();
    if (CanInteract(interactingObject))
    {
        isUp.val = true;
        OnButtonReleased();
    }
}

void RaiseGroundButtonScript::OnButtonPressed()
{
    for (size_t i = 0; i < walls.size(); ++i)
    {
        Object* wall = walls[i];
        if (wall)
        {
            Transform* platformTransform = wall->transform;
            Vector3 endPosition = originalPositions[i];
            endPosition.y += 500.0f;
            platformTransform->MoveTo(endPosition, 2.0f, Dotween::EasingEffect::OutSine);
        }
    }
}

void RaiseGroundButtonScript::OnButtonReleased()
{
    for (size_t i = 0; i < walls.size(); ++i)
    {
        Object* wall = walls[i];
        if (wall)
        {
            Transform* platformTransform = wall->transform;
            platformTransform->MoveTo(originalPositions[i], 2.0f, Dotween::EasingEffect::OutSine);
        }
    }
}

bool RaiseGroundButtonScript::CanInteract(Object* _object)
{
    return true;
}

std::vector<std::wstring> RaiseGroundButtonScript::GetWallTags()
{
    // 태그가 _1 로 끝나는 오브젝트의 태그를 _2와 _3로 바꿔서 반환
    std::vector<std::wstring> wallTags;
    std::wstring currentTag = gameObject->GetTag();
    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        std::wstring tag2 = currentTag;
        std::wstring tag3 = currentTag;
        wallTags.push_back(tag2.replace(tag2.find(L"_1"), 2, L"_2"));
        wallTags.push_back(tag3.replace(tag3.find(L"_1"), 2, L"_3"));
    }
    return wallTags;
}