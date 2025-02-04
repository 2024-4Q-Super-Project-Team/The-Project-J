#include "pch.h"
#include "Component/UI/UIWidget/UIText.h"

#include "Resource/FontResource/FontResource.h"

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
	Vector3 objPos = gameObject->transform->position;
	
	if (!m_pSpriteFont)
		return;

	if (bUseOutline)
	{
		OutlinedTextRender(_scale, objPos);
	}

	Color color = Color(mColor.x, mColor.y, mColor.z, mAlpha);

	m_pSpriteFont->DrawString(UIManager::GetSpriteBatch(), mFormat, Vector2(objPos.x * _scale.x, objPos.y * _scale.y), color, 0.f, Vector2(0.0f, 0.0f), _scale.x);
}

void UIText::OutlinedTextRender(Vector2 _scale, Vector3 _objPos) 
{
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


void UIText::SetFont(ResourceHandle _handle)
{
	m_pFont = ResourceManager::GetResource<FontResource>(_handle);

	if (!m_pFont)
		return;

	std::wstring path = m_pFont->GetPath();

	if (!path.empty())
	{
		if (m_pSpriteFont != nullptr)
		{
			delete m_pSpriteFont;
			m_pSpriteFont = nullptr;
		}

		m_pSpriteFont = new SpriteFont(D3DGraphicsDevice::GetDevice(), path.c_str());
	}
}

void UIText::SetTextLine(float _line)
{
	m_pSpriteFont->SetLineSpacing(_line);
	m_pSpriteFont->SetDefaultCharacter(' ');
}

void UIText::SetTextFormat(const wchar_t* _msg, ...)
{
	// 가변 인자 처리
	va_list args;
	va_start(args, _msg);
	vswprintf_s(mFormat, sizeof(mFormat) / sizeof(wchar_t), _msg, args);
	va_end(args);
}

void UIText::PreUpdate()
{
	UIWidget::PreUpdate();
}

void UIText::Update()
{
	
}

void UIText::EditorUpdate()
{
	
}

void UIText::EditorGlobalUpdate()
{
	gameObject->GetOwnerWorld()->
		mNeedResourceHandleTable.insert(mFontHandle.GetKey());
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
		std::string widgetID = "NULL Font";
		std::string name = "NULL Font";

		ImGui::Separator();
		if (m_pFont)
		{
			m_pFont->EditorRendering(EditorViewerType::INSPECTOR);
			name = Helper::ToString(m_pFont->GetKey());
			widgetID = m_pFont->GetEID();

			ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
			ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
		}

		if (EditorDragNDrop::ReceiveDragAndDropResourceData<FontResource>(uid.c_str(), &mFontHandle))
		{
			SetFont(mFontHandle);
		}

		ImGui::Separator();
		ImGui::Text("Text Format : ");
		static char Format[1024] = "Text";
		ImGui::InputText("##AddUIText", Format, IM_ARRAYSIZE(Format));

		ImGui::SameLine();
		if (ImGui::Button("OK") || Input::IsKeyDown(Key::ENTER))
		{
			wchar_t wFormat[1024];
			MultiByteToWideChar(CP_UTF8, 0, Format, -1, wFormat, 1024);
			SetTextFormat(wFormat);
		}

		ImGui::Text("Current Text Format: %s", Format);
		ImGui::NewLine();

		ImGui::Text("Text INFO : ");
		ImGui::DragFloat("line", &mLine, 0.5f, 1.0f, 100.f);
		ImGui::ColorEdit3("color", &mColor.x);
		ImGui::DragFloat("alpha", &mAlpha, 0.5f, 0.0f, 1.f);

		ImGui::NewLine();
		ImGui::Checkbox("Use Outline", &bUseOutline);
		
		if (bUseOutline)
		{
			ImGui::Separator();

			ImGui::Text("Text Outline INFO : ");
			ImGui::DragFloat("outline Offset", &mOutlineOffset, 0.5f, 0.f, 2.0f);
			ImGui::DragFloat3("outline Color", &mOutlineColor.x, 0.5f, 0.0f, 1.0f);
		}

		EDITOR_COLOR_POP(1);

		break;
	}
	default:
		break;
	}
}

void _CALLBACK UIText::OnEnable()
{
	SetFont(mFontHandle);
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

json UIText::Serialize()
{
	json ret;
	ret["id"] = GetId();
	ret["name"] = "UIText";
	ret["font handle"] = mFontHandle.Serialize();

	ret["ui type"] = mUIType;
	ret["color"] = { mColor.x, mColor.y, mColor.z };
	ret["alpha"] = mAlpha;
	ret["line"] = mLine;
	ret["format"] = mFormat;

	ret["use outline"] = bUseOutline;
	ret["outline offset"] = mOutlineOffset;
	ret["outline color"] = { mOutlineColor.x, mOutlineColor.y, mOutlineColor.z , mOutlineColor.w};

	return ret;
}

void UIText::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if (j.contains("font handle"))
	{
		mFontHandle.Deserialize(j["font handle"]);
		SetFont(mFontHandle);
	}

	if (j.contains("ui type"))
	{
		mUIType = (eUIType)j["ui type"].get<int>();
	}

	if (j.contains("color")) {
		auto color = j["color"];
		if (color.is_array() && color.size() == 3)
		{
			Color col = {	color[0].get<float>(),
							color[1].get<float>(),
							color[2].get<float>(),
							color[3].get<float>(), };

			SetColor(col);
		}
	}

	if (j.contains("alpha"))
	{
		SetAlpha(j["alpha"].get<float>());
	}

	if (j.contains("line"))
	{
		SetTextLine(j["line"].get<float>());
	}

	if (j.contains("format"))
	{
		SetTextFormat(j["line"].get<std::wstring>().c_str());
	}

	if (j.contains("use outline"))
	{
		bUseOutline = j["use outline"].get<int>();
	}

	if (j.contains("outline offset"))
	{
		SetTextOutlineOffset(j["outline offset"].get<float>());
	}

	if (j.contains("outline color")) {
		auto outcolor = j["outline color"];
		if (outcolor.is_array() && outcolor.size() == 3)
		{
			Color col = {	outcolor[0].get<float>(),
							outcolor[1].get<float>(),
							outcolor[2].get<float>(),
							outcolor[3].get<float>(), };

			SetTextOutlineColor(col);
		}
	}
}
