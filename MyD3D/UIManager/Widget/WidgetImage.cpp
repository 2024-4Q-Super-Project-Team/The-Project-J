#include "pch.h"
#include "WidgetImage.h"

WidgetImage::WidgetImage()
{

}

WidgetImage::~WidgetImage()
{
	
}

void WidgetImage::Init()
{
	Widget::Init();

	ResourceHandle handle;
	handle.mResourceType = eResourceType::Texture2DResource;
	handle.mMainKey = mID;

	if (mFilepath.empty())
		return;

	handle.mPath = mFilepath;
	ResourceManager::RegisterResourceHandle(handle);
	m_pTexture = ResourceManager::Alloc_Resource<Texture2DResource>(handle);
}

void WidgetImage::Update()
{
	m_pSpriteBatch->SetRotation(DXGI_MODE_ROTATION_IDENTITY);
}

void WidgetImage::Render()
{
	m_pSpriteBatch->Begin();
	m_pSpriteBatch->Draw(m_pTexture->Texture->mSRV, mPosition, nullptr, mColor);
	m_pSpriteBatch->End();
}

void WidgetImage::Release()
{
	Widget::Release();
}
