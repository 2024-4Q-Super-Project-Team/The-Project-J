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
	virtual ~WidgetImage(); // ������ �����ߵǴµ� �� �ȵ�?

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

