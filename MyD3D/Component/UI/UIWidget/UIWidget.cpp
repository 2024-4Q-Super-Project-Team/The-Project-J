#include "pch.h"
#include "Component/UI/UIWidget/UIWidget.h"

#include "Component/Camera/Camera.h"
#include "Component/UI/Canvas.h"
#include "Manager/UIManager.h"

/*
	DXGI_MODE_ROTATION

	DXGI_MODE_ROTATION_UNSPECIFIED = 0,
	DXGI_MODE_ROTATION_IDENTITY = 1,
	DXGI_MODE_ROTATION_ROTATE90 = 2,
	DXGI_MODE_ROTATION_ROTATE180 = 3,
	DXGI_MODE_ROTATION_ROTATE270 = 4
*/

UIWidget::UIWidget(Object* _owner)
	: Component(_owner)
{

}

UIWidget::~UIWidget()
{
	//if (m_pTexture != nullptr)
	//{
	//	delete m_pTexture;
	//	m_pTexture = nullptr;
	//}
}

void UIWidget::Tick()
{
}

void UIWidget::FixedUpdate()
{
}

void UIWidget::PreUpdate()
{
	if (!gameObject->transform->GetParent())
		return;

	Canvas* canvas = 
		gameObject->transform->GetParent()->gameObject->GetComponent<Canvas>();

	if (canvas)
	{
		canvas->mWidgets.push_back(this);
	}
}

void UIWidget::Update()
{
}

void UIWidget::PostUpdate()
{
}

void UIWidget::PreRender()
{
}

void UIWidget::Render()
{
}

void UIWidget::Draw(Camera* _camera)
{
}

void UIWidget::PostRender()
{
}

void UIWidget::SetTexture(ResourceHandle _handle)
{
	m_pTexture = ResourceManager::GetResource<Texture2DResource>(_handle);

	if (m_pTexture)
	{
		// 이미지 크기 설정
		gameObject->transform->scale.x = m_pTexture->Texture->mWidth;
		gameObject->transform->scale.y = m_pTexture->Texture->mHeight;
	}
}

void UIWidget::EditorUpdate()
{
}

void UIWidget::EditorRender()
{

}

void UIWidget::EditorRendering(EditorViewerType _viewerType)
{

}
