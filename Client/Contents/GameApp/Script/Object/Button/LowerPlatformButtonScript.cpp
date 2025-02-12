#include "pch.h"
#include "LowerPlatformButtonScript.h"
#include "LowerPlatformCollisionScript.h"

LowerPlatformButtonScript::LowerPlatformButtonScript(Object* _owner)
    : ButtonScript(_owner), platform(nullptr)
{
}

void LowerPlatformButtonScript::Start()
{
    auto& children = gameObject->transform->GetChildren();
    for (auto& child : children)
    {
        if (child->gameObject->GetName() == L"ButtonObject")
            mButtonObject = child->gameObject;
        if (child->gameObject->GetName() == L"LowerWallPlate")
            mLowerPlatformObject = child->gameObject;
    }

    {
        mButtonCollisionScript = mButtonObject->GetComponent<LowerPlatformCollisionScript>();
        if (mButtonCollisionScript == nullptr)
            mButtonCollisionScript = mButtonObject->AddComponent<LowerPlatformCollisionScript>();
        mButtonCollisionScript->SetOwnerScript(this);
    }
}

void LowerPlatformButtonScript::Update()
{
    if (!mRotated && isCollided)
    {
            Transform* tr = mLowerPlatformObject->transform;
            tr->RotateTo(2.f, Quaternion::CreateFromYawPitchRoll(-1.6f, 0, 0));
            mRotated = true;
    }  
}

void LowerPlatformButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
}

void _CALLBACK LowerPlatformButtonScript::OnCollisionEnterCallback(Rigidbody* _origin, Rigidbody* _destination)
{
    isCollided = true;
}

void LowerPlatformButtonScript::RotatePlatform()
{
    //if (mLowerPlatformObject)
    //{
    //    Transform* tr = mLowerPlatformObject->transform;
    //    tr->RotateTo(2.f, Quaternion::CreateFromYawPitchRoll(0, 0, 80));
    //}
}