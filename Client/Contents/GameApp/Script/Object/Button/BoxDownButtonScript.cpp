#include "pch.h"
#include "BoxDownButtonScript.h"

#define DOOR_ANIM_LEFT L"01"
#define DOOR_ANIM_RIGHT L"04"


BoxDownButtonScript::BoxDownButtonScript(Object* _owner)
    : ButtonScript(_owner), door1(nullptr), door2(nullptr)
{
}

void BoxDownButtonScript::Start()
{
    std::wstring currentTag = gameObject->GetTag();

    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        //gameObject->AddComponent<Rigidbody>();
        //BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
        //boxCol->SetPosition(Vector3(0, 10, 0));
        //boxCol->SetExtents(Vector3(100, 1, 100));
    }

    auto objects = FindObjectsWithTag(L"Stage_wall");
    if (!objects.empty())
    {
        for (auto obj : objects)
        {
             stageWall = obj;
        }
    }

    Transform* stageWallTrasnform = stageWall->transform;
    auto& Children = stageWallTrasnform->GetChildren();
    for (Transform* child : Children)
    {
        if (child->gameObject->GetName() == L"Stage_wall_door_L")
            door1 = child->gameObject;
        if (child->gameObject->GetName() == L"Stage_wall_door_R")
            door2 = child->gameObject;
    }

    if (door1 == nullptr) Helper::HRT(E_FAIL, "Door1 is nullptr");
    if (door2 == nullptr) Helper::HRT(E_FAIL, "Door2 is nullptr");

    mLeftDoorAnimator = door1->GetComponent<Animator>();
    mRightDoorAnimator = door2->GetComponent<Animator>();

    mLeftDoorAnimator->SetCurrentAnimation(DOOR_ANIM_LEFT);
    mRightDoorAnimator->SetCurrentAnimation(DOOR_ANIM_RIGHT);

    mLeftDoorAnimator->Stop();
    mRightDoorAnimator->Stop();

   
    //// 문 찾기
    //std::wstring door1Tag = GetDoorTag(2);
    //std::wstring door2Tag = GetDoorTag(3);
    //auto objects1 = FindObjectsWithTag(door1Tag);
    //auto objects2 = FindObjectsWithTag(door2Tag);
    //if (!objects1.empty())
    //{
    //    door1 = objects1[0];
    //}
    //if (!objects2.empty())
    //{
    //    door2 = objects2[0];
    //}

    //mLeftDoorAnimator = 
}

void BoxDownButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
}

void BoxDownButtonScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
}

void BoxDownButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    Display::Console::Log("Collision Enter");
    
    mLeftDoorAnimator->SetReverse(false);
    mRightDoorAnimator->SetReverse(false);

    mLeftDoorAnimator->Play();
    mLeftDoorAnimator->SetLoop(false);
    mRightDoorAnimator->Play();
    mRightDoorAnimator->SetLoop(false);

}

void BoxDownButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
    Display::Console::Log("Collision Exit");

    // 역재생

    mLeftDoorAnimator->SetReverse(true);
    mRightDoorAnimator->SetReverse(true);

    // Duration을 마지막 프레임으로 설정
    float leftTotalFrame = mLeftDoorAnimator->GetActiveAnimationResource()->GetTotalFrame();
    float rightTotalFrame = mRightDoorAnimator->GetActiveAnimationResource()->GetTotalFrame();

    mLeftDoorAnimator->SetFrame(leftTotalFrame);
    mRightDoorAnimator->SetFrame(rightTotalFrame);
    mLeftDoorAnimator->Resume();
    mRightDoorAnimator->Resume();
    mLeftDoorAnimator->SetLoop(false);
    mRightDoorAnimator->SetLoop(false);
}

void BoxDownButtonScript::OnButtonPressed()
{
    Display::Console::Log("BoxButtonDown Pressed!");

    if (door1)
    {
        RotateDoor(door1, XM_PIDIV2); // 90도 회전
    }
    if (door2)
    {
        RotateDoor(door2, -XM_PIDIV2); // -90도 회전
    }
}

void BoxDownButtonScript::OnButtonReleased()
{
    Display::Console::Log("BoxButtonDown Released!");

    if (door1)
    {
        RotateDoor(door1, -XM_PIDIV2); // -90도 회전
    }
    if (door2)
    {
        RotateDoor(door2, XM_PIDIV2); // 90도 회전
    }
}

std::wstring BoxDownButtonScript::GetDoorTag(int doorNumber)
{
    std::wstring currentTag = gameObject->GetTag();
    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        return currentTag.replace(currentTag.find(L"_1"), 2, L"_" + std::to_wstring(doorNumber));
    }
    return L"";
}

void BoxDownButtonScript::RotateDoor(Object* door, float angle)
{
    Transform* doorTransform = door->transform;
    Object* pivotObject = nullptr;
    auto children = doorTransform->GetChildren();
    for (auto child : children)
    {
        if (child->gameObject->GetTag() == L"pivot")
        {
            pivotObject = child->gameObject;
            break;
        }
    }

    if (pivotObject)
    {
        Vector3 pivotPoint = doorTransform->LocalToWorld(pivotObject->transform->position);
        // pivotPoint = Vector3(pivotPoint.z, pivotPoint.y, pivotPoint.x);

        Vector3 forward = doorTransform->Forward();
        Vector3 up = doorTransform->Up();
        Vector3 right = doorTransform->Right();

        Vector3 rotationAxis = up;
        //if (abs(forward.y) > 0.9f)
        //{
        //    rotationAxis = right;
        //}
        //else
        //{
        //    rotationAxis = up;
        //}

        doorTransform->RotateByPivot(pivotPoint, Vector3(0,1,0), angle, 3.5f, Dotween::EasingEffect::OutBounce);
    }
    else
    {
        Display::Console::Log("Pivot object not found!");
    }
}