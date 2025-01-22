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

	if (mImgInfo.filepath.empty())
		return;

	handle.mPath = mImgInfo.filepath;
	ResourceManager::RegisterResourceHandle(handle);
	mImgInfo.texture = ResourceManager::Alloc_Resource<Texture2DResource>(handle);

	m_pSpriteBatch = new SpriteBatch(D3DGraphicsRenderer::GetDevicecontext());
}

void WidgetImage::Update()
{
	
}

void WidgetImage::PreRender()
{
	m_pSpriteBatch->Begin();
}

void WidgetImage::Render()
{
	m_pSpriteBatch->Draw(mImgInfo.texture->Texture->mSRV, mPosition);
}

void WidgetImage::PostRender()
{
	m_pSpriteBatch->End();
}