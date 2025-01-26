#pragma once
#include "Widget.h"

class WidgetImage
	: public Widget
{
public:
	explicit WidgetImage(Object* _owner);
	virtual ~WidgetImage();
public:
	virtual void DrawWidget(Vector2 _scale)	override;
};

