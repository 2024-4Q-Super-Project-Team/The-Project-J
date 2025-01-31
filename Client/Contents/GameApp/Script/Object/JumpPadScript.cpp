#include "pch.h"
#include "JumpPadScript.h"
#include "Component/Collider/Collider.h"
#include "Component/Controller/PlayerController.h"

void JumpPadScript::Start()
{
}

void JumpPadScript::OnCollisionEnter(Collider* _origin, Collider* _destination)
{
    if (_destination->GetOwner()->GetComponent<PlayerController>())
    {
        auto* playerController = _destination->GetOwner()->GetComponent<PlayerController>();
        playerController->SetJumpSpeed(mJumpForce);
        playerController->StartJump();
		Display::Console::Log("Jump!");
    }
}