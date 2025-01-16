#include "pch.h"
#include "EditorWorld.h"
#include "../Script/EditorManager.h"

void _CALLBACK EditorWorld::OnCreate()
{
    ObjectGroup* group = CreateObjectGroup(L"Editor", L"");
    Object* obj = group->CreateObject(L"EditorManager", L"");
    EditorManager* em = obj->AddComponent<EditorManager>();
    em->Start(); //임시로 하드코딩했는데, EditorManager는 엔진으로 가서 먼저 실행되어야 합니다. 
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
