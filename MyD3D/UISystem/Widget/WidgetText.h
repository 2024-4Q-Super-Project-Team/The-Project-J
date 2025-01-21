#pragma once
#include "Widget.h"

class WidgetText

	: public Widget
{
public:
	WidgetText();
	virtual ~WidgetText(); // 버츄얼로 돌려야되는데 왜 안됨?

public:
	virtual void Update();
	virtual void Render();
public:
	
private:
	
};

