#pragma once
#include "Widget.h"

class WidgetImage
	: public Widget
{
public:
	WidgetImage();
	virtual ~WidgetImage(); // ������ �����ߵǴµ� �� �ȵ�?
public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
private:
};

