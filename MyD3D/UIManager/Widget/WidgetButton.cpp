#include "pch.h"
#include "WidgetButton.h"

WidgetButton::WidgetButton()
{

}

WidgetButton::~WidgetButton()
{
	m_pSpriteBatch = nullptr;
}

void WidgetButton::Init()
{
	Widget::Init();

	ResourceHandle handle;
	handle.mMainKey = mID;

	if (mFilepath.empty())
		return;

	handle.mPath = mFilepath;
	ResourceManager::RegisterResourceHandle(handle);
	m_pTexture = ResourceManager::Alloc_Resource<Texture2DResource>(handle);
}

void WidgetButton::Update()
{

}

void WidgetButton::Render()
{
	m_pSpriteBatch->Begin();
	m_pSpriteBatch->Draw(m_pTexture->Texture->mSRV, mPosition);
	m_pSpriteBatch->End();
}

void WidgetButton::Release()
{
	Widget::Release();
}
