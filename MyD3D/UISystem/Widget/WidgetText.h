#pragma once
#include "Widget.h"

struct TextInfo
{
	std::wstring filepath = L"";
	float line = 10.f;
	char defaultText = '_';
	Color color{};
	wchar_t msg[1024];
};

class WidgetText
	: public Widget
{
public:
	WidgetText();
	virtual ~WidgetText(); // 버츄얼로 돌려야되는데 왜 안됨?

public:
	virtual void Start();
	virtual void Update();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
public:
	void SetTextFilename(const std::wstring _path) { mTextInfo.filepath = _path; }
	void SetTextLine(float _line) { mTextInfo.line = _line; }
	void SetTextDefault(char _defaultText) { mTextInfo.defaultText = _defaultText; }
	void SetTextColor(Color _color) { mTextInfo.color = _color; }
	void SetTextPosition(Vector2 _position) { SetPosition(_position); }
	void SetTextFormat(wchar_t* _msg, ...);
private:
	SpriteFont* m_pSpriteFont = nullptr;
	SpriteBatch* m_pSpriteBatch = nullptr;

	TextInfo mTextInfo;
};

