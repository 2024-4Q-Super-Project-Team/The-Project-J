#pragma once
#include "Widget.h"

class WidgetButton
	: public Widget
{
public:
	WidgetButton();
	virtual ~WidgetButton(); // 버츄얼로 돌려야되는데 왜 안됨?
public:
	virtual void Init()						override;
	virtual void PreUpdate()				override;
	virtual void Update()					override;
	virtual void Render(Vector2 _scale)		override;
	virtual void Release()					override;
public:
	bool IsEnter()		{ return bIsEnter; }
	bool OnClick()		{ return bIsEnter && Input::IsMouseDown(Mouse::LEFT); }
	bool OnHold()		{ return bIsEnter && Input::IsMouseHold(Mouse::LEFT); }
	bool OnRelease()	{ return bIsEnter && Input::IsMouseUp(Mouse::LEFT); }
	bool OnExit()		{ return !bIsEnter; }
private:
	bool bIsEnter = false;
};