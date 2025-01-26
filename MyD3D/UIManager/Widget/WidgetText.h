#pragma once
#include "Widget.h"

class WidgetText
	: public Widget
{
public:
	WidgetText(Object* _owner);
	virtual ~WidgetText();
public:
	virtual void DrawWidget(Vector2 _scale)	override;
public:
	void SetFont(std::wstring _path);
	void SetTextLine(float _line);
	void SetTextDefault(char _defaultText);
	void SetTextFormat(const wchar_t* _msg, ...);
	void SetTextOutlineOffset(float _offset);
	void SetTextOutlineColor(Color _color);
public:
	std::wstring GetTextFormat()	{ return mFormat; }
private:
	SpriteFont* m_pSpriteFont = nullptr;
	wchar_t mFormat[1024];	// �ؽ�Ʈ ����

	// �ܰ��� ����
	void OutlinedTextRender(Vector2 _scale, Vector3 _objPos);
	// �ܰ��� ���� ������
	bool bUseOutline = false;
	float mOutlineOffset = 1.f;
	Color mOutlineColor{};
};

