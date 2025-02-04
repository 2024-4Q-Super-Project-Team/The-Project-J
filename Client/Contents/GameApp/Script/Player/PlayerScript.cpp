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
    mPlayerHP.val = mPlayerMaxHP.val;
    mHpReduceCount.val = 0.0f;
}

void PlayerScript::SetHP(INT _val)
{
    if (mPlayerState != ePlayerStateType::DEAD)
    {
        mPlayerHP.val = _val;
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
            mPlayerHP.val -= _damage;
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
            mHpReduceCount.val += Time::GetUnScaledDeltaTime();
            // 체력 감소 카운트가 틱보다 높은 경우 HP감소
            while (mHpReduceCount.val >= mHpReduceTick.val)
            {
                --mPlayerHP.val;
                mHpReduceCount.val -= mHpReduceTick.val;
            }
        }
        // 플레이어의 체력이 0 이하일 경우
        if (mPlayerHP.val <= 0)
        {
            mPlayerState = ePlayerStateType::DEAD;
            // 애니메이션 Dead재생
            mAnimator->SetCurrentAnimation(L"Dead");
            mAnimator->SetLoop(false);
        }
    }
}

json PlayerScript::Serialize()
{
    json ret = MonoBehaviour::Serialize();

    ret["player handle"] = mPlayerHandle.val;
    ret["player max hp"] = mPlayerMaxHP.val;
    ret["player hp reduce tick"] = mHpReduceTick.val;

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
}
