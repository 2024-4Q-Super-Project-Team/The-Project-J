#include "pch.h"
#include "PlayerScript.h"
#include "PlayerState.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
void PlayerScript::Start()
{
    // �ʱ�ȭ �ڵ�
    gameObject->SetTag(L"Player");
    SetPlayerAnimation();


    {   // PlayerController������Ʈ ����, ������ �߰�
        mPlayerController = gameObject->GetComponent<PlayerController>();
        if (mPlayerController == nullptr)
            mPlayerController = gameObject->AddComponent<PlayerController>();
    }
    {   // BurnObject�߰�
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
    // �� �ű�� �ý���
    // [02/02 ~ ] �۾��� : ���� (�⺻ ����)
    ////////////////////////////////////////////////
    // ���� �پ����� ��
    if (mBurnObjectScript->IsBurning())
    {
        if (_destination->gameObject->GetTag() == L"Player" ||
            _destination->gameObject->GetTag() == L"BurnObject")
        {
            // ��� ���� �±� ���� ���ص� BurnObjectScript�� GetComponent�����߳ķ� ������ �� �ֱ���.
            BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
            // dstBurnObject�� �°�, dstBurnObject�� ���� �پ��ִ� ���
            if (dstBurnObject && dstBurnObject->IsBurning())
            {
                // TODO : ĳ����(gameO-bject)�� �Ӹ� ���� �� �ű�� UI �˾�

                // �� �ű�� Ű�� ������ ��
                if (GetKeyForce(ePlayerInputType::ACTION_MOVE_FIRE) != 0.0f)
                {
                    // BurnObjectScript�� �ҿű�� �۾��� �Ѵ�.
                    if (dstBurnObject->IsProcessing() == false)
                    {
                        dstBurnObject->ProcessBurn(mBurnObjectScript);
                    }
                }
                else
                {  
                    // BurnObjectScript�� �ҿű�� �۾��� ��ҽ�Ų��.
                    if (dstBurnObject->IsProcessing() == true &&
                        dstBurnObject->GetDestObject() == mBurnObjectScript)
                    {
                        dstBurnObject->CancleProcess();
                    }
                    // TODO : �ִϸ��̼��� IDLE�� ����
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
    {   // Animator������Ʈ ����, ������ �߰�
        mAnimator = gameObject->GetComponent<Animator>();
        if (mAnimator == nullptr)
            mAnimator = gameObject->AddComponent<Animator>();
    }
    //mAnimator->AddAnimation();
}

// ��ȯ ���� float�� ������, pad�� ���̽�ƽ�� 0~1�� ���� ��ȯ�ϱ� ����
// ���̽�ƽ�� ���� 1.0f�� ��ȯ
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
