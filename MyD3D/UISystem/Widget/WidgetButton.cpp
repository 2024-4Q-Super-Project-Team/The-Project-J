#include "pch.h"
#include "WidgetButton.h"

WidgetButton::WidgetButton()
{

}

WidgetButton::~WidgetButton()
{
	m_pSpriteBatch = nullptr;
}

void WidgetButton::Start()
{
	ResourceHandle handle;
	handle.mMainKey = mID;

	if (mFilepath.empty())
		return;

	handle.mPath = mFilepath;
	ResourceManager::RegisterResourceHandle(handle);
	mTexture = ResourceManager::Alloc_Resource<Texture2DResource>(handle);

	m_pSpriteBatch = new SpriteBatch(D3DGraphicsRenderer::GetDevicecontext());
}

void WidgetButton::Update()
{

}

void WidgetButton::Render()
{
	m_pSpriteBatch->Begin();
	m_pSpriteBatch->Draw(mTexture->Texture->mSRV, mPosition);
	m_pSpriteBatch->End();
}
