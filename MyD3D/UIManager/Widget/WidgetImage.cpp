#include "pch.h"
#include "WidgetImage.h"

WidgetImage::WidgetImage(Object* _owner)
	: Widget(_owner)
{
	mUIType = eUIType::IMAGE;
}

WidgetImage::~WidgetImage()
{

}

void WidgetImage::Draw(Vector2 _scale)
{
	RECT rect{};
	rect.left = gameObject->transform->position.x * _scale.x;
	rect.top = gameObject->transform->position.y * _scale.y;
	rect.right = rect.left + (gameObject->transform->scale.x * _scale.x);
	rect.bottom = rect.top + (gameObject->transform->scale.y * _scale.y);

	UIManager::GetSpriteBatch()->Draw(m_pTexture->Texture->mSRV, rect, nullptr, mColor);
}

void WidgetImage::Release()
{
	if (m_pTexture != nullptr)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}
}
