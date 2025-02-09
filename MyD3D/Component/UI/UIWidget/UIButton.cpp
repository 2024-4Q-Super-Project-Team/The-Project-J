#include "pch.h"
#include "UIButton.h"

UIButton::UIButton(Object* _owner)
	: UIWidget(_owner)
{
	SetEID("UI_Button");
	mType = eComponentType::UI_BUTTON;
}

UIButton::~UIButton()
{

}

void UIButton::Start()
{
}

void UIButton::PreUpdate()
{
	UIWidget::PreUpdate();
}

void UIButton::Update()
{
	
}

void UIButton::DrawWidget(Vector2 _scale)
{
	RECT rect{};
	rect.left = gameObject->transform->position.x * _scale.x;
	rect.top = gameObject->transform->position.y * _scale.y;
	rect.right = rect.left + (gameObject->transform->scale.x * _scale.x);
	rect.bottom = rect.top + (gameObject->transform->scale.y * _scale.y);

	if (mButtonState == eButtonState::DEFAULT && m_pDefaultTexture)
	{
		UIManager::GetSpriteBatch()->
			Draw(m_pDefaultTexture->Texture->mSRV, rect, nullptr, mColor);
	}

	if (mButtonState == eButtonState::SELECTED && m_pSelectedTexture)
	{
		UIManager::GetSpriteBatch()->
			Draw(m_pSelectedTexture->Texture->mSRV, rect, nullptr, mColor);
	}
}

void UIButton::EditorUpdate()
{
	UIWidget::PreUpdate();
}

void UIButton::EditorGlobalUpdate()
{
	gameObject->GetOwnerWorld()->
		mNeedResourceHandleTable.insert(mDefaultTextureHandle.GetKey());

	gameObject->GetOwnerWorld()->
		mNeedResourceHandleTable.insert(mSelectedTextureHandle.GetKey());
}

void UIButton::EditorRendering(EditorViewerType _viewerType)
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

		const char* stateNames[] = { "Default", "Selected" };
		static int currentState = (int)mButtonState;

		if (ImGui::Combo("State", &currentState, stateNames, IM_ARRAYSIZE(stateNames)))
		{
			SetState((eButtonState)currentState);
		}
		ImGui::ColorEdit4("color", &mColor.x);

		ImGui::NewLine();

		ImGui::Text("Default Texture");

		std::string widgetID_d = "NULL Default Texture";

		if (m_pDefaultTexture)
		{
			m_pDefaultTexture->EditorRendering(EditorViewerType::INSPECTOR);
			widgetID_d = m_pDefaultTexture->GetEID();

			ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
			ImGui::Selectable(widgetID_d.c_str(), false, ImGuiSelectableFlags_Highlight);
		}

		if (EditorDragNDrop::ReceiveDragAndDropResourceData<Texture2DResource>(widgetID_d.c_str(), &mDefaultTextureHandle))
		{
			SetTexture(mDefaultTextureHandle, m_pDefaultTexture);
		}

		ImGui::Text("Selected Texture");

		std::string widgetID_s = "NULL Selected Texture";

		if (m_pSelectedTexture)
		{
			m_pSelectedTexture->EditorRendering(EditorViewerType::INSPECTOR);
			widgetID_s = m_pSelectedTexture->GetEID();

			ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
			ImGui::Selectable(widgetID_s.c_str(), false, ImGuiSelectableFlags_Highlight);
		}

		if (EditorDragNDrop::ReceiveDragAndDropResourceData<Texture2DResource>(widgetID_s.c_str(), &mSelectedTextureHandle))
		{
			SetTexture(mSelectedTextureHandle, m_pSelectedTexture);
		}

		ImGui::NewLine();
		ImGui::Checkbox("use fade", &bUseFade);

		EDITOR_COLOR_POP(2);

		break;
	}
	default:
		break;
	}
}

void _CALLBACK UIButton::OnEnable()
{
	SetTexture(mDefaultTextureHandle, m_pDefaultTexture);
	SetTexture(mSelectedTextureHandle, m_pSelectedTexture);
	return void _CALLBACK();
}

void _CALLBACK UIButton::OnDisable()
{
	return void _CALLBACK();
}

void _CALLBACK UIButton::OnDestroy()
{
	return void _CALLBACK();
}

json UIButton::Serialize()
{
	json ret;
	ret["id"] = GetId();
	ret["name"] = "UIButton";
	ret["default texture handle"] = mDefaultTextureHandle.Serialize();
	ret["selected texture handle"] = mSelectedTextureHandle.Serialize();

	ret["ui type"] = mUIType;
	ret["color"] = { mColor.x, mColor.y, mColor.z, mColor.w };
	ret["use fade"] = bUseFade;
	ret["button state"] = mButtonState;

	return ret;
}

void UIButton::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if (j.contains("default texture handle"))
	{
		mDefaultTextureHandle.Deserialize(j["default texture handle"]);
		if (mDefaultTextureHandle.GetPath() != L"")
		{
			SetTexture(mDefaultTextureHandle, m_pDefaultTexture);
		}
	}

	if (j.contains("selected texture handle"))
	{
		mSelectedTextureHandle.Deserialize(j["selected texture handle"]);
		if (mSelectedTextureHandle.GetPath() != L"")
		{
			SetTexture(mSelectedTextureHandle, m_pSelectedTexture);
		}
	}

	if (j.contains("ui type"))
	{
		mUIType = (eUIType)j["ui type"].get<int>();
	}

	if (j.contains("color")) {
		auto color = j["color"];
		if (color.is_array() && color.size() == 4)
		{
			Color col = { color[0].get<float>(),
							color[1].get<float>(),
							color[2].get<float>(),
							color[3].get<float>() };

			SetColor(col);
		}
	}

	if (j.contains("use fade"))
	{
		bUseFade = j["use fade"].get<bool>();
	}

	if (j.contains("button state"))
	{
		mButtonState = (eButtonState)j["button state"].get<int>();
	}
}
