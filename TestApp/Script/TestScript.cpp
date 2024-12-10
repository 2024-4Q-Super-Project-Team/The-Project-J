#include "TestScript.h"


void TestScript::Start()
{
	//ownerObject->GetComponent<TestScript>()[1];
	for (int i = 0; i < 3; i++)
	{
		mScripts.push_back(ownerObject->AddComponent<Test>());
	}

	TestScript* tc = ownerObject->GetComponent<TestScript>();

	Test* a; 
	Test* b; 
	Test* c; 
	
	std::vector<Test*>& testArr = ownerObject->GetComponents<Test>();
	a = testArr[0];
	b = testArr[1];
	c = testArr[2];

}

void TestScript::Update()
{
	for (auto& a : mScripts)
	{
		a->AddCount(1);
	}
}
