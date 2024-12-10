#include "TestWorld.h"
#include "../Script/TestScript.h"

void _CALLBACK TestWorld::OnEnable()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnDisable()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnCreate()
{
	mGMng = TestApp::Instance->GetGameManager();
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnDestroy()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnLoad()
{
	int a = 0;
	ObjectGroup* testGroup;
	GameObject* testObject;
	testGroup = CreateObjectGroup(L"Test", L"Default");
	//testGroup = GetObjectGroup(L"Test");
	testObject = testGroup->CreateObject(L"TestObject_01", L"Default");
	testObject->AddComponent<TestScript>();
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnUnLoad()
{
	int a = 0;
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnFixedUpdate()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnPreUpdate()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnUpdate()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnPostUpdate()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnPreRender()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnRender()
{
	return void _CALLBACK();
}

void _CALLBACK TestWorld::OnPostRender()
{
	return void _CALLBACK();
}
