#pragma once
#include "Component/Component.h"

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

class Widget
	: public Component
	, public IWidgetContext
{
public:
	explicit Widget(Object* _owner);
	virtual ~Widget();
public:
	// �⺻ ���� ����
	void SetTexture(std::wstring _id, std::wstring _path);
	// �׷��� ���� ����
	void SetAlpha(FLOAT _alpha)	{ mAlpha = Clamp(_alpha, 0.0f, 1.0f); }
	void SetColor(Color _color)	{ mColor = _color; }
protected:
	std::wstring mID = L"";
	Texture2DResource*	m_pTexture = nullptr;

	eUIType	mUIType = eUIType::NON;
	Color	mColor{1,1,1};
	float	mAlpha = 1.f;
};