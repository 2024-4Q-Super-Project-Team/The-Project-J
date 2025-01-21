#include "pch.h"
#include "TestWorld.h"
#include "../Script/ModelSpawner.h"
#include "../Script/CameraController.h"

void _CALLBACK TestWorld::OnCreate()
{
    CreateObjectGroup(L"Default", L"");
    CreateObjectGroup(L"Test", L"");

    return void _CALLBACK();
}

void _CALLBACK TestWorld::OnDestroy()
{
    return void _CALLBACK();
}

void _CALLBACK TestWorld::OnEnable()
{   
    ObjectGroup* pGroup = GetObjectGroup(L"Test");
    if (pGroup)
    {
        Object* clone;
        clone = pGroup->CreateObject(L"ModelController", L"Default");
        clone->AddComponent<ModelSpawner>();

        clone = pGroup->CreateObject(L"CameraController", L"Default");
        clone->AddComponent<CameraController>();
    }
    return void _CALLBACK();
}
