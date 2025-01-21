#pragma once
#include "Widget.h"

class WidgetButton
	: public Widget
{
public:
	WidgetButton();
	virtual ~WidgetButton(); // 버츄얼로 돌려야되는데 왜 안됨?

public:
	virtual void Update()	override;
	virtual void Render()	override;
public:
	bool IsEnter() { return bIsEnter; }
	bool OnClick() { return bIsEnter && Input::IsMouseDown(Mouse::LEFT); }
	bool OnHold() { return bIsEnter && Input::IsMouseHold(Mouse::LEFT); }
private:
	bool bIsEnter = false;
};