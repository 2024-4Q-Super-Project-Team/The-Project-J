#include "pch.h"
#include "PlayerScript.h"

void PlayerScript::Start()
{
    // 초기화 코드
}

void PlayerScript::Update()
{
    // 업데이트 코드
}

void PlayerScript::OnCollisionEnter(Collider* _origin, Collider* _destination)
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
    //    // 버튼이 눌렸을 때의 동작을 추가합니다.
    //    button->Press();
    //}
}

void PlayerScript::OnCollisionExit(Collider* _origin, Collider* _destination)
{
    //if (auto* button = _destination->GetOwner()->GetComponent<Button>())
    //{
    //    // 버튼이 떼어졌을 때의 동작을 추가합니다.
    //    button->Release();
    //}
}