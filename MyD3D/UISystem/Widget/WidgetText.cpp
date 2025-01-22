#include "pch.h"
#include "WidgetText.h"

WidgetText::WidgetText()
{

}

WidgetText::~WidgetText()
{
	m_pSpriteFont = nullptr;
	m_pSpriteBatch = nullptr;
}

void WidgetText::Start()
{
	m_pSpriteBatch = new SpriteBatch(D3DGraphicsRenderer::GetDevicecontext());
	m_pSpriteFont = new SpriteFont(D3DGraphicsDevice::GetDevice(), mTextInfo.filepath.c_str());

	m_pSpriteFont->SetLineSpacing(mTextInfo.line);
	m_pSpriteFont->SetDefaultCharacter(mTextInfo.defaultText);
}

void WidgetText::Update()
{
	
}

void WidgetText::PreRender()
{
	m_pSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void WidgetText::Render()
{
	m_pSpriteFont->DrawString(m_pSpriteBatch, mTextInfo.msg, mPosition, mTextInfo.color);
}

void WidgetText::PostRender()
{
	m_pSpriteBatch->End();
}

void WidgetText::SetTextFormat(const wchar_t* _msg, ...)
{
	// 가변 인자 처리
	va_list args;
	va_start(args, _msg);
	vswprintf(mTextInfo.msg, sizeof(mTextInfo.msg) / sizeof(wchar_t), _msg, args);
	va_end(args);
}
