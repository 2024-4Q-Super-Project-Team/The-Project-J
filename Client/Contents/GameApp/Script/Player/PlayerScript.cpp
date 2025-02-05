#include "pch.h"
#include "PlayerScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
#include "Manager/PlayerManager.h"
#include "Contents/GameApp/Script/Player/CheckIceSlope.h"

#define PLAYER_ANIM_IDLE L"003"
#define PLAYER_ANIM_WALK L"004"
#define PLAYER_ANIM_JUMP L"005"
#define PLAYER_ANIM_MOVE_FIRE L"006"
#define PLAYER_ANIM_OFF_FIRE L"008"
#define PLAYER_ANIM_HIT L"009"
#define PLAYER_ANIM_DEAD L"010"

void PlayerScript::Start()
{
    // 초기화 코드
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
    {   // PlayerController컴포넌트 저장, 없으면 추가
        mPlayerController = gameObject->GetComponent<PlayerController>();
        if (mPlayerController == nullptr)
            mPlayerController = gameObject->AddComponent<PlayerController>();
    }
    {   // BurnObjectScript추가
        mBurnObjectScript = mBodyObject->AddComponent<BurnObjectScript>();
        mBurnObjectScript->SetBurnMesh(mFireObject->GetComponent<MeshRenderer>());
    }
    {
        mBodyObject->AddComponent<CheckIceSlope>();
    }
    InitFireLight();

    PlayerManager::SetPlayerInfo(this);
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
    case ePlayerStateType::HIT:
        UpdateHit();
        break;
    case ePlayerStateType::MOVE_FIRE:
        UpdateMoveFire();
        break;
    case ePlayerStateType::DEAD:
        UpdateDead();
        break;
    default:
        break;
    }
    UpdatePlayerAnim();

    mFireObject->transform->position = mCandleTopBone->GetWorldPosition();
}

void PlayerScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{

}

void PlayerScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
    ////////////////////////////////////////////////
    // 불 옮기기 시스템
    // [02/02 ~ ] 작업자 : 주형 (기본 구조)
    ////////////////////////////////////////////////
    // 불이 붙어있을 때
    if (mBurnObjectScript->IsBurning())
    {
        // BurnObjectScript를 GetComponent성공했냐로 대상이 불을 옮길 수 있는 오브젝트 인가를 구분
        BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
        // dstBurnObject가 맞고, dstBurnObject의 불이 붙어있지 않는 경우
        if (dstBurnObject && dstBurnObject->IsBurning() == false)
        {
            // JH TODO : 캐릭터(gameO-bject)의 머리 위에 불 옮기기 UI 팝업

            // 불 옮기기 키를 눌렀을 때
            if (InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
            {
                // BurnObjectScript의 불옮기기 작업을 한다.
                if (dstBurnObject->IsProcessing() == false)
                {
                    dstBurnObject->ProcessBurn(mBurnObjectScript);
                    mPlayerState = ePlayerStateType::MOVE_FIRE;
                }
            }
            else  if (InputSyncer::IsKeyUp(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
            {
                // BurnObjectScript의 불옮기기 작업을 취소시킨다.
                if (dstBurnObject->IsProcessing() == true &&
                    dstBurnObject->GetDestObject() == mBurnObjectScript)
                {
                    dstBurnObject->CancleProcess();
                }
                // 애니메이션 Idle재생
              
                mBodyAnimator->SetLoop(true);
                isAction = false;
            }
        }
    }

    if (_destination->gameObject->GetTag() == L"IceSlope" 
        && mBurnObjectScript->IsBurning() == true)
    {
        mPlayerController->SetSlopeMode(PlayerController::SlopeMode::Slide);
    }
}

void PlayerScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
    if (_destination->gameObject->GetTag() == L"IceSlope" 
        && mPlayerController->GetSlopeMode() == PlayerController::SlopeMode::Slide)
    {
        mPlayerController->SetSlopeMode(PlayerController::SlopeMode::Ride);
        mPlayerController->SetMoveForceY(0.0f);
    }
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
        // 피격 애니메이션이 재생중인 동안에는 무적이다.
        if (mBodyAnimator->GetActiveAnimationKey() != PLAYER_ANIM_HIT)
        {
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_HIT);
            SetState(ePlayerStateType::HIT);
            mPlayerCurHP -= _damage;
        }
    }
}

void PlayerScript::UpdatePlayerHP()
{
    // 사망 상태면 업데이트 할 필요가 없음
    if (mPlayerState != ePlayerStateType::DEAD)
    {
        // 불에 타고있으면 
        if (mBurnObjectScript->IsBurning())
        {
            // 체력 감소 카운트 틱 계산
            mHpReduceCount += Time::GetUnScaledDeltaTime();
            // 체력 감소 카운트가 틱보다 높은 경우 HP감소
            while (mHpReduceCount >= mHpReduceTick.val)
            {
                --mPlayerCurHP;
                mHpReduceCount -= mHpReduceTick.val;
            }
        }
        // 플레이어의 체력이 0 이하일 경우
        if (mPlayerCurHP <= 0)
        {
            SetState(ePlayerStateType::DEAD);
        }
    }
}

void PlayerScript::UpdatePlayerAnim()
{
    switch (mPlayerState)
    {
    case ePlayerStateType::IDLE:
    {
        isJump == false ?
            mBodyAnimator->SetLoop(true) :
            mBodyAnimator->SetLoop(false);
        isJump == false ?
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_IDLE) :
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_JUMP);
        break;
    }
    case ePlayerStateType::MOVE:
    {
        isJump == false ?
        mBodyAnimator->SetLoop(true) :
        mBodyAnimator->SetLoop(false);
        isJump == false ?
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_WALK) :
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_JUMP);
        break;
    } 
    case ePlayerStateType::HIT:
    {
        mBodyAnimator->SetLoop(false);
        mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_HIT);
        break;
    }
    case ePlayerStateType::MOVE_FIRE:
    {
        break;
    }
    case ePlayerStateType::DEAD:
    {
        mBodyAnimator->SetLoop(false);
        mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_DEAD);
        break;
    }
    default:
        break;
    }
}

void PlayerScript::UpdateIdle()
{
    ProcessJump();
    if (ProcessMove() == true)
    {
        SetState(ePlayerStateType::MOVE);
        return;
    }
    if (InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
    {
        ProcessMoveFire();
    }
}

void PlayerScript::UpdateMove()
{
    ProcessJump();
    if (ProcessMove() == false)
    {
        SetState(ePlayerStateType::IDLE);
        return;
    }
    
   
}

void PlayerScript::UpdateHit()
{
    if (mBodyAnimator->IsEnd())
    {
        // 애니메이션 블렌드
        SetState(ePlayerStateType::IDLE);
    }
}

void PlayerScript::UpdateMoveFire()
{
    if (mBodyAnimator->GetActiveAnimationKey() == PLAYER_ANIM_MOVE_FIRE)
    {

        //if (mBodyAnimator->IsEnd())
        //{
        //
        //}
        //else
        //{
        //    isAction = true;
        //}
    }
    
}

void PlayerScript::UpdateDead()
{
}

bool PlayerScript::ProcessMove()
{
    Vector2 moveDirection = InputSyncer::GetInputDirection(mPlayerHandle.val);
    Vector2 moveForce = Vector2::Zero;
    // 인풋을 통해 Direction값이 있다고 판정되면
    if (moveDirection != Vector2::Zero)
    {
        moveForce.x = moveDirection.x * mMoveSpeed.val * Time::GetScaledDeltaTime();
        moveForce.y = moveDirection.y * mMoveSpeed.val * Time::GetScaledDeltaTime();
        // 이동 방향에 따른 회전 각도 계산
        float PlayerDirectionY = atan2(moveDirection.x, moveDirection.y); // 라디안 단위
        gameObject->transform->SetEulerAngles(Vector3(0.0f, PlayerDirectionY - Degree::ToRadian(180.0f), 0.0f));
    }
    mPlayerController->SetMoveForceX(moveForce.x);
    mPlayerController->SetMoveForceZ(moveForce.y);

    return moveDirection != Vector2::Zero ? true : false;
}

void PlayerScript::ProcessJump()
{
    if (isJump == false)
    {
        if (InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::JUMP) &&
            mPlayerController->GetSlopeMode() == PlayerController::SlopeMode::Ride)
        {
            isJump = true;
            mJumpTrigger = true;
            mJumpTimeCount = 0.0f;
            mPlayerController->SetMoveForceY(0.0f);
            mPlayerController->AddMoveForceY(mJumpPower.val);
        }
    }
    else if (isJump == true)
    {
        if (mPlayerController->IsGround())
        {
            isJump = false;
            mJumpTrigger = false;
            mPlayerController->SetMoveForceY(0.0f);
        }
    }
    if (mJumpTrigger == true)
    {
        mJumpTimeCount += Time::GetScaledDeltaTime();
        if (mJumpTimeCount >= mMaxJumpTimeTick.val ||
            InputSyncer::IsKeyUp(mPlayerHandle.val, InputSyncer::JUMP))
        {
            mJumpTrigger = false;
            mJumpTimeCount = mMaxJumpTimeTick.val;
        }
        float jumpTimeRatio = (mMaxJumpTimeTick.val - mJumpTimeCount) * 0.01f;
        mPlayerController->AddMoveForceY(mJumpPower.val * jumpTimeRatio);
    }
   
}

void PlayerScript::ProcessMoveFire()
{
    if (mBurnObjectScript->IsBurning() == true)
    {
        SetState(ePlayerStateType::MOVE_FIRE);
    }
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
    ret["player jump tick"] = mMaxJumpTimeTick.val;

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
    if (j.contains("player jump tick"))
    {
        mMaxJumpTimeTick.val = j["player jump tick"].get<FLOAT>();
    }
}
