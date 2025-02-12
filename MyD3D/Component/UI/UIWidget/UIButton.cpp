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
	if (mButtonState == eButtonState::DEFAULT)
	{
		gameObject->transform->position.x = defaultPos.x;
		gameObject->transform->position.y = defaultPos.y;
		gameObject->transform->scale.x = m_pDefaultTexture->Texture->mWidth;
		gameObject->transform->scale.y = m_pDefaultTexture->Texture->mHeight;
	}
	else if (mButtonState == eButtonState::SELECTED)
	{
		gameObject->transform->position.x = selectedPos.x;
		gameObject->transform->position.y = selectedPos.y;
		gameObject->transform->scale.x = m_pSelectedTexture->Texture->mWidth;
		gameObject->transform->scale.y = m_pSelectedTexture->Texture->mHeight;
	}

	RECT rect{};
	rect.left = gameObject->transform->position.x * _scale.x;
	rect.top = gameObject->transform->position.y * _scale.y;
	rect.right = rect.left + (gameObject->transform->scale.x * _scale.x);
	rect.bottom = rect.top + (gameObject->transform->scale.y * _scale.y);

	if (mButtonState == eButtonState::DEFAULT && m_pDefaultTexture)
	{
		UIManager::GetSpriteBatch()->
			Draw(m_pDefaultTexture->Texture->mSRV, rect, nullptr, mColor * alpha);
	}

	if (mButtonState == eButtonState::SELECTED && m_pSelectedTexture)
	{
		UIManager::GetSpriteBatch()->
			Draw(m_pSelectedTexture->Texture->mSRV, rect, nullptr, mColor * alpha);
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
		ImGui::ColorEdit3("color", &mColor.x);
		ImGui::DragFloat("alpha", &alpha);
		ImGui::DragFloat2("default position", &defaultPos.x, 0.5f,  -5000.f, 5000.f);
		ImGui::DragFloat2("selected position", &selectedPos.x, 0.5f, -5000.f, 5000.f);

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
	ret["color"] = { mColor.x, mColor.y, mColor.z };
	ret["alpha"] = alpha;
	ret["button state"] = mButtonState;

	ret["default position"] = { defaultPos.x, defaultPos.y };
	ret["selected position"] = { selectedPos.x, selectedPos.y };

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
		if (color.is_array() && color.size() == 3)
		{
			Color col = { color[0].get<float>(),
							color[1].get<float>(),
							color[2].get<float>(),
							1.f};

			SetColor(col);
		}
	}

	if (j.contains("alpha"))
	{
		SetAlpha(j["alpha"].get<float>());
	}

	if (j.contains("button state"))
	{
		mButtonState = (eButtonState)j["button state"].get<int>();
	}

	if (j.contains("default position") && j["default position"].size() == 2)
	{
		auto dp = j["default position"];
		defaultPos.x = dp[0].get<float>();
		defaultPos.y = dp[1].get<float>();
	}

	if (j.contains("selected position") && j["selected position"].size() == 2)
	{
		auto sp = j["selected position"];
		selectedPos.x = sp[0].get<float>();
		selectedPos.y = sp[1].get<float>();
	}
}
