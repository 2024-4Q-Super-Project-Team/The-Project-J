#include "pch.h"
#include "PlayerScript.h"

void PlayerScript::Start()
{
    // �ʱ�ȭ �ڵ�
}

void PlayerScript::Update()
{
    // ������Ʈ �ڵ�
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
    //    // ��ư�� ������ ���� ������ �߰��մϴ�.
    //    button->Press();
    //}
}

void PlayerScript::OnCollisionExit(Collider* _origin, Collider* _destination)
{
    //if (auto* button = _destination->GetOwner()->GetComponent<Button>())
    //{
    //    // ��ư�� �������� ���� ������ �߰��մϴ�.
    //    button->Release();
    //}
}