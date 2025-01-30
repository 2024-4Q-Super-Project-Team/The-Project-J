#include "pch.h"
#include "UIText.h"

UIText::UIText(Object* _owner)
	: UIWidget(_owner)
{
	SetEID("UI_Text");
	mType = eComponentType::UI_TEXT;
	memset(mFormat, 0, sizeof(mFormat));
	mUIType = eUIType::TEXT;
}

UIText::~UIText()
{
	if (m_pSpriteFont != nullptr)
	{
		delete m_pSpriteFont;
		m_pSpriteFont = nullptr;
	}
}

void UIText::DrawWidget(Vector2 _scale)
{
	Vector3 objPos = gameObject->transform->GetWorldPosition();

	if (bUseOutline)
	{
		OutlinedTextRender(_scale, objPos);
	}

	m_pSpriteFont->DrawString(UIManager::GetSpriteBatch(), mFormat, (objPos.x, objPos.y) * _scale, mColor, 0.f, Vector2(0.0f, 0.0f), _scale.x);
}

void UIText::OutlinedTextRender(Vector2 _scale, Vector3 _objPos) {
	
	// 외곽선 오프셋 (4방향)
	Vector2 offsets[] = {
		{-mOutlineOffset, 0.0f}, {mOutlineOffset, 0.0f}, {0.0f, -mOutlineOffset}, {0.0f, mOutlineOffset}
	};

	// 외곽선 그리기
	for (auto& offset : offsets) {
		Vector2 outlinePos = Vector2(_objPos.x + offset.x, _objPos.y + offset.y) * _scale;
		m_pSpriteFont->DrawString(UIManager::GetSpriteBatch(), mFormat, outlinePos, mOutlineColor, 0.f, Vector2(0.0f, 0.0f), _scale.x);
	}
}

void UIText::SetFont(std::wstring _path)
{
	if (!_path.empty())
	{
		m_pSpriteFont = new SpriteFont(D3DGraphicsDevice::GetDevice(), _path.c_str());
	}
}

void UIText::SetTextLine(float _line)
{
	m_pSpriteFont->SetLineSpacing(_line);
}

void UIText::SetTextDefault(char _defaultText)
{
	m_pSpriteFont->SetDefaultCharacter(_defaultText);
}

void UIText::SetTextFormat(const wchar_t* _msg, ...)
{
	// 가변 인자 처리
	va_list args;
	va_start(args, _msg);
	vswprintf_s(mFormat, sizeof(mFormat) / sizeof(wchar_t), _msg, args);
	va_end(args);
}

void UIText::SetTextOutlineOffset(float _offset)
{
	mOutlineOffset = _offset;
}

void UIText::SetTextOutlineColor(Color _color)
{
	mOutlineColor = _color;
}

void _CALLBACK UIText::OnEnable()
{
	return void _CALLBACK();
}

void _CALLBACK UIText::OnDisable()
{
	return void _CALLBACK();
}

void _CALLBACK UIText::OnDestroy()
{
	return void _CALLBACK();
}

void UIText::PreUpdate()
{
	UIWidget::PreUpdate();
}

void UIText::EditorUpdate()
{

}

void UIText::EditorGlobalUpdate()
{

}

void UIText::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	switch (_viewerType)
	{
	case EditorViewerType::DEFAULT:
	{
		break;
	}
	case EditorViewerType::HIERARCHY:
	{
		break;
	}
	case EditorViewerType::INSPECTOR:
	{
		ImGui::Separator();

		ImGui::DragFloat2((uid + "Text Position").c_str(), &gameObject->transform->position.x, 0.5f, 0.0f, 0.0f);

		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
		EDITOR_COLOR_POP(1);

		break;
	}
	default:
		break;
	}
}

json UIText::Serialize()
{
	return json();
}

void UIText::Deserialize(json& j)
{
}
