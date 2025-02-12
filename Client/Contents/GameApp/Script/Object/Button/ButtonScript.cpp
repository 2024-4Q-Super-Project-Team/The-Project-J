#include "pch.h"
#include "ButtonScript.h"

#define BUTTON_FSX_PRESSED L"pressed"

ButtonScript::ButtonScript(Object* _owner)
    : MonoBehaviour(_owner, "ButtonScript")
{
}

void ButtonScript::AudioInit()
{
    mAudioSource = gameObject->AddComponent<AudioSource>();
    ResourceHandle ButtonSoundHandle;
    ButtonSoundHandle.mResourceType = eResourceType::AudioResource;
    ButtonSoundHandle.mMainKey = L"SFX_button";
    ButtonSoundHandle.mPath = L"resource/sound/SFX_button.mp3";
    if (ResourceManager::GetResource<AudioResource>(ButtonSoundHandle) == nullptr)
    {
        ResourceManager::LoadFileFromHandle(ButtonSoundHandle);
    }
    mAudioSource->AddAudio(BUTTON_FSX_PRESSED, ButtonSoundHandle);
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

void ButtonScript::PlayPressedSound()
{
    mAudioSource->Play(BUTTON_FSX_PRESSED);
}

void ButtonScript::MoveButton(float yOffset, float duration)
{
    Transform* transform = gameObject->transform;
    if (transform)
    {
        Vector3 startPosition = transform->position;
        Vector3 endPosition = startPosition;
        endPosition.y += yOffset;

        transform->MoveTo(endPosition, duration, Dotween::EasingEffect::OutSine);
    }
}
