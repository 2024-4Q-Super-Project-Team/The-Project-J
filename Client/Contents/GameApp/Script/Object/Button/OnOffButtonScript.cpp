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
void OnOffButtonScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    Display::Console::Log("OnOffCollision Enter\n");

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

void OnOffButtonScript::OnCollisionExit(Rigidbody* _origin, Rigidbody* _destination)
{
}

void OnOffButtonScript::OnButtonPressed()
{
    if (isProcessing) return; // �̹� ó�� ���̸� ��ȯ
    isProcessing = true; // ó�� ������ ����

    if (otherButton)
    {
        // ��� ��ư �ø���
        otherButton->OnButtonReleased();
		otherButton->isUp.val = true;
        Display::Console::Log(otherButton->gameObject->GetTag(), "Up!\n");

    }

    // ���� ��ư ������
    MoveButton(-80.0f, 1.0f);

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
    MoveButton(80.0f, 1.0f);
    isProcessing = false; // ó�� �Ϸ�
}

bool OnOffButtonScript::CanInteract(Object* _object)
{
    return true;
}

std::wstring OnOffButtonScript::GetOtherButtonTag()
{
	// ���� ��ư�� �±׸� ������� ��� ��ư�� �±� ��ȯ (1_1������ 1_2��ȯ, 2_2������ 2_1��ȯ...)
    // ex) OnOff1_1�� OnOff1_2�� ¦�̰�, OnOff2_1�� OnOff2_2�� ¦�̰� ... _1 �� _2�� ¦�������� �±׸� �����ָ� �ʹ̴�
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