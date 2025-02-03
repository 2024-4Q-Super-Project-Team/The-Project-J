#include "pch.h"
#include "OnOffButtonScript.h"

OnOffButtonScript::OnOffButtonScript(Object* _owner)
	: ButtonScript(_owner), otherButton(nullptr), isProcessing(false)
{
    
}

void OnOffButtonScript::Start()
{
    // ��� ��ư ã��
    std::wstring otherButtonTag = GetOtherButtonTag();
    auto objects = FindObjectsWithTag(otherButtonTag);
    if (!objects.empty())
    {
        for (auto obj : objects)
        {
            // obj�� ���� �ƴϸ� 
            if (obj != gameObject)
            {
                otherButton = obj->GetComponent<OnOffButtonScript>();
            }
        }
    }
}
void OnOffButtonScript::OnButtonPressed()
{
    if (isProcessing) return; // �̹� ó�� ���̸� ��ȯ
    isProcessing = true; // ó�� ������ ����

    if (otherButton)
    {
        // ��� ��ư �ø���
        otherButton->OnButtonReleased();
        Display::Console::Log(otherButton->gameObject->GetTag(), "Button Up!\n");

    }

    // ���� ��ư ������
    MoveButton(-150.0f, 1.0f);

    isProcessing = false; // ó�� �Ϸ�
}

void OnOffButtonScript::OnButtonReleased()
{
    if (isProcessing) return; // �̹� ó�� ���̸� ��ȯ
    isProcessing = true; // ó�� ������ ����

    if (otherButton)
    {
        // ��� ��ư ������
        otherButton->OnButtonPressed();
    }

    // ���� ��ư �ø���
    MoveButton(150.0f, 1.0f);
    isProcessing = false; // ó�� �Ϸ�
}

void OnOffButtonScript::OnTriggerEnter(Collider* _origin, Collider* _destination)
{
    Object* interactingObject = _destination->GetOwner();
    if (CanInteract(interactingObject))
    {
        if (isUp.val)
        {
            OnButtonPressed(); // ��� ��ư �ø���, �� ��ư ������
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
//			OnButtonReleased(); // �� ��ư �ø���
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
	// ���� ��ư�� �±׸� ������� ��� ��ư�� �±� ��ȯ (1_1������ 1_2��ȯ, 2_2������ 2_1��ȯ...)
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