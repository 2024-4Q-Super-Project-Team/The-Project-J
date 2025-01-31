#include "pch.h"
#include "JumpPadScript.h"
#include "Component/Collider/Collider.h"
#include "Component/Controller/PlayerController.h"

void JumpPadScript::Start()
{
}

void JumpPadScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	Display::Console::Log("Jump!");

    if (_destination->GetOwner()->GetComponent<PlayerController>())
    {
        auto* playerController = _destination->GetOwner()->GetComponent<PlayerController>();
        playerController->SetJumpSpeed(mJumpForce.val);
        playerController->StartJump();
		Display::Console::Log("Jump!");
    }
}

json JumpPadScript::Serialize()
{
	json ret = MonoBehaviour::Serialize();

	ret["jump force"] = mJumpForce.val;
	
	return ret;
}

void JumpPadScript::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);

	mJumpForce.val = j["jump force"].get<float>();
}
