#include "pch.h"
#include "OnOffButtonScript.h"

OnOffButtonScript::OnOffButtonScript(Object* _owner)
	: ButtonScript(_owner), otherButton(nullptr), isProcessing(false)
{
    
}

void OnOffButtonScript::Start()
{
    gameObject->AddComponent<Rigidbody>();

    BoxCollider* boxCol = gameObject->AddComponent<BoxCollider>();
    boxCol->SetPosition(Vector3(0, 120.0f, 0));
    Vector3 scale = gameObject->transform->GetWorldScale();
    boxCol->SetExtents(Vector3(70, 8, 70) * scale);

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
void OnOffButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    Display::Console::Log("OnOffCollision Enter\n");

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

void OnOffButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
}

void OnOffButtonScript::OnButtonPressed()
{
    if (isProcessing) return; // 이미 처리 중이면 반환
    isProcessing = true; // 처리 중으로 설정

    if (otherButton)
    {
        // 상대 버튼 올리기
        otherButton->OnButtonReleased();
		otherButton->isUp.val = true;
        Display::Console::Log(otherButton->gameObject->GetTag(), "Up!\n");

    }

    // 현재 버튼 내리기
    MoveButton(-80.0f, 1.0f);

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
    MoveButton(80.0f, 1.0f);
    isProcessing = false; // 처리 완료
}

bool OnOffButtonScript::CanInteract(Object* _object)
{
    return true;
}

std::wstring OnOffButtonScript::GetOtherButtonTag()
{
	// 현재 버튼의 태그를 기반으로 상대 버튼의 태그 반환 (1_1밟으면 1_2반환, 2_2밟으면 2_1반환...)
    // ex) OnOff1_1은 OnOff1_2와 짝이고, OnOff2_1은 OnOff2_2와 짝이고 ... _1 과 _2로 짝지어지게 태그를 지어주면 됨미다
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