#include "pch.h"
#include "PlayerScript.h"
#include "PlayerCollisionScript.h"
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
            if (child->gameObject->GetName() == L"Player_Collision")
                mCollisionObject = child->gameObject;
        }
        if (mBodyObject == nullptr) Helper::HRT(E_FAIL, "Player_Body Object is nullptr");
        if (mCandleObject == nullptr) Helper::HRT(E_FAIL, "Player_Candle Object is nullptr");
        if (mFireObject == nullptr) Helper::HRT(E_FAIL, "Player_Fire Object is nullptr");
        //if (mCollisionObject == nullptr) Helper::HRT(E_FAIL, "Player_Collision Object is nullptr");

        // Object -> RootNode -> Amature -> Bone -> TopBone
        mCandleTopBone = mCandleObject->transform->GetChild()->GetChild()->GetChild()->GetChild();
    }
    {
        mBodyAnimator = mBodyObject->GetComponent<Animator>();
        mCandleAnimator = mCandleObject->GetComponent<Animator>();
        mCandleAnimator->Stop();
    }
    {   // PlayerController컴포넌트 저장, 없으면 추가
        mPlayerController = gameObject->GetComponent<PlayerController>();
        if (mPlayerController == nullptr)
            mPlayerController = gameObject->AddComponent<PlayerController>();
    }
    {   // BurnObjectScript추가
        mBurnObjectScript = gameObject->AddComponent<BurnObjectScript>();
        mBurnObjectScript->SetBurnObject(mFireObject);
    }
    //{
    //    mCollisionScript =  mCollisionObject->AddComponent<PlayerCollisionScript>();
    //    mCollisionScript->SetOwnerPlayer(this);
    //}

    InitFireLight();

    PlayerManager::SetPlayerInfo(this);
}

void PlayerScript::Update()
{
    UpdatePlayerHP();
    UpdatePlayerAnim();
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
    case ePlayerStateType::OFF_FIRE:
        UpdateOffFire();
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

void _CALLBACK PlayerScript::OnTriggerStayCallback(Collider* _origin, Collider* _destination)
{
    ////////////////////////////////////////////////
    // 불 옮기기 시스템
    // [02/02 ~ ] 작업자 : 주형 (기본 구조)
    ////////////////////////////////////////////////
    // BurnObjectScript를 GetComponent성공했냐로 대상이 불을 옮길 수 있는 오브젝트 인가를 구분
    BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
    if (dstBurnObject)
        Display::Console::Log(L"Can Fire \n");
    if (InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
    {
        ProcessMoveFire(dstBurnObject);
        return;
    }
    if (InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::OFF_FIRE))
    {
        ProcessOffFire(dstBurnObject);
        return;
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

void PlayerScript::Jump(FLOAT _scale, bool _canHold)
{
    isJump = true;
    mJumpTimeCount = 0.0f;
    mPlayerController->SetMoveForceY(0.0f);
    mPlayerController->AddMoveForceY(mJumpPower.val * _scale);

    if (_canHold == TRUE)
    {
        mJumpTrigger = true;
    }
    else
    {
        mJumpTrigger = false;
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
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_IDLE, 0.5f) :
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_JUMP, 0.5f);
        break;
    }
    case ePlayerStateType::MOVE:
    {
        isJump == false ?
        mBodyAnimator->SetLoop(true) :
        mBodyAnimator->SetLoop(false);
        isJump == false ?
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_WALK, 0.5f) :
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_JUMP, 0.5f);
        break;
    } 
    case ePlayerStateType::HIT:
    {
        mBodyAnimator->SetLoop(false);
        mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_HIT, 0.5f);
        break;
    }
    case ePlayerStateType::MOVE_FIRE:
    {
        mBodyAnimator->SetLoop(false);
        mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_MOVE_FIRE, 0.5f);
        break;
    }
    case ePlayerStateType::OFF_FIRE:
    {
        mBodyAnimator->SetLoop(false);
        mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_OFF_FIRE, 0.5f);
        break;
    }
    case ePlayerStateType::DEAD:
    {
        mBodyAnimator->SetLoop(false);
        mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_DEAD, 0.5f);
        break;
    }
    default:
        break;
    }

    FLOAT totalFrame = mCandleAnimator->GetActiveAnimationResource()->GetTotalFrame();
    FLOAT hpRatio = (FLOAT)mPlayerCurHP / (FLOAT)mPlayerMaxHP.val;
    mCandleAnimator->SetFrame(totalFrame - (hpRatio * totalFrame));
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
        ProcessOffFire(nullptr);
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
    if (InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
    {
        ProcessOffFire(nullptr);
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
        if (mBurnProcessTarget)
        {
            // 로테이션 보간
            Vector3 dstPos = mBurnProcessTarget->gameObject->transform->GetWorldPosition();
            Vector3 srcPos = gameObject->transform->GetWorldPosition();
            Vector2 viewDirection = { dstPos.x - srcPos.x, srcPos.y - srcPos.y };
            viewDirection.Normalize();
            float PlayerDirectionY = atan2(viewDirection.x, viewDirection.y); // 라디안 단위
            gameObject->transform->SetEulerAngles(Vector3(0.0f, PlayerDirectionY - Degree::ToRadian(180.0f), 0.0f));

            // 중간에 키를 떼면 취소한다.
            if (InputSyncer::IsKeyUp(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
            {
                SetState(ePlayerStateType::IDLE);
                return;
            }
            // 불 옮기기 카운트를 스케일이 적용된 델타타임으로 더한다.
            mMoveFireCount += Time::GetScaledDeltaTime();

            // 불 옮기기 진행도를 비율화 한다. UI게이지를 위함.
            FLOAT ProcessRatio = Clamp(mMoveFireCount / mMoveFireTick.val, 0.0f, 1.0f);

            if (mMoveFireCount >= mMoveFireTick.val)
            {
                // ===== 불 옮기기 Action ====
                 // 대상이 불타고 내가 꺼져있음
                if (mBurnProcessTarget->IsBurning() == false &&
                    mBurnObjectScript->IsBurning() == true)
                {
                    mBurnProcessTarget->SetBurn(false);
                    mBurnObjectScript->SetBurn(true);
                }
                // 대상이 꺼져있고 내가 불타는 중
                if (mBurnProcessTarget->IsBurning() == true &&
                    mBurnObjectScript->IsBurning() == false)
                {
                    mBurnProcessTarget->SetBurn(true);
                    mBurnObjectScript->SetBurn(false);
                }
                SetState(ePlayerStateType::IDLE);
            }
        }
    }
}

void PlayerScript::UpdateOffFire()
{
    if (mBodyAnimator->GetActiveAnimationKey() == PLAYER_ANIM_OFF_FIRE)
    {
        if (mBodyAnimator->IsEnd())
        {
            // 대상이 없는 상태면 자신의 불을 끈다.
            if (mBurnProcessTarget == nullptr)
            {
                mBurnObjectScript->SetBurn(false);
            }
            SetState(ePlayerStateType::IDLE);
        }
    }
}

void PlayerScript::UpdateDead()
{
    if (mBurnObjectScript->IsBurning() == true)
    {
        mBurnObjectScript->SetBurn(false);
    }
    mPlayerController->SetMoveForceX(0.0f);
    mPlayerController->SetMoveForceZ(0.0f);
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
    if (isJump == false && mPlayerController->GetSlopeMode() != PlayerController::SlopeMode::Slide)
    {
        if (InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::JUMP) &&
            mPlayerController->GetSlopeMode() == PlayerController::SlopeMode::Ride)
        {
            Jump();
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

void PlayerScript::ProcessMoveFire(BurnObjectScript* _dst)
{
    // dst가 없으면 행동을 할 수 없다.
    if (_dst == nullptr) return;
    if ((mBurnObjectScript->IsBurning() == true && _dst->IsBurning() == false) ||
        (mBurnObjectScript->IsBurning() == false && _dst->IsBurning() == true) )
    {
        SetState(ePlayerStateType::MOVE_FIRE);
        mBurnProcessTarget = _dst;
        mMoveFireCount = 0.0f;
    }
}

void PlayerScript::ProcessOffFire(BurnObjectScript* _dst)
{
    // dst가 있으면 대상의 불을 끄기
    // dst가 없으면 나 자신의 불을 끄기
    if (_dst)
    {
        // dst가 불타는 중일 때만 가능
        if (_dst->IsBurning() == false)
        {
            SetState(ePlayerStateType::OFF_FIRE);
            mBurnProcessTarget = _dst;
        }
    }
    else
    {
        // 불타는 중일 때만 가능
        if (mBurnObjectScript->IsBurning() == true)
        {
            SetState(ePlayerStateType::OFF_FIRE);
            mBurnProcessTarget = _dst;
        }
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
    ret["player move fire tick"] = mMoveFireTick.val;

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
    if (j.contains("player move fire tick"))
    {
        mMoveFireTick.val = j["player move fire tick"].get<FLOAT>();
    }
}
