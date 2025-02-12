#include "pch.h"
#include "PlayerScript.h"
#include "Contents/GameApp/Script/GameProgressManager.h"
#include "PlayerCollisionScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
#include "Contents/GameApp/Script/Player/CheckIceSlope.h"
#include "Contents/GameApp/Script/SpriteAnimScript.h"
#include "Manager/SavePointManager.h"
#include "Contents/GameApp/Script/CameraController.h"

#define PLAYER_ANIM_IDLE L"003"
#define PLAYER_ANIM_WALK L"004"
#define PLAYER_ANIM_JUMP L"005"
#define PLAYER_ANIM_MOVE_FIRE L"006"
#define PLAYER_ANIM_OFF_FIRE L"008"
#define PLAYER_ANIM_HIT L"009"
#define PLAYER_ANIM_DEAD L"010"

#define PLAYER_SFX_WALKING_NORMAL_01    L"walking_normal_01"
#define PLAYER_SFX_WALKING_NORMAL_02    L"walking_normal_02"
#define PLAYER_SFX_WALKING_NORMAL_03    L"walking_normal_03"
#define PLAYER_SFX_WALKING_ICE_01       L"walking_ice_01"
#define PLAYER_SFX_WALKING_ICE_02       L"walking_ice_02"
#define PLAYER_SFX_WALKING_ICE_03       L"walking_ice_03"
#define PLAYER_SFX_MOVE_FIRE_START      L"move_fire_start"
#define PLAYER_SFX_MOVE_FIRE_END        L"move_fire_end"
#define PLAYER_SFX_OFF_FIRE             L"off_fire"
#define PLAYER_SFX_DEAD                 L"dead"
#define PLAYER_SFX_HIT                  L"hit"

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
            if (child->gameObject->GetName() == L"Player_Collision")
                mCollisionObject = child->gameObject;
        }
        if (mBodyObject == nullptr) Helper::HRT(E_FAIL, "Player_Body Object is nullptr");
        if (mCandleObject == nullptr) Helper::HRT(E_FAIL, "Player_Candle Object is nullptr");
        if (mFireObject == nullptr) Helper::HRT(E_FAIL, "Player_Fire Object is nullptr");
        if (mCollisionObject == nullptr) Helper::HRT(E_FAIL, "Player_Collision Object is nullptr");

        // Object -> RootNode -> Amature -> Bone -> TopBone
        mCandleTopBone = mCandleObject->transform->GetChild()->GetChild()->GetChild()->GetChild();
    }
    {
        mBodyAnimator = mBodyObject->GetComponent<Animator>();
        mCandleAnimator = mCandleObject->GetComponent<Animator>();
        mCandleAnimator->Stop();
    }
    {   // PlayerController������Ʈ ����, ������ �߰�
        mPlayerController = gameObject->GetComponent<PlayerController>();
        if (mPlayerController == nullptr)
            mPlayerController = gameObject->AddComponent<PlayerController>();
    }

    {
        // CameraController �ʱ�ȭ
        Object* cameraObject = FindObjectWithName(L"Main_Camera");             
        if (cameraObject != nullptr)
        {
            mCameraController = cameraObject->GetComponent<CameraController>();
            if (mCameraController == nullptr)
            {
                Helper::HRT(E_FAIL, "CameraController is nullptr");
            }
        }
        else
        {
            Helper::HRT(E_FAIL, "Camera object is nullptr");
        }
    }

    {   // BurnObjectScript�߰�
        mBurnObjectScript = gameObject->AddComponent<BurnObjectScript>();
        mBurnObjectScript->SetBurnObject(mFireObject);
        mBurnObjectScript->SetBurn(true);
    }
    {
        mCollisionScript = mCollisionObject->GetComponent<PlayerCollisionScript>();
        if (mCollisionScript == nullptr)
            mCollisionScript = mCollisionObject->AddComponent<PlayerCollisionScript>();
        mCollisionScript->SetOwnerPlayer(this);
    }
    {
        mFireOffEffectObject = FindChildObject(gameObject->transform->GetParent()->gameObject, L"Player_Fireoff_Effect");
        SpriteRenderer* fireOffRenderer = mFireOffEffectObject->GetComponent<SpriteRenderer>();
        if(fireOffRenderer)
            fireOffRenderer->SetActive(false);
    }
    {
        mAudioSource = mBodyObject->GetComponent<AudioSource>();
        if (mAudioSource == nullptr)
            mAudioSource = mBodyObject->AddComponent<AudioSource>();
    }

    InitFireLight();

    GameProgressManager::SetPlayerInfo(this);
}

void PlayerScript::Update()
{
    UpdatePlayerHP();
    UpdatePlayerAnim();
    CheckDead();

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
    mFireOffEffectObject->transform->position = mCandleTopBone->GetWorldPosition() + Vector3(0, 30, 0);
}

void PlayerScript::PostRender()
{
    mBurnObjectTriggerTable.clear();
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
        return;
    }
    BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
    if (dstBurnObject)
        Display::Console::Log(L"Can Fire \n");
    //if (isJump == false && InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
    //{
    //    ProcessMoveFire(dstBurnObject);
    //    return;
    //}
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

void _CALLBACK PlayerScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{

    //isInTrigger = true;

    //PlayerScript* player1 = GameProgressManager::GetPlayerInfo(0);
    //PlayerScript* player2 = GameProgressManager::GetPlayerInfo(1);

    //if (player1 && player2 && player1->isInTrigger && player2->isInTrigger)
    //{
    //    Camera* cam = mCameraController->gameObject->GetComponent<Camera>();
    //    cam->ZoomToFov(1.0f, 1.5f, 2.0f, Dotween::EasingEffect::OutSine);
    //    mCameraController->LookAt(Vector3(0.0f, 0.02f, -0.035f), 5.0f, Dotween::EasingEffect::OutSine);

    //}

    if (_destination->gameObject->GetTag() == L"CameraTrigger1")
    {
        mCameraController->mMinCameraDistance.val = 1000.0f;
        mCameraController->mMaxCameraDistance.val = 1500.0f;
    }

    if (_destination->gameObject->GetTag() == L"CameraTrigger2")
    {
        isInTrigger = true;

        PlayerScript* player1 = GameProgressManager::GetPlayerInfo(0);
        PlayerScript* player2 = GameProgressManager::GetPlayerInfo(1);

        Display::Console::Log("Trigger2 in\n");
        mCameraController->TweenMidpointOffset(Vector3(300.0f, 100.0f, 0.0f), 2.0f, Dotween::EasingEffect::OutSine);
        mCameraController->TweenOffset(Vector3(-800.0f, -250.0f, 0.0f), 2.0f, Dotween::EasingEffect::OutSine);
    }

    if (_destination->gameObject->GetTag() == L"CameraTrigger3")
    {
        mCameraController->mMinCameraDistance.val = 900.0f;
        mCameraController->mMaxCameraDistance.val = 1200.0f;
        mCameraController->TweenMidpointOffset(Vector3(-400.0f, -100.0f, 100.0f), 2.0f, Dotween::EasingEffect::OutSine);
        mCameraController->TweenOffset(Vector3(350.0f, 30.0f, 0.0f), 2.0f, Dotween::EasingEffect::OutSine);
    }

    if (_destination->gameObject->GetTag() == L"CameraTrigger4")
    {
        Display::Console::Log("Trigger4 in\n");
        mCameraController->TweenOffset(Vector3(100.0f, 0.0f, 0.0f), 2.0f, Dotween::EasingEffect::OutSine);
        mCameraController->TweenMidpointOffset(Vector3(-50.0f, 0.0f, 0.0f), 2.0f, Dotween::EasingEffect::OutSine);

    }

    if (_destination->gameObject->GetTag() == L"CameraTrigger6")
    {
        //isInTrigger = true;

        PlayerScript* player1 = GameProgressManager::GetPlayerInfo(0);
        PlayerScript* player2 = GameProgressManager::GetPlayerInfo(1);

        if (player1->isDone6 == false && player2->isDone6 == false)
        {
            Camera* cam = mCameraController->gameObject->GetComponent<Camera>();
            cam->ZoomToFov(1.0f, 1.4f, 2.0f, Dotween::EasingEffect::OutSine);
            mCameraController->LookAt(Vector3(0.0f, 0.035f, -0.035f), 3.0f, Dotween::EasingEffect::OutSine);
            isDone6 = true;
        }

    }

    if (_destination->gameObject->GetTag() == L"CameraTrigger7")
    {
        PlayerScript* player1 = GameProgressManager::GetPlayerInfo(0);
        PlayerScript* player2 = GameProgressManager::GetPlayerInfo(1);
        
        if (player1->isDone7 == false && player2->isDone7 == false)
        {
            mCameraController->LookAt(Vector3(0.0f, 0.02f, -0.035f), 4.0f, Dotween::EasingEffect::OutSine);
            isDone7 = true;
        }
    }

    if (_destination->gameObject->GetTag() == L"CameraTrigger8")
    {
        //mCameraController->LookAt(Vector3(0.0f, 0.015f, -0.035f), 6.0f, Dotween::EasingEffect::OutSine);
    }

    if (_destination->gameObject->GetTag() == L"CameraTrigger_Zoomlimit")
    {
        mCameraController->mMinCameraDistance.val = 600.0f;
        mCameraController->mMaxCameraDistance.val = 750.0f;
    }


    return void _CALLBACK();
}

void _CALLBACK PlayerScript::OnTriggerStay(Collider* _origin, Collider* _destination)
{
    OnTriggerStayCallback(_origin, _destination);
    return void _CALLBACK();
}

void _CALLBACK PlayerScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
    if (_destination->gameObject->GetTag() == L"CameraTrigger1" || L"CameraTrigger2")
    {
        isInTrigger = false;
    }
    return void _CALLBACK();
}

void _CALLBACK PlayerScript::OnTriggerStayCallback(Collider* _origin, Collider* _destination)
{
    ////////////////////////////////////////////////
    // �� �ű�� �ý���
    // [02/02 ~ ] �۾��� : ���� (�⺻ ����)
    ////////////////////////////////////////////////
    // BurnObjectScript�� GetComponent�����߳ķ� ����� ���� �ű� �� �ִ� ������Ʈ �ΰ��� ����
    BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
    
    if (dstBurnObject && isJump == false)
    {
        ProcessMoveFire(dstBurnObject);
        return;
    }
}

void _CALLBACK PlayerScript::OnTriggerExitCallback(Collider* _origin, Collider* _destination)
{
    if (mPlayerState == ePlayerStateType::MOVE_FIRE)
    {
        BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
        if (dstBurnObject)
        {
            auto itr = mBurnObjectTriggerTable.find(dstBurnObject);
            if (FIND_SUCCESS(itr, mBurnObjectTriggerTable))
            {
                mBurnObjectTriggerTable.erase(itr);
            }
            // ���� ����� ���� �� �ű�� �������� ������Ʈ�� ������ ����ϰ� IDLE�� ����
            if (dstBurnObject == mBurnProcessTarget)
            {
                mBurnProcessTarget = nullptr;
                mMoveFireCount = 0.0f;
                SetState(ePlayerStateType::IDLE);
            }
        }
    }
    return void _CALLBACK();
}

void PlayerScript::Reset()
{
    isAction = false;
    isJump = true;
    mBurnObjectScript->SetBurn(true);
    mPlayerCurHP = mPlayerMaxHP.val;
    mHpReduceCount = 0.0f;

    ResetController();
    SetState(ePlayerStateType::IDLE);
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
        if (mBodyAnimator->GetActiveAnimationKey() != PLAYER_ANIM_HIT)
        {
            mBodyAnimator->SetCurrentAnimation(PLAYER_ANIM_HIT);
            SetState(ePlayerStateType::HIT);
            mPlayerCurHP -= _damage;
            mAudioSource->Play(PLAYER_SFX_HIT);

            if (mPlayerState == ePlayerStateType::MOVE_FIRE)
            {
                mBurnProcessTarget = nullptr;
                mMoveFireCount = 0.0f;
            }
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
            SetState(ePlayerStateType::DEAD);
            mAudioSource->Play(PLAYER_SFX_DEAD);
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
    mPlayerController->SetMoveForceX(0.0f);
    mPlayerController->SetMoveForceZ(0.0f);
    ProcessJump();
    if (ProcessMove() == true)
    {
        // ������ �õ��� �־��ٸ� MOVE������Ʈ�� ����
        SetState(ePlayerStateType::MOVE);
        return;
    }
    if (isJump == false && InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::OFF_FIRE))
    {
        ProcessOffFire(nullptr);
    }
}

void PlayerScript::UpdateMove()
{
    static FLOAT walkSoundTick = 0.4f;
    static FLOAT walkSoundCounter = 0.0f;

    walkSoundCounter += Time::GetScaledDeltaTime();
    if (walkSoundCounter >= walkSoundTick)
    {
        walkSoundCounter = 0.0f;
        mAudioSource->Play(PLAYER_SFX_WALKING_NORMAL_01);
    }

    ProcessJump();

    if (ProcessMove() == false)
    {
        SetState(ePlayerStateType::IDLE);
        return;
    }
    if (isJump == false && InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::OFF_FIRE))
    {
        ProcessOffFire(nullptr);
    }
}

void PlayerScript::UpdateHit()
{
    mPlayerController->SetMoveForceX(0.0f);
    mPlayerController->SetMoveForceZ(0.0f);
    if (mBodyAnimator->IsEnd())
    {
        // �ִϸ��̼� ����
        SetState(ePlayerStateType::IDLE);
    }
}

void PlayerScript::UpdateMoveFire()
{
    mPlayerController->SetMoveForceX(0.0f);
    mPlayerController->SetMoveForceZ(0.0f);
    if (mBodyAnimator->GetActiveAnimationKey() == PLAYER_ANIM_MOVE_FIRE)
    {
        if (mBurnProcessTarget)
        {
            // �߰��� Ű�� ���� ����Ѵ�.
            if (false == InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
            {
                SetState(ePlayerStateType::IDLE);
                mBurnProcessTarget = nullptr;
                mMoveFireCount = 0.0f;
                return;
            }

            Vector3 dstPos = mBurnProcessTarget->gameObject->transform->GetWorldPosition();
            Vector3 srcPos = gameObject->transform->GetWorldPosition();
            Vector2 viewDirection = { dstPos.x - srcPos.x, dstPos.z - srcPos.z };
            viewDirection.Normalize();
            float targetAngleY = atan2(-viewDirection.x, -viewDirection.y);
            if (targetAngleY < 0.0f)
                targetAngleY += XM_2PI;  // 360�� ��� 2�� ���
            Vector3 CurrAngle = gameObject->transform->GetEulerAngles();
            float currAngleY = CurrAngle.y;
            float delta = fmod(targetAngleY - currAngleY + XM_PI, XM_2PI) - XM_PI;
            float newAngleY = currAngleY + delta * 0.7f;
            gameObject->transform->SetEulerAngles(Vector3(0.0f, newAngleY, 0.0f));

            // �� �ű�� ī��Ʈ�� �������� ����� ��ŸŸ������ ���Ѵ�.
            mMoveFireCount += Time::GetScaledDeltaTime();

            // �� �ű�� ���൵�� ����ȭ �Ѵ�. UI�������� ����.
            FLOAT ProcessRatio = Clamp(mMoveFireCount / mMoveFireTick.val, 0.0f, 1.0f);

            if (mMoveFireCount >= mMoveFireTick.val)
            {
                // ����� �����ְ� ���� ��Ÿ�� �߿��� �����Ѵ�.
                if (mBurnProcessTarget->IsBurning() == false &&
                    mBurnObjectScript->IsBurning() == true)
                {
                    mBurnProcessTarget->SetBurn(true);
                    mBurnProcessTarget->SetDestObject(mBurnObjectScript);
                    mAudioSource->Play(PLAYER_SFX_MOVE_FIRE_END);
                }
                mBurnProcessTarget = nullptr;
                mMoveFireCount = 0.0f;
                SetState(ePlayerStateType::IDLE);
            }
        }
    }
}

void PlayerScript::UpdateOffFire()
{
    mPlayerController->SetMoveForceX(0.0f);
    mPlayerController->SetMoveForceZ(0.0f);
    if (mBodyAnimator->GetActiveAnimationKey() == PLAYER_ANIM_OFF_FIRE)
    {
        if (mBodyAnimator->IsEnd())
        {
            SetState(ePlayerStateType::IDLE);
        }
    }
}

void PlayerScript::UpdateDead()
{
    mPlayerController->SetMoveForceX(0.0f);
    mPlayerController->SetMoveForceZ(0.0f);
    if (mBurnObjectScript->IsBurning() == true)
    {
        mBurnObjectScript->SetBurn(false);
    }
    if(mBodyAnimator->IsEnd())
        SavePointManager::GetInstance().GoBackSavePoint(this);
}

bool PlayerScript::ProcessMove()
{
    Vector2 moveDirection = InputSyncer::GetInputDirection(mPlayerHandle.val);
    Vector2 moveForce = Vector2::Zero;
    // ��ǲ�� ���� Direction���� �ִٰ� �����Ǹ�
    if (moveDirection != Vector2::Zero)
    {
        moveForce.x = moveDirection.x * mMoveSpeed.val * Time::GetScaledDeltaTime();
        moveForce.y = moveDirection.y * mMoveSpeed.val * Time::GetScaledDeltaTime();
        // �̵� ���⿡ ���� ȸ�� ���� ���
        float PlayerDirectionY = atan2(moveDirection.x, moveDirection.y); // ���� ����
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
        else
        {
            float jumpTimeRatio = (mMaxJumpTimeTick.val - mJumpTimeCount) * (mMaxJumpHoldScale.val * 0.01f);
            mPlayerController->AddMoveForceY(mJumpPower.val * jumpTimeRatio);
        }
    }
}

void PlayerScript::ProcessMoveFire(BurnObjectScript* _dst)
{
    // dst�� ������ �ൿ�� �� �� ����.
    if (_dst == nullptr || _dst == mBurnObjectScript) return;

    // ����� �����ְ� ���� ��Ÿ�� �߿��� �����Ѵ�.
    if (_dst->IsBurning() == false && 
        mBurnObjectScript->IsBurning() == true)
    {
        mBurnObjectTriggerTable.insert(_dst);
        if (InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE) && mPlayerState != ePlayerStateType::MOVE_FIRE)
        {
            SetState(ePlayerStateType::MOVE_FIRE);
            mBurnProcessTarget = _dst;
            mMoveFireCount = 0.0f;
            mAudioSource->Play(PLAYER_SFX_MOVE_FIRE_START);
        }
    }
}

void PlayerScript::ProcessOffFire(BurnObjectScript* _dst)
{
    // dst�� ������ ����� ���� ����
    // dst�� ������ �� �ڽ��� ���� ����
    if (_dst)
    {
        // dst�� ��Ÿ�� ���� ���� ����
        if (_dst->IsBurning() == false)
        {
            SetState(ePlayerStateType::OFF_FIRE);
            mBurnProcessTarget = _dst;
            mAudioSource->Play(PLAYER_SFX_OFF_FIRE);
            //�� ���� �� ����Ʈ 
            Object* dstFireObject = FindChildObject(_dst->gameObject->transform->GetParent()->gameObject, L"Player_Fireoff_Effect");
            SpriteAnimScript* effectAnim = mFireOffEffectObject->GetComponent<SpriteAnimScript>();
            if (effectAnim != nullptr)
                 effectAnim->Play();
        }
    }
    else
    {
        // ��Ÿ�� ���� ���� ����
        if (mBurnObjectScript->IsBurning() == true)
        {
            mBurnObjectScript->SetBurn(false);
            SetState(ePlayerStateType::OFF_FIRE);
            mAudioSource->Play(PLAYER_SFX_OFF_FIRE);
            mBurnProcessTarget = nullptr;

            SpriteAnimScript* effectAnim = mFireOffEffectObject->GetComponent<SpriteAnimScript>();
            if (effectAnim != nullptr)
                effectAnim->Play();
        }
    }
}

void PlayerScript::CheckDead()
{

    // y��ǥ ���� ���ϸ� �ֱ�
    if (gameObject->transform->position.y < -100.0f)
    {
        mPlayerState = ePlayerStateType::DEAD;
    }

    if (GameProgressManager::GetGameStatus() == eGameProgressStatus::GAME_OVER)
        mPlayerState = ePlayerStateType::DEAD;

}

bool PlayerScript::IsBurning()
{
    return mBurnObjectScript->IsBurning();
}

bool PlayerScript::IsJump()
{
    return mPlayerController->IsGround() == false;
}

float PlayerScript::GetRatio()
{
    return Clamp(mMoveFireCount / mMoveFireTick.val, 0.0f, 1.0f);
}

void PlayerScript::ResetController()
{
    gameObject->GetComponent<PlayerController>()->Reset();
    mPlayerController->SetSlopeMode(PlayerController::SlopeMode::Ride);
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
    ret["player jump hold scale"] = mMaxJumpHoldScale.val;
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
    if (j.contains("player jump hold scale"))
    {
        mMaxJumpHoldScale.val = j["player jump hold scale"].get<FLOAT>();
    }
}
