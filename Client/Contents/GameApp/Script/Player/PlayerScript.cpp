#include "pch.h"
#include "PlayerScript.h"
#include "PlayerState.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
void PlayerScript::Start()
{
    // 초기화 코드
    gameObject->SetTag(L"Player");
    SetPlayerAnimation();


    {   // PlayerController컴포넌트 저장, 없으면 추가
        mPlayerController = gameObject->GetComponent<PlayerController>();
        if (mPlayerController == nullptr)
            mPlayerController = gameObject->AddComponent<PlayerController>();
    }
    {   // BurnObject추가
        mBurnObjectScript = gameObject->AddComponent<BurnObjectScript>();
    }
}

void PlayerScript::Update()
{
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
        if (_destination->gameObject->GetTag() == L"Player" ||
            _destination->gameObject->GetTag() == L"BurnObject")
        {
            // 사실 굳이 태그 구별 안해도 BurnObjectScript를 GetComponent성공했냐로 구분할 수 있긴함.
            BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
            // dstBurnObject가 맞고, dstBurnObject의 불이 붙어있는 경우
            if (dstBurnObject && dstBurnObject->IsBurning())
            {
                // TODO : 캐릭터(gameO-bject)의 머리 위에 불 옮기기 UI 팝업

                // 불 옮기기 키를 눌렀을 때
                if (GetKeyForce(ePlayerInputType::ACTION_MOVE_FIRE) != 0.0f)
                {
                    // BurnObjectScript의 불옮기기 작업을 한다.
                    if (dstBurnObject->IsProcessing() == false)
                    {
                        dstBurnObject->ProcessBurn(mBurnObjectScript);
                    }
                }
                else
                {  
                    // BurnObjectScript의 불옮기기 작업을 취소시킨다.
                    if (dstBurnObject->IsProcessing() == true &&
                        dstBurnObject->GetDestObject() == mBurnObjectScript)
                    {
                        dstBurnObject->CancleProcess();
                    }
                    // TODO : 애니메이션을 IDLE로 변경
                }
            }
        }
    }
}

void PlayerScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
 
}

void PlayerScript::SetPlayerHandle(UINT _Index)
{
    mPlayerHandle = _Index;
}

void PlayerScript::SetPlayerAnimation()
{
    {   // Animator컴포넌트 저장, 없으면 추가
        mAnimator = gameObject->GetComponent<Animator>();
        if (mAnimator == nullptr)
            mAnimator = gameObject->AddComponent<Animator>();
    }
    //mAnimator->AddAnimation();
}

// 반환 값이 float인 이유는, pad의 조이스틱은 0~1의 값을 반환하기 때문
// 조이스틱을 빼면 1.0f를 반환
float PlayerScript::GetKeyForce(ePlayerInputType _inputType)
{
    switch (_inputType)
    {
    case ePlayerInputType::MOVE_LEFT:
        break;
    case ePlayerInputType::MOVE_RIGHT:
        break;
    case ePlayerInputType::MOVE_FORWARD:
        break;
    case ePlayerInputType::MOVE_BACKWARD:
        break;
    case ePlayerInputType::JUMP:
        break;
    case ePlayerInputType::ACTION_MOVE_FIRE:
        break;
    case ePlayerInputType::ACTION_OFF_FIRE:
        break;
    default:
        break;
    }
    return false;
}
