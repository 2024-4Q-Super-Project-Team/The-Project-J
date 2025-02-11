#include "pch.h"
#include "JumpPadScript.h"
#include "Component/Collider/Collider.h"
#include "Contents/GameApp/Script/Player/PlayerScript.h"

void JumpPadScript::Start()
{
}

void JumpPadScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
}

void JumpPadScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
	Object* interactingObject = _destination->gameObject;
	PlayerScript* playerScript = interactingObject->GetComponent<PlayerScript>();

	if (playerScript)
	{
		Display::Console::Log("jump!");
		playerScript->Jump(mJumpScale.val);
	}
}

void JumpPadScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
	Display::Console::Log("exit!");

}

json JumpPadScript::Serialize()
{
	json ret = MonoBehaviour::Serialize();

	ret["id"] = GetId();
	ret["name"] = "JumpPadScript";
	ret["jump scale"] = mJumpScale.val;
	
	return ret;
}

void JumpPadScript::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);

	if (j.contains("jump scale"))
	{
		mJumpScale.val = j["jump scale"].get<float>();
	}
}
