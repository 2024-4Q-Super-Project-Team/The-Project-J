#include "pch.h"
#include "SpriteRenderer.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/ResourceManager.h"

SpriteRenderer::SpriteRenderer(Object* _owner)
	: Component(_owner)
{
    SetEID("SpriteRenderer");
    mType = eComponentType::MESH_RENDERER;
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Start()
{
    SetSprite(mSpriteHandle);
}

void SpriteRenderer::Tick()
{
}

void SpriteRenderer::FixedUpdate()
{
}

void SpriteRenderer::PreUpdate()
{
}

void SpriteRenderer::Update()
{
}

void SpriteRenderer::PostUpdate()
{
}

void SpriteRenderer::PreRender()
{
    mTileIndexWidth  = Clamp(mTileIndexWidth, 0, INT_MAX);
    mTileIndexHeight = Clamp(mTileIndexHeight, 0, INT_MAX);
}

void SpriteRenderer::Render()
{
}

void SpriteRenderer::Draw(Camera* _camera)
{
    if (mSpriteTexture)
    {
        _camera->PushDrawList(this);
    }
}

void SpriteRenderer::PostRender()
{
}

void SpriteRenderer::EditorUpdate()
{
}

void SpriteRenderer::EditorGlobalUpdate()
{
    gameObject->GetOwnerWorld()->
        mNeedResourceHandleTable.insert(mSpriteHandle.GetKey());
}

void SpriteRenderer::EditorRender()
{
}

void _CALLBACK SpriteRenderer::OnEnable()
{
    Start();
    return void _CALLBACK();
}

void _CALLBACK SpriteRenderer::OnDisable()
{
    mSpriteTexture = nullptr;
    return void _CALLBACK();
}

void _CALLBACK SpriteRenderer::OnDestroy()
{
    return void _CALLBACK();
}

void SpriteRenderer::DrawObject(Matrix& _view, Matrix& _projection)
{
    if (!mSpriteTexture) return;

    Vector2 SpriteSize = mSpriteTexture->GetSize();
    float tileWidth = SpriteSize.x / (FLOAT)mTileIndexWidth;
    float tileHeight = SpriteSize.y / (FLOAT)mTileIndexHeight;

    // 타일 인덱스 계산
    UINT tileIndexX = mTileIndex % mTileIndexWidth;
    UINT tileIndexY = mTileIndex / mTileIndexWidth;

    float uMin = (tileIndexX * tileWidth) / SpriteSize.x;
    float uMax = ((tileIndexX + 1) * tileWidth) / SpriteSize.x;
    float vMin = (tileIndexY * tileHeight) / SpriteSize.y;
    float vMax = ((tileIndexY + 1) * tileHeight) / SpriteSize.y;

    // 텍스처 바인딩
    mSpriteTexture->Texture->SetBindSlot(11);
    mSpriteTexture->Texture->SetBindStage(eShaderStage::PS);
    mSpriteTexture->Texture->Bind();

    // 트랜스폼 상수 버퍼
    TransformCBuffer cb;
    cb.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
    cb.View = XMMatrixTranspose(_view);
    cb.Projection = XMMatrixTranspose(_projection);

    // Sprite UV 상수 버퍼
    SpriteCBuffer sb;
    sb.uvMin = Vector2(uMin, vMin);
    sb.uvMax = Vector2(uMax, vMax);

    ParticleSizeCBuffer pcb;
    pcb.particleSize = mTileSize;

    // GPU에 상수 버퍼 업데이트
    GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&cb);
    GraphicsManager::GetConstantBuffer(eCBufferType::Sprite)->UpdateGPUResoure(&sb);
    GraphicsManager::GetConstantBuffer(eCBufferType::ParticleSize)->UpdateGPUResoure(&pcb);

    D3DGraphicsDefault::GetPointVertexBuffer()->Bind();

    D3DGraphicsRenderer::DrawCall(1, 0);
}


void SpriteRenderer::DrawShadow(Light* _pLight)
{
}

void SpriteRenderer::DrawWire()
{
}

Vector3 SpriteRenderer::GetDistanceFromCamera(Camera* _camera)
{
	return Vector3();
}

eBlendModeType SpriteRenderer::GetBlendMode()
{
	return eBlendModeType::SPRITE_BLEND;
}

eRasterizerStateType SpriteRenderer::GetCullingMode()
{
	return eRasterizerStateType::NONE_CULLING;
}

void SpriteRenderer::SetSprite(ResourceHandle _handle)
{
	if (_handle.GetResourceType() == eResourceType::Texture2DResource)
	{
		mSpriteHandle = _handle;
		mSpriteTexture = ResourceManager::GetResource<Texture2DResource>(_handle);
	}
}

void SpriteRenderer::SetTileIndexWidth(UINT _width)
{
	mTileIndexWidth = _width;
    mFrameCount = mTileIndexWidth * mTileIndexHeight;
}

void SpriteRenderer::SetTileIndexHeight(UINT _height)
{
    mTileIndexHeight = _height;
    mFrameCount = mTileIndexWidth * mTileIndexHeight;
}

void SpriteRenderer::SetIndex(UINT _index)
{
	mTileIndex = _index;
}

void SpriteRenderer::SetIndex(UINT _width, UINT height)
{
	mTileIndex = _width * height;
}

json SpriteRenderer::Serialize()
{
    json ret;
    ret["id"] = GetId();
    ret["name"] = "SpriteRenderer";

    ret["sprite handle"] = mSpriteHandle.Serialize();

    ret["tile index width"] = mTileIndexWidth;
    ret["tile index height"] = mTileIndexHeight;
    ret["tile index"] = mTileIndex;
    ret["sprite size"] = mTileSize;

	return ret;
}

void SpriteRenderer::Deserialize(json& j)
{
    SetId(j["id"].get<unsigned int>());

    if (j.contains("sprite handle"))
    {
        mSpriteHandle.Deserialize(j["sprite handle"]);
        SetSprite(mSpriteHandle);
    }
    if (j.contains("tile index width"))
    {
        mTileIndexWidth = j["tile index width"].get<UINT>();
        mFrameCount = mTileIndexWidth * mTileIndexHeight;
    }
    if (j.contains("tile index height"))
    {
        mTileIndexHeight = j["tile index height"].get<UINT>();
        mFrameCount = mTileIndexWidth * mTileIndexHeight;
    }
    if (j.contains("tile index"))
    {
        mTileIndex = j["tile index"].get<UINT>();
    }
    if (j.contains("sprite size"))
    {
        mTileSize = j["sprite size"].get<INT>();
    }
}

void SpriteRenderer::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    {
        std::string widgetID = "NULL Sprite";
        if (mSpriteTexture)
        {
            mSpriteTexture->EditorRendering(EditorViewerType::DEFAULT);
            widgetID = mSpriteTexture->GetEID();
            mSpriteTexture->EditorRendering(EditorViewerType::INSPECTOR);
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
            ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
            EDITOR_COLOR_POP(1);
        }
        if (EditorDragNDrop::ReceiveDragAndDropResourceData<Texture2DResource>(widgetID.c_str(), &mSpriteHandle))
        {
            SetSprite(mSpriteHandle);
        }
    }
    ImGui::Separator();
    {
        ImGui::InputInt(("Num Tile Width : " + uid).c_str(), &mTileIndexWidth);
        ImGui::InputInt(("Num Tile Height : " + uid).c_str(), &mTileIndexHeight);
        ImGui::InputInt(("Index : " + uid).c_str(), &mTileIndex);
        ImGui::InputInt(("Size : " + uid).c_str(), &mTileSize);
        mFrameCount = mTileIndexWidth * mTileIndexHeight;
    }
}
