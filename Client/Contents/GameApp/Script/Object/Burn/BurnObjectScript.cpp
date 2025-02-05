#include "pch.h"
#include "BurnObjectScript.h"

void BurnObjectScript::Update()
{
    if (IsBurning() == false)
    {
        // �� �ű�� �۾��� ���� ���̶��
        if (isProcessing && mDstObject)
        {
            // ī��Ʈ�� ������Ų��.
            mFireMoveCount += Time::GetUnScaledDeltaTime();
            // ī��Ʈ�� �� �ű�� ƽ�� ������ ��
            if (mFireMoveCount >= mFireMoveTick.val)
            {
                // �� ���� �Ѱ� ��� ������Ʈ�� ���� ����.
                isBurning.val = true;
                mDstObject->isBurning.val = false;
                isProcessing = false;
                // �ʱ�ȭ
                SetBurnCount(0.0f);
                mDstObject = nullptr;
            }
        }
        // �� �ű�� �۾��� ���� ���� �ƴ϶��
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