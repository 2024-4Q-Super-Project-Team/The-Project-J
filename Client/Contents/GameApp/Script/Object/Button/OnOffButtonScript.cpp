#include "pch.h"
#include "OnOffButtonScript.h"

OnOffButtonScript::OnOffButtonScript(Object* _owner)
	: ButtonScript(_owner), otherButton(nullptr), isProcessing(false)
{
    
}

void OnOffButtonScript::Start()
{
    // 상대 버튼 찾기
    std::wstring otherButtonTag = GetOtherButtonTag();
    auto objects = FindObjectsWithTag(otherButtonTag);
    if (!objects.empty())
    {
        for (auto obj : objects)
        {
            // obj가 지가 아니면 
            if (obj != gameObject)
            {
                otherButton = obj->GetComponent<OnOffButtonScript>();
            }
        }
    }
}
void OnOffButtonScript::OnButtonPressed()
{
    if (isProcessing) return; // 이미 처리 중이면 반환
    isProcessing = true; // 처리 중으로 설정

    if (otherButton)
    {
        // 상대 버튼 올리기
        otherButton->OnButtonReleased();
        Display::Console::Log(otherButton->gameObject->GetTag(), "Button Up!\n");

    }

    // 현재 버튼 내리기
    MoveButton(-150.0f, 1.0f);

    isProcessing = false; // 처리 완료
}

void OnOffButtonScript::OnButtonReleased()
{
    if (isProcessing) return; // 이미 처리 중이면 반환
    isProcessing = true; // 처리 중으로 설정

    if (otherButton)
    {
        // 상대 버튼 내리기
        otherButton->OnButtonPressed();
    }

    // 현재 버튼 올리기
    MoveButton(150.0f, 1.0f);
    isProcessing = false; // 처리 완료
}

void OnOffButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
    Object* interactingObject = _destination->GetOwner();
    if (CanInteract(interactingObject))
    {
        if (isUp.val)
        {
            OnButtonPressed(); // 상대 버튼 올리고, 내 버튼 내리기
            isUp.val = false;
        }
    }
}

//void OnOffButtonScript::OnTriggerExit(Collider* _origin, Collider* _destination)
//{
//    Object* interactingObject = _destination->GetOwner();
//    if (CanInteract(interactingObject))
//    {
//        if (!isUp.val)
//        {
//			OnButtonReleased(); // 내 버튼 올리기
//            isUp.val = true;
//        }
//    }
//}

void OnOffButtonScript::OnCollisionStay(Rigidbody* _origin, Rigidbody* _destination)
{
       
}

bool OnOffButtonScript::CanInteract(Object* _object)
{
    return true;
}

std::wstring OnOffButtonScript::GetOtherButtonTag()
{
	// 현재 버튼의 태그를 기반으로 상대 버튼의 태그 반환 (1_1밟으면 1_2반환, 2_2밟으면 2_1반환...)
    std::wstring currentTag = gameObject->GetTag();
    if (currentTag.find(L"_1") != std::wstring::npos)
    {
        return currentTag.replace(currentTag.find(L"_1"), 2, L"_2");
    }
    else if (currentTag.find(L"_2") != std::wstring::npos)
    {
        return currentTag.replace(currentTag.find(L"_2"), 2, L"_1");
    }
    return L"";
}

void OnOffButtonScript::MoveButton(float yOffset, float duration)
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