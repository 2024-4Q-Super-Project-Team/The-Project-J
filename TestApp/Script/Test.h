#pragma once
#include "../TestApp.h"

class Test 
	: public Component::Script
{
public:
	using::Component::Script::Script;
public:
	virtual void Start() override;
public:
	void AddCount(int _num) { counter += _num; }
private:
	int counter = 0;
};

