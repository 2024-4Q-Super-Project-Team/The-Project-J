#pragma once
#include "../TestApp.h"
#include "Test.h"

class TestScript 
	: public Component::Script
{
public:
	using::Component::Script::Script;
public:
	virtual void Start() override;
	virtual void Update() override;
private:
	std::vector<Test*> mScripts;
};

