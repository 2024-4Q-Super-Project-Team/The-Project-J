#pragma once
#include <vector>
#include "../MyEngineDX/Component/Script/Script.h"

class Test;

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

