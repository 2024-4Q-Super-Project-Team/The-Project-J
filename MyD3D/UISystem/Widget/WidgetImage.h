#pragma once
#include "Widget.h"

struct ImageInfo
{
	std::wstring filepath = L"";
	Texture2DResource* texture = nullptr;
};

class WidgetImage
	: public Widget
{
public:
	WidgetImage();
	virtual ~WidgetImage(); // 버츄얼로 돌려야되는데 왜 안됨?

public:
	virtual void Start();
	virtual void Update();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();
public:
	void SetImageFilename(const std::wstring _path) { mImgInfo.filepath = _path; }
	void SetImagePosition(Vector2 _position) { SetPosition(_position); }
private:
	SpriteBatch* m_pSpriteBatch = nullptr;

	ImageInfo mImgInfo;
};

