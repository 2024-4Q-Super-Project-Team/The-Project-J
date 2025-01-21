#pragma once
#include "Widget.h"

class WidgetCheckBox 
	: public Widget
{
public:
	WidgetCheckBox();
	virtual ~WidgetCheckBox(); // ������ �����ߵǴµ� �� �ȵ�?

public:
	virtual void Update();
	virtual void Render();
public:
	bool IsCheck() { return bIsCheck; }
	bool OnClick() { return bIsCheck && Input::IsMouseDown(Mouse::LEFT); }
	bool OnExit() { return !bIsCheck && Input::IsMouseDown(Mouse::LEFT); }
private:
	bool bIsCheck = false;
};

