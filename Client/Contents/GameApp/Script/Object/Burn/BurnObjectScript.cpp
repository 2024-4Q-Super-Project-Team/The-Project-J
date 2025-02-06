#include "pch.h"
#include "BurnObjectScript.h"

void BurnObjectScript::Update()
{
    if (IsBurning() == false)
    {
        // 불 옮기기 작업이 진행 중이라면
        if (isProcessing && mDstObject)
        {
            // 카운트를 증가시킨다.
            mFireMoveCount += Time::GetUnScaledDeltaTime();
            // 카운트가 불 옮기기 틱을 충족할 시
            if (mFireMoveCount >= mFireMoveTick.val)
            {
                // 내 불을 켜고 대상 오브젝트의 불을 끈다.
                isBurning.val = true;
                mDstObject->isBurning.val = false;
                isProcessing = false;
                // 초기화
                SetBurnCount(0.0f);
                mDstObject = nullptr;
            }
        }
        // 불 옮기기 작업이 진행 중이 아니라면
        else
        {
            SetBurnCount(0.0f);
        }
    }
    if (mBurnObject)
    {
        if (isBurning.val)
        {
            mBurnObject->SetActive(true);
        }
        else
        {
            mBurnObject->SetActive(false);
        }
    }
   
}