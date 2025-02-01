#include "pch.h"
#include "PlayerScript.h"
#include "PlayerState.h"

void PlayerScript::Start()
{
    // 초기화 코드
	// 게임오브젝트에 FSM을 추가하고, FSM에 상태를 추가합니다.
	//auto fsm = gameObject->AddComponent<FiniteStateMachine>();
	//fsm->AddState<State::Player_Run>(L"Player_Run");
	//// 초기 상태를 설정합니다.
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