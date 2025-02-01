#include "pch.h"
#include "JumpPadScript.h"
#include "Component/Collider/Collider.h"
#include "Component/Controller/PlayerController.h"

void JumpPadScript::Start()
{
}

void JumpPadScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
	Display::Console::Log("Jump!");

    if (_destination->gameObject->GetComponent<PlayerController>())
    {
        auto* playerController = _destination->gameObject->GetComponent<PlayerController>();
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
