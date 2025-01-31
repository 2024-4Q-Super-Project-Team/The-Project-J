#pragma once
#include "Component/UI/UIWidget/UIWidget.h"

class FontResource;

class UIText
	: public UIWidget
{
public:
	explicit UIText(Object* _owner);
	virtual ~UIText();
public:
	void PreUpdate() override;
	void Update() override;
	void EditorUpdate() override;
	void EditorGlobalUpdate() override;
	void EditorRendering(EditorViewerType _viewerType) override;
public:
	virtual void DrawWidget(Vector2 _scale)	override;
public:
	void SetFont(ResourceHandle _handle);
	void SetTextLine(float _line);
	void SetTextFormat(const wchar_t* _msg, ...);
	void SetTextOutlineOffset(float _offset)	{ mOutlineOffset = _offset; }
	void SetTextOutlineColor(Color _color)		{ mOutlineColor = _color; }
public:
	std::wstring	GetTextFormat()			{ return mFormat; }
	float			GetTextLine()			{ return mLine; }
	float			GetTextOulineOffset()	{ return mOutlineOffset; }
	Color			GetTextOutlineColor()	{ return mOutlineColor; }
public:
	void _CALLBACK OnEnable()	override;
	void _CALLBACK OnDisable()	override;
	void _CALLBACK OnDestroy()	override;
public:
	virtual json Serialize() override;
	virtual void Deserialize(json& j) override;
private:
	FontResource* m_pFont = nullptr;
	ResourceHandle mFontHandle;
	SpriteFont* m_pSpriteFont = nullptr;
	wchar_t mFormat[1024];	// �ؽ�Ʈ ����
	// �ؽ�Ʈ �׷��� ����
	float mLine = 14.f;

	// �ܰ��� ����
	void OutlinedTextRender(Vector2 _scale, Vector3 _objPos);
	// �ܰ��� ���� ������
	bool bUseOutline = false;
	float mOutlineOffset = 1.f;
	Color mOutlineColor{};
};

