#pragma once
#include "Widget.h"

class WidgetButton
	: public Widget
{
public:
	WidgetButton();
	virtual ~WidgetButton(); // ������ �����ߵǴµ� �� �ȵ�?
public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
public:
	bool IsEnter()	{ return bIsEnter; }
	bool OnClick()	{ return bIsEnter && Input::IsMouseDown(Mouse::LEFT); }
	bool OnHold()	{ return bIsEnter && Input::IsMouseHold(Mouse::LEFT); }
private:
	bool bIsEnter = false;
};