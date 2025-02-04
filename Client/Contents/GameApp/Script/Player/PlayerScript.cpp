#include "pch.h"
#include "PlayerScript.h"
#include "Contents/GameApp/Script/Object/Burn/BurnObjectScript.h"
void PlayerScript::Start()
{
    // �ʱ�ȭ �ڵ�
    gameObject->SetTag(L"Player");

    {
        mAnimator = gameObject->GetComponent<Animator>();
    }
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
    if (isAction)
    {
        // ��Ʈ�ѷ� �������� ���ϰ�
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
                if(InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
                {
                    // BurnObjectScript�� �ҿű�� �۾��� �Ѵ�.
                    if (dstBurnObject->IsProcessing() == false)
                    {
                        dstBurnObject->ProcessBurn(mBurnObjectScript);
                        mPlayerState = ePlayerStateType::MOVE_FIRE;
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
                    // �ִϸ��̼��� IDLE�� ����
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
