#include "pch.h"
#include "EditorWorld.h"
#include "../Script/EditorManager.h"

void _CALLBACK EditorWorld::OnCreate()
{
    ObjectGroup* group = CreateObjectGroup(L"Editor", L"");
    Object* obj = group->CreateObject(L"EditorManager", L"");
    EditorManager* em = obj->AddComponent<EditorManager>();
    em->Start(); //�ӽ÷� �ϵ��ڵ��ߴµ�, EditorManager�� �������� ���� ���� ����Ǿ�� �մϴ�. 
    em->Wake();
    return void _CALLBACK();
}

void _CALLBACK EditorWorld::OnDestroy()
{
    return void _CALLBACK();
}

void _CALLBACK EditorWorld::OnTick()
{
    //mOwnerScene->GetIWindow()->SetPosition();
    return void _CALLBACK();
}

void _CALLBACK EditorWorld::OnUpdate()
{


    return void _CALLBACK();
}

void _CALLBACK EditorWorld::OnRender()
{


    return void _CALLBACK();
}
