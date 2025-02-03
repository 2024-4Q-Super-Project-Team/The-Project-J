#include "pch.h"
#include "ButtonScript.h"

ButtonScript::ButtonScript(Object* _owner)
    : MonoBehaviour(_owner, "ButtonScript")
{
}

void ButtonScript::Start()
{
    isUp.val = false;
}

void ButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
  
}

void ButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
}

void ButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
}

void ButtonScript::OnTriggerExit(Collider* _origin, Collider* _destination)
{
}

void ButtonScript::OnButtonPressed()
{
}

void ButtonScript::OnButtonReleased()
{
}

json ButtonScript::Serialize()
{
	json ret = MonoBehaviour::Serialize();

	ret["id"] = GetId();
	ret["name"] = "ButtonScript";
	ret["isUp"] = isUp.val;

	return ret;
}

void ButtonScript::Deserialize(json& j)
{
	MonoBehaviour::Deserialize(j);

	if (j.contains("isUp"))
	{
		isUp.val = j["isUp"].get<float>();
	}
}

bool ButtonScript::CanInteract(Object* _object)
{
    return true;
}
