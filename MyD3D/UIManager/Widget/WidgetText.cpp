#include "pch.h"
#include "WidgetText.h"

WidgetText::WidgetText(Object* _owner)
	: Widget(_owner)
{
	memset(mFormat, 0, sizeof(mFormat));
	mUIType = eUIType::TEXT;
}

WidgetText::~WidgetText()
{

}

void WidgetText::Draw(Vector2 _scale)
{
	Vector3 objPos = gameObject->transform->GetWorldPosition();

	if (bUseOutline)
	{
		OutlinedTextRender(_scale, objPos);
	}

	m_pSpriteFont->DrawString(UIManager::GetSpriteBatch(), mFormat, (objPos.x, objPos.y) * _scale, mColor, 0.f, Vector2(0.0f, 0.0f), _scale.x);
}

void WidgetText::OutlinedTextRender(Vector2 _scale, Vector3 _objPos) {
	
	// 외곽선 오프셋 (4방향)
	Vector2 offsets[] = {
		{-mOutlineOffset, 0.0f}, {mOutlineOffset, 0.0f}, {0.0f, -mOutlineOffset}, {0.0f, mOutlineOffset}
	};

	// 외곽선 그리기
	for (auto& offset : offsets) {
		Vector2 outlinePos = Vector2(_objPos.x + offset.x, _objPos.y + offset.y) * _scale;
		m_pSpriteFont->DrawString(UIManager::GetSpriteBatch(), mFormat, outlinePos, mOutlineColor, 0.f, Vector2(0.0f, 0.0f), _scale.x);
	}
}

void WidgetText::Release()
{
	if (m_pSpriteFont != nullptr)
	{
		delete m_pSpriteFont;
		m_pSpriteFont = nullptr;
	}
}

void WidgetText::SetFont(std::wstring _path)
{
	if (!_path.empty())
	{
		m_pSpriteFont = new SpriteFont(D3DGraphicsDevice::GetDevice(), _path.c_str());
	}
}

void WidgetText::SetTextLine(float _line)
{
	m_pSpriteFont->SetLineSpacing(_line);
}

void WidgetText::SetTextDefault(char _defaultText)
{
	m_pSpriteFont->SetDefaultCharacter(_defaultText);
}

void WidgetText::SetTextFormat(const wchar_t* _msg, ...)
{
	// 가변 인자 처리
	va_list args;
	va_start(args, _msg);
	vswprintf_s(mFormat, sizeof(mFormat) / sizeof(wchar_t), _msg, args);
	va_end(args);
}

void WidgetText::SetTextOutlineOffset(float _offset)
{
	mOutlineOffset = _offset;
}

void WidgetText::SetTextOutlineColor(Color _color)
{
	mOutlineColor = _color;
}
