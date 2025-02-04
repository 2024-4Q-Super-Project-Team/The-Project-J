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
    }
}

void PlayerScript::Update()
{
    if (isAction)
    {
        // 컨트롤러 움직이지 못하게
    }
    InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE);
    InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::JUMP);
    InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::OFF_FIRE);
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
                if(InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
                {
                    // BurnObjectScript의 불옮기기 작업을 한다.
                    if (dstBurnObject->IsProcessing() == false)
                    {
                        dstBurnObject->ProcessBurn(mBurnObjectScript);
                        mPlayerState = ePlayerStateType::MOVE_FIRE;
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
                    // 애니메이션을 IDLE로 변경
                    mAnimator->SetCurrentAnimation(L"Idle");
                    isAction = false;
                }
            }
        }
    }
}

void PlayerScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
 
}

json PlayerScript::Serialize()
{
    json ret = MonoBehaviour::Serialize();

    ret["player handle"] = mPlayerHandle.val;

    return ret;
}

void PlayerScript::Deserialize(json& j)
{
    MonoBehaviour::Deserialize(j);
    if (j.contains("player handle"))
    {
        mPlayerHandle.val = j["player handle"].get<INT>();
    }
}
