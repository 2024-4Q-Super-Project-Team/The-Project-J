#include "pch.h"
#include "PlayerScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"

#define PLAYER_ANIM_IDLE L"003"
#define PLAYER_ANIM_WALK L"004"
#define PLAYER_ANIM_JUMP L"005"
#define PLAYER_ANIM_MOVE_FIRE_RIGHT L"006"
#define PLAYER_ANIM_MOVE_FIRE_LEFT L"007"
#define PLAYER_ANIM_OFF_FIRE L"008"
#define PLAYER_ANIM_HIT L"009"
#define PLAYER_ANIM_DEAD L"010"

void PlayerScript::Start()
{
    // �ʱ�ȭ �ڵ�
    gameObject->SetTag(L"Player");

    {
        Object* RootObject = gameObject->transform->GetParent()->gameObject;
        auto& Children = RootObject->transform->GetChildren();
        for (Transform* child : Children)
        {
            if (child->gameObject->GetName() == L"Player_Body")
                mBodyObject = child->gameObject;
            if (child->gameObject->GetName() == L"Player_Candle")
                mCandleObject = child->gameObject;
            if (child->gameObject->GetName() == L"Player_Fire")
                mFireObject = child->gameObject;
        }
        if (mBodyObject == nullptr) Helper::HRT(E_FAIL, "Player_Body Object is nullptr");
        if (mCandleObject == nullptr) Helper::HRT(E_FAIL, "Player_Candle Object is nullptr");
        if (mFireObject == nullptr) Helper::HRT(E_FAIL, "Player_Fire Object is nullptr");

        // Object -> RootNode -> Amature -> Bone -> TopBone
        mCandleTopBone = mCandleObject->transform->GetChild()->GetChild()->GetChild()->GetChild();
    }
    {
        mBodyAnimator = mBodyObject->GetComponent<Animator>();
        mCandleAnimator = mCandleObject->GetComponent<Animator>();
    }
    {   // PlayerController������Ʈ ����, ������ �߰�
        mPlayerController = gameObject->GetComponent<PlayerController>();
        if (mPlayerController == nullptr)
            mPlayerController = gameObject->AddComponent<PlayerController>();
    }
    {   // BurnObjectScript�߰�
        mBurnObjectScript = mBodyObject->AddComponent<BurnObjectScript>();
        mBurnObjectScript->SetBurnMesh(mFireObject->GetComponent<MeshRenderer>());
    }
    InitFireLight();
}

void PlayerScript::Update()
{
    UpdatePlayerHP();

    // FSMUpdate
    switch (mPlayerState)
    {
    case ePlayerStateType::IDLE:
        UpdateIdle();
        break;
    case ePlayerStateType::MOVE:
        UpdateMove();
        break;
    case ePlayerStateType::JUMP:
        UpdateJump();
        break;
    case ePlayerStateType::MOVE_FIRE:
        break;
    case ePlayerStateType::DEAD:
        UpdateDead();
        break;
    default:
        break;
    }

    mFireObject->transform->position = mCandleTopBone->GetWorldPosition();
}

void PlayerScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{

}

void PlayerScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
    ////////////////////////////////////////////////
    // �� �ű�� �ý���
    // [02/02 ~ ] �۾��� : ���� (�⺻ ����)
    ////////////////////////////////////////////////
    // ���� �پ����� ��
    if (mBurnObjectScript->IsBurning())
    {
        // BurnObjectScript�� GetComponent�����߳ķ� ����� ���� �ű� �� �ִ� ������Ʈ �ΰ��� ����
        BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
        // dstBurnObject�� �°�, dstBurnObject�� ���� �پ����� �ʴ� ���
        if (dstBurnObject && dstBurnObject->IsBurning() == false)
        {
            // JH TODO : ĳ����(gameO-bject)�� �Ӹ� ���� �� �ű�� UI �˾�

            // �� �ű�� Ű�� ������ ��
            if (InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
            {
                // BurnObjectScript�� �ҿű�� �۾��� �Ѵ�.
                if (dstBurnObject->IsProcessing() == false)
                {
                    dstBurnObject->ProcessBurn(mBurnObjectScript);
                    mPlayerState = ePlayerStateType::MOVE_FIRE;
                }
            }
            else  if (InputSyncer::IsKeyUp(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
            {
                // BurnObjectScript�� �ҿű�� �۾��� ��ҽ�Ų��.
                if (dstBurnObject->IsProcessing() == true &&
                    dstBurnObject->GetDestObject() == mBurnObjectScript)
                {
                    dstBurnObject->CancleProcess();
                }
                // �ִϸ��̼� Idle���
              
                mBodyAnimator->SetLoop(true);
                isAction = false;
            }
        }
    }
}

void PlayerScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
 
}

void PlayerScript::Reset()
{
    isAction = false;
    mBurnObjectScript->SetBurn(true);
    mPlayerCurHP = mPlayerMaxHP.val;
    mHpReduceCount = 0.0f;
}

void PlayerScript::SetHP(INT _val)
{
    if (mPlayerState != ePlayerStateType::DEAD)
    {
        mPlayerCurHP = _val;
    }
}

void PlayerScript::Hit(INT _damage)
{
    if (mPlayerState != ePlayerStateType::DEAD)
    {
        // �ǰ� �ִϸ��̼��� ������� ���ȿ��� �����̴�.
        if (mBodyAnimator->GetActiveAnimationKey() != L"Hit")
        {
            mBodyAnimator->SetCurrentAnimation(L"Hit");
            mPlayerCurHP -= _damage;
        }
    }
}

void PlayerScript::UpdatePlayerHP()
{
    // ��� ���¸� ������Ʈ �� �ʿ䰡 ����
    if (mPlayerState != ePlayerStateType::DEAD)
    {
        // �ҿ� Ÿ�������� 
        if (mBurnObjectScript->IsBurning())
        {
            // ü�� ���� ī��Ʈ ƽ ���
            mHpReduceCount += Time::GetUnScaledDeltaTime();
            // ü�� ���� ī��Ʈ�� ƽ���� ���� ��� HP����
            while (mHpReduceCount >= mHpReduceTick.val)
            {
                --mPlayerCurHP;
                mHpReduceCount -= mHpReduceTick.val;
            }
        }
        // �÷��̾��� ü���� 0 ������ ���
        if (mPlayerCurHP <= 0)
        {
            mPlayerState = ePlayerStateType::DEAD;
            // �ִϸ��̼� Dead���
            mBodyAnimator->SetCurrentAnimation(L"Dead");
            mBodyAnimator->SetLoop(false);
        }
    }
}

void PlayerScript::UpdateIdle()
{
}

void PlayerScript::UpdateMove()
{
    mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_WALK);
    Vector2 moveDirection = InputSyncer::GetInputDirection(mPlayerHandle.val);
    Vector2 moveForce = Vector2::Zero;
    // ��ǲ�� ���� Direction���� �ִٰ� �����Ǹ�
    if (moveDirection != Vector2::Zero)
    {
        moveForce.x = moveDirection.x * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
        moveForce.y = moveDirection.y * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
        // �̵� ���⿡ ���� ȸ�� ���� ���
        float PlayerDirectionY = atan2(moveDirection.x, moveDirection.y); // ���� ����
        gameObject->transform->SetEulerAngles(Vector3(0.0f, PlayerDirectionY - Degree::ToRadian(180.0f), 0.0f));
        mPlayerController->SetMoveForceX(moveForce.x);
        mPlayerController->SetMoveForceZ(moveForce.y);
    }
    else
    {
        SetState(ePlayerStateType::IDLE);
    }
    if (mPlayerController->IsGround() == true && isAction == false)
    {
        // ���� ���� �ƴ� �� ����Ű�� ������ ����
        if (InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::JUMP))
        {
            mPlayerController->SetMoveForceY(0.0f);
            mPlayerController->AddMoveForceY(mJumpPower.val);
            SetState(ePlayerStateType::JUMP);
        }
    }
}

void PlayerScript::UpdateJump()
{
}

void PlayerScript::UpdateAction()
{
}

void PlayerScript::UpdateDead()
{
}

void PlayerScript::InitFireLight()
{
    //////////////////////////////////////////////////////////
    // PointLight
    //////////////////////////////////////////////////////////
    Object* FireLightObject = CreateObject(L"Fire_Light", L"Light");
    FireLightObject->transform->SetParent(mFireObject->transform);
    FireLightObject->transform->position = Vector3(0.0f, 0.5f, 0.0f);
    Light* FireLight = FireLightObject->AddComponent<Light>();
    FireLight->SetLightType(eLightType::Point);
    FireLight->SetLightCutOff(0.0f);
    FireLight->SetLightRange(15.0f);
    FireLight->SetLightStrengh(50.0f);
    FireLight->SetLightNear(300.0f);
    FireLight->SetLightFar(5000.0f);
    FireLight->SetShadowDistance(3000.0f);
    FireLight->SetLightColor(ColorF(0.3f, 0.15f, 0.0f));
}

json PlayerScript::Serialize()
{
    json ret = MonoBehaviour::Serialize();

    ret["player handle"] = mPlayerHandle.val;
    ret["player max hp"] = mPlayerMaxHP.val;
    ret["player hp reduce tick"] = mHpReduceTick.val;
    ret["player move speed"] = mMoveSpeed.val;
    ret["player jump power"] = mJumpPower.val;

    return ret;
}

void PlayerScript::Deserialize(json& j)
{
    MonoBehaviour::Deserialize(j);
    if (j.contains("player handle"))
    {
        mPlayerHandle.val = j["player handle"].get<INT>();
    }
    if (j.contains("player max hp"))
    {
        mPlayerMaxHP.val = j["player max hp"].get<INT>();
    }
    if (j.contains("player hp reduce tick"))
    {
        mHpReduceTick.val = j["player hp reduce tick"].get<FLOAT>();
    }
    if (j.contains("player move speed"))
    {
        mMoveSpeed.val = j["player move speed"].get<FLOAT>();
    }
    if (j.contains("player jump power"))
    {
        mJumpPower.val = j["player jump power"].get<FLOAT>();
    }
}
