#pragma once
#include "Component/Component.h"
#include "Resource/Resource.h"

class Transform;

// ���� Ÿ��
enum class eUIType
{
	NON,	// ����
	IMAGE,	// �̹���
	TEXT,	// �ؽ�Ʈ
};

class IWidgetContext
{
public:
	virtual void DrawWidget(Vector2 _scale) = 0;
};

class Camera;

class UIWidget
	: public Component
	, public IWidgetContext
{
public:
	explicit UIWidget(Object* _owner);
	virtual ~UIWidget();
public:
	virtual void Tick() override;
	virtual void FixedUpdate() override;
	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Draw(Camera* _camera) override;
	virtual void PostRender() override;
	// Editor Only
	virtual void EditorUpdate() override;
	virtual void EditorRender() override;
	virtual void EditorRendering(EditorViewerType _viewerType) override;
public:
	// �⺻ ���� ����
	void SetTexture(ResourceHandle _handle);
	// �׷��� ���� ����
	void SetAlpha(FLOAT _alpha) { mAlpha = Clamp(_alpha, 0.0f, 1.0f); }
	void SetColor(Color _color) { mColor = _color; }
protected:
	std::wstring mID = L"";
	Texture2DResource* m_pTexture = nullptr;

	eUIType	mUIType = eUIType::NON;
	Color	mColor{ 1,1,1 };
	float	mAlpha = 1.f;
};