#include "pch.h"
#include "UISprite.h"

UISprite::UISprite(Object* _owner)
	: UIWidget(_owner)
{
	SetEID("UI_Sprite");
	mType = eComponentType::UI_SPRITE;
	mUIType = eUIType::IMAGE;
}

UISprite::~UISprite()
{

}

void UISprite::DrawWidget(Vector2 _scale)
{
	RECT rect{};
	rect.left = gameObject->transform->position.x * _scale.x;
	rect.top = gameObject->transform->position.y * _scale.y;
	rect.right = rect.left + (gameObject->transform->scale.x * _scale.x);
	rect.bottom = rect.top + (gameObject->transform->scale.y * _scale.y);

	if (m_pTexture)
	{
		UIManager::GetSpriteBatch()->
			Draw(m_pTexture->Texture->mSRV, rect, nullptr, mColor);
	}
}

void _CALLBACK UISprite::OnEnable()
{
	SetTexture(mTextureHandle);
	return void _CALLBACK();
}

void _CALLBACK UISprite::OnDisable()
{
	return void _CALLBACK();
}

void _CALLBACK UISprite::OnDestroy()
{
	return void _CALLBACK();
}

void UISprite::PreUpdate()
{
	UIWidget::PreUpdate();
}

void UISprite::Update()
{

}

void UISprite::EditorUpdate()
{
	UIWidget::PreUpdate();
}

void UISprite::EditorGlobalUpdate()
{
	gameObject->GetOwnerWorld()->
		mNeedResourceHandleTable.insert(mTextureHandle.GetKey());
}

void UISprite::EditorRendering(EditorViewerType _viewerType)
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
       std::string widgetID = "NULL Texture";
       std::string name = "NULL Texture";

       if (m_pTexture)
       {
		   m_pTexture->EditorRendering(EditorViewerType::INSPECTOR);
		   name = Helper::ToString(m_pTexture->GetKey());
		   widgetID = m_pTexture->GetEID();

		   ImGui::ColorEdit4("color", &mColor.x);
		   ImGui::PushStyleColor(ImGuiCol_Header, EDITOR_COLOR_EXTRA);
       }
       else
       {
           ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
           ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
       }

	   if (EditorDragNDrop::ReceiveDragAndDropResourceData<Texture2DResource>(uid.c_str(), &mTextureHandle))
	   {
		   SetTexture(mTextureHandle);
	   }

	   EDITOR_COLOR_POP(1);

	   break;
   }
   default:
	   break;
   }
}

json UISprite::Serialize()
{
	json ret;
	ret["id"] = GetId();
	ret["name"] = "UISprite";
	ret["texture handle"] = mTextureHandle.Serialize();

	ret["ui type"] = mUIType;
	ret["color"] = { mColor.x, mColor.y, mColor.z };
	ret["alpha"] = mAlpha;

	return ret;
}

void UISprite::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if (j.contains("texture handle"))
	{
		mTextureHandle.Deserialize(j["texture handle"]);
		SetTexture(mTextureHandle);
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
							1.f						};

			SetColor(col);
		}
	}

	if (j.contains("alpha"))
	{
		SetAlpha(j["alpha"].get<float>());
	}
}
