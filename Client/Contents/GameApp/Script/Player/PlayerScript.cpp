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
        // JH TODO : �� �޽� �־��ֱ�
        mBurnObjectScript->SetBurnMesh(nullptr);
    }
}

void PlayerScript::Update()
{
    // HP ����
    UpdatePlayerHP();
    UpdateInput();
    if (isAction)
    {
        // ��Ʈ�ѷ� �������� ���ϰ�
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
    // �� �ű�� �ý���
    // [02/02 ~ ] �۾��� : ���� (�⺻ ����)
    ////////////////////////////////////////////////
    // ���� �پ����� ��
    if (mBurnObjectScript->IsBurning())
    {
        // BurnObjectScript�� GetComponent�����߳ķ� ����� ���� �ű� �� �ִ� ������Ʈ �ΰ��� ����
        BurnObjectScript* dstBurnObject = _destination->gameObject->GetComponent<BurnObjectScript>();
        // dstBurnObject�� �°�, dstBurnObject�� ���� �پ����� �ʴ� ���
        if (dstBurnObject && dstBurnObject->IsBurning() == false)
        {
            // JH TODO : ĳ����(gameO-bject)�� �Ӹ� ���� �� �ű�� UI �˾�

            // �� �ű�� Ű�� ������ ��
            if (InputSyncer::IsKeyHold(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
            {
                // BurnObjectScript�� �ҿű�� �۾��� �Ѵ�.
                if (dstBurnObject->IsProcessing() == false)
                {
                    dstBurnObject->ProcessBurn(mBurnObjectScript);
                    mPlayerState = ePlayerStateType::MOVE_FIRE;
                }
            }
            else  if (InputSyncer::IsKeyUp(mPlayerHandle.val, InputSyncer::MOVE_FIRE))
            {
                // BurnObjectScript�� �ҿű�� �۾��� ��ҽ�Ų��.
                if (dstBurnObject->IsProcessing() == true &&
                    dstBurnObject->GetDestObject() == mBurnObjectScript)
                {
                    dstBurnObject->CancleProcess();
                }
                // �ִϸ��̼� Idle���
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
        // �ǰ� �ִϸ��̼��� ������� ���ȿ��� �����̴�.
        if (mAnimator->GetActiveAnimationKey() != L"Hit")
        {
            mAnimator->SetCurrentAnimation(L"Hit");
            mPlayerCurHP -= _damage;
        }
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
            mPlayerState = ePlayerStateType::DEAD;
            // �ִϸ��̼� Dead���
            mAnimator->SetCurrentAnimation(L"Dead");
            mAnimator->SetLoop(false);
        }
    }
}

void PlayerScript::UpdateInput()
{
    Vector2 moveDirection = InputSyncer::GetInputDirection(mPlayerHandle.val);
    Vector2 moveForce = Vector2::Zero;
    // ��ǲ�� ���� Direction���� �ִٰ� �����Ǹ�
    if (moveDirection != Vector2::Zero)
    {
        moveForce.x = moveDirection.x * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
        moveForce.y = moveDirection.y * mMoveSpeed.val * Time::GetUnScaledDeltaTime();
    }
    mPlayerController->SetMoveForceX(moveForce.x);
    mPlayerController->SetMoveForceZ(moveForce.y);
    if (mPlayerController->IsGround() == true && isAction == false)
    {
        // ���� ���� �ƴ� �� ����Ű�� ������ ����
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
