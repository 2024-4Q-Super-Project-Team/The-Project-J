#include "pch.h"
#include "WidgetImage.h"

WidgetImage::WidgetImage()
{

}

WidgetImage::~WidgetImage()
{
	m_pSpriteBatch = nullptr;
}

void WidgetImage::Start()
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

void WidgetImage::Update()
{
	
}

void WidgetImage::Render()
{
	m_pSpriteBatch->Begin();
	m_pSpriteBatch->Draw(mTexture->Texture->mSRV, mPosition);
	m_pSpriteBatch->End();
}