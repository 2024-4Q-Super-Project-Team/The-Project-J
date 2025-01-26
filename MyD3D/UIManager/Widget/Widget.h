#pragma once
#include "Component/Component.h"

class Transform;

// 위젯 타입
enum class eUIType
{
	NON,	// 없음
	IMAGE,	// 이미지
	TEXT,	// 텍스트
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
	// 기본 정보 설정
	void SetTexture(std::wstring _id, std::wstring _path);
	// 그래픽 정보 설정
	void SetAlpha(FLOAT _alpha)	{ mAlpha = Clamp(_alpha, 0.0f, 1.0f); }
	void SetColor(Color _color)	{ mColor = _color; }
protected:
	std::wstring mID = L"";
	Texture2DResource*	m_pTexture = nullptr;

	eUIType	mUIType = eUIType::NON;
	Color	mColor{1,1,1};
	float	mAlpha = 1.f;
};