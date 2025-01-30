#pragma once
#include "UIWidget.h"

class UIText
	: public UIWidget
{
public:
	explicit UIText(Object* _owner);
	virtual ~UIText();
public:
	void PreUpdate() override;
	void EditorUpdate() override;
	void EditorGlobalUpdate() override;
	void EditorRendering(EditorViewerType _viewerType) override;
public:
	virtual void DrawWidget(Vector2 _scale)	override;
public:
	void SetFont(std::wstring _path);
	void SetTextLine(float _line);
	void SetTextDefault(char _defaultText);
	void SetTextFormat(const wchar_t* _msg, ...);
	void SetTextOutlineOffset(float _offset);
	void SetTextOutlineColor(Color _color);
public:
	std::wstring GetTextFormat()	{ return mFormat; }
public:
	void _CALLBACK OnEnable()	override;
	void _CALLBACK OnDisable()	override;
	void _CALLBACK OnDestroy()	override;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
private:
	SpriteFont* m_pSpriteFont = nullptr;
	wchar_t mFormat[1024];	// 텍스트 포맷

	// 외곽선 렌더
	void OutlinedTextRender(Vector2 _scale, Vector3 _objPos);
	// 외곽선 관련 정보들
	bool bUseOutline = false;
	float mOutlineOffset = 1.f;
	Color mOutlineColor{};
};

