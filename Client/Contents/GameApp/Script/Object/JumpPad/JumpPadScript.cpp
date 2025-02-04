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
        //playerController->SetJumpSpeed(mJumpForce.val);
        //playerController->StartJump(); TODO: 점프 기능 클라로 뺐음. 따로 점프스크립트 구현필요.
		Display::Console::Log("Jump!");
    }
}

json JumpPadScript::Serialize()
{
	json ret = MonoBehaviour::Serialize();

	ret["id"] = GetId();
	ret["name"] = "JumpPadScript";
	ret["jump force"] = mJumpForce.val;
	
	return ret;
}

void JumpPadScript::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);

	if (j.contains("jump force"))
	{
		mJumpForce.val = j["jump force"].get<float>();
	}
}
