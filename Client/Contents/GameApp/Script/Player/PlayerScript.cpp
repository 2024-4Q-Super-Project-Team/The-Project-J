#include "pch.h"
#include "PlayerScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
void PlayerScript::Start()
{
    // 초기화 코드
    gameObject->SetTag(L"Player");

    {
        mAnimator = gameObject->GetComponent<Animator>();
    }
    {   // PlayerController컴포넌트 저장, 없으면 추가
        mPlayerController = gameObject->GetComponent<PlayerController>();
        if (mPlayerController == nullptr)
            mPlayerController = gameObject->AddComponent<PlayerController>();
    }
    {   // BurnObject추가
        mBurnObjectScript = gameObject->AddComponent<BurnObjectScript>();
        // JH TODO : 불 메쉬 넣어주기
        mBurnObjectScript->SetBurnMesh(nullptr);
    }
}

void PlayerScript::Update()
{
    // HP 갱신
    UpdatePlayerHP();
    UpdateInput();
    if (isAction)
    {
        // 컨트롤러 움직이지 못하게
    }
    else
    {

    }
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
                mAnimator->SetCurrentAnimation(L"Idle");
                mAnimator->SetLoop(true);
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
        // 피격 애니메이션이 재생중인 동안에는 무적이다.
        if (mAnimator->GetActiveAnimationKey() != L"Hit")
        {
            mAnimator->SetCurrentAnimation(L"Hit");
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
            mPlayerState = ePlayerStateType::DEAD;
            // 애니메이션 Dead재생
            mAnimator->SetCurrentAnimation(L"Dead");
            mAnimator->SetLoop(false);
        }
    }
}

void PlayerScript::UpdateInput()
{
    Vector2 moveDirection = InputSyncer::GetInputDirection(mPlayerHandle.val);
    Vector2 moveForce = Vector2::Zero;
    // 인풋을 통해 Direction값이 있다고 판정되면
    if (moveDirection != Vector2::Zero)
    {
        moveForce.x = moveDirection.x * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
        moveForce.y = moveDirection.y * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
    }
    mPlayerController->SetMoveForceX(moveForce.x);
    mPlayerController->SetMoveForceZ(moveForce.y);
    if (mPlayerController->IsGround() == true && isAction == false)
    {
        // 점프 중이 아닐 때 점프키를 누르면 점프
        if (InputSyncer::IsKeyDown(mPlayerHandle.val, InputSyncer::JUMP))
        {
            mPlayerController->SetMoveForceY(0.0f);
            mPlayerController->AddMoveForceY(mJumpPower.val);
        }
    }
}

json PlayerScript::Serialize()
{
    json ret = MonoBehaviour::Serialize();

    ret["player handle"] = mPlayerHandle.val;
    ret["player max hp"] = mPlayerMaxHP.val;
    ret["player hp reduce tick"] = mHpReduceTick.val;
    ret["player move speed"] = mJumpPower.val;
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
