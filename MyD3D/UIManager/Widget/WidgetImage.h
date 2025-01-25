#pragma once
#include "Widget.h"

class WidgetImage
	: public Widget
{
public:
	WidgetImage();
	virtual ~WidgetImage(); // 버츄얼로 돌려야되는데 왜 안됨?
public:
	virtual void Init()					override;
	virtual void Update()				override;
	virtual void Render(Vector2 _scale)	override;
	virtual void Release()				override;
private:
};

