#include "pch.h"
#include "PlayerScript.h"
#include "PlayerState.h"

void PlayerScript::Start()
{
    // �ʱ�ȭ �ڵ�
	// ���ӿ�����Ʈ�� FSM�� �߰��ϰ�, FSM�� ���¸� �߰��մϴ�.
	//auto fsm = gameObject->AddComponent<FiniteStateMachine>();
	//fsm->AddState<State::Player_Run>(L"Player_Run");
	//// �ʱ� ���¸� �����մϴ�.
	//fsm->ChangeState(L"Player_Run");
}

void PlayerScript::Update()
{
}

void PlayerScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    //if (auto* jumpPad = _destination->GetOwner()->GetComponent<JumpPad>())
    //{
    //    auto* playerController = GetOwner()->GetComponent<PlayerController>();
    //    if (playerController)
    //    {
    //        playerController->Jump(jumpPad->GetJumpForce());
    //    }
    //}
    //else if (auto* button = _destination->GetOwner()->GetComponent<Button>())
    //{
    //    button->Press();
    //}
}

void PlayerScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
    //if (auto* button = _destination->GetOwner()->GetComponent<Button>())
    //{
    //    button->Release();
    //}
}