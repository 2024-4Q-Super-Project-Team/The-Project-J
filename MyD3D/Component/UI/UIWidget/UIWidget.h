#pragma once
#include "Component/Component.h"

class Transform;
class Texture2DResource;

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
	virtual void Start() override;
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
	void SetTexture(ResourceHandle _handle, Texture2DResource* _texture);
	bool ProcessFadeIn(Color* _color);
	bool ProcessFadeOut(Color* _color);

	// �׷��� ���� ����
	void	SetColor(Color _color)	{ mColor = _color; }
	Color	GetColor()				{ return mColor; }
protected:
	std::wstring mID = L"";

	eUIType	mUIType = eUIType::NON;
	Color	mColor{ 1,1,1,0 };
	float a = 127.f;
	float mFadeTime = 5.f;
	bool bUseFade = false;
};