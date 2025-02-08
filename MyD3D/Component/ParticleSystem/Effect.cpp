#include "pch.h"
#include "Effect.h"
#include "World/World.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/Texture/Texture.h"

Effect::Effect(Object* _owner) : Component(_owner)
{
	SetEID("Effect");
	mType = eComponentType::EFFECT;
	mTexture = nullptr;
}

Effect::~Effect()
{
	SAFE_DELETE(mVertexBuffer);
}

void Effect::Start()
{
	unsigned int stride = sizeof(float) * 4;
	unsigned int count = 1; 

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //vertex buffer 용도
	desc.CPUAccessFlags = 0;
	desc.ByteWidth = stride * count;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = &mPosition;

	mVertexBuffer = new D3DGraphicsVertexBuffer(&desc, &data);
	mVertexBuffer->Create();

	mVertexBuffer->SetStride(sizeof(float) * 4);
	mVertexBuffer->SetOffset(0);
}

void Effect::Tick()
{
}

void Effect::FixedUpdate()
{
}

void Effect::PreUpdate()
{
}

void Effect::Update()
{
}

void Effect::PostUpdate()
{
}

void Effect::PreRender()
{
}

void Effect::Render()
{
}

void Effect::Draw(Camera* _camera)
{
	_camera->PushDrawList(this);

	if (EditorManager::mEditorCamera.mIsColliderRendering)
	{
		_camera->PushWireList(this);
	}

}

void Effect::PostRender()
{
}

void Effect::EditorUpdate()
{
}

void Effect::EditorRender()
{
}

json Effect::Serialize()
{
	json ret;

	ret["id"] = GetId();
	ret["name"] = "Effect";
	ret["texture handle"] = mTextureHandle.Serialize();
	
	return ret;
}

void Effect::Deserialize(json& j)
{
	if (j.contains("id"))
		SetId(j["id"].get<unsigned int>());

	if (j.contains("texture handle"))
	{
		mTextureHandle.Deserialize(j["texture handle"]);

		if (mTextureHandle.GetResourceType() == eResourceType::Texture2DResource)
		{
			mTexture = ResourceManager::GetResource<Texture2DResource>(mTextureHandle);
		}
	}
}

void Effect::EditorRendering(EditorViewerType _viewerType)
{

	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	ImGui::Text("Texture : ");

	std::string widgetID = "NULL Texure";
	std::string name = "NULL Texure";
	if (mTexture)
	{
		mTexture->EditorRendering(EditorViewerType::DEFAULT);
		name = Helper::ToString(mTexture->GetKey());
		widgetID = mTexture->GetEID();
		mTexture->EditorRendering(EditorViewerType::INSPECTOR);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
		ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
		EDITOR_COLOR_POP(1);
	}

	if (EditorDragNDrop::ReceiveDragAndDropResourceData<Texture2DResource>(widgetID.c_str(), &mTextureHandle))
	{
		SetTexture(mTextureHandle);
	}
	ImGui::Separator();

	ImGui::Text("Size : ");
	ImGui::DragFloat((uid + "Size").c_str(), &mSize, 0.1f, 0.f, 1000.f);

}

Vector3 Effect::GetDistanceFromCamera(Camera* _camera)
{
	return _camera->GetDistance(gameObject->transform);
}

void Effect::DrawObject(Matrix& _view, Matrix& _projection)
{
	if (!mTexture) return;
	D3DGraphicsRenderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//// 리소스 바인딩
	GraphicsManager::GetVertexShader(eVertexShaderType::PARTICLE)->Bind();
	GraphicsManager::GetGeometryShader(eGeometryShaderType::PARTICLE)->Bind();
	GraphicsManager::GetPixelShader(ePixelShaderType::PARTICLE)->Bind();
	mTexture->Texture->SetBindSlot(25);
	mTexture->Texture->SetBindStage(eShaderStage::PS);
	mTexture->Texture->Bind();

	TransformCBuffer cb;
	cb.World = XMMatrixTranspose(gameObject->transform->GetWorldMatrix());
	cb.View = XMMatrixTranspose(_view);
	cb.Projection = XMMatrixTranspose(_projection);

	ParticleSizeCBuffer pcb;
	pcb.particleSize = mSize;

	mVertexBuffer->Bind();
	GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&cb);
	GraphicsManager::GetConstantBuffer(eCBufferType::ParticleSize)->UpdateGPUResoure(&pcb);

	D3DGraphicsRenderer::DrawVertexCall(1, 0);

	D3DGraphicsRenderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsManager::GetGeometryShader(eGeometryShaderType::PARTICLE)->Reset(); 
}

void Effect::DrawShadow(Light* _pLight)
{
}

void Effect::EditorGlobalUpdate()
{
	gameObject->GetOwnerWorld()->
		mNeedResourceHandleTable.insert(mTextureHandle.GetParentkey());

}
void _CALLBACK Effect::OnEnable()
{
	SetTexture(mTextureHandle);

	return void _CALLBACK();
}
void Effect::DrawWire()
{
	mDebugBox.Center = gameObject->transform->GetWorldPosition();
	mDebugBox.Orientation = gameObject->transform->GetWorldRotation();
	mDebugBox.Extents.x = mDebugBox.Extents.y = mDebugBox.Extents.z =  mSize;

	Debug::Draw(DebugRenderer::GetBatch(), mDebugBox, Color(0, 0, 1, 1));
}

void Effect::SetTexture(ResourceHandle _handle)
{
	if (_handle.GetResourceType() == eResourceType::Texture2DResource)
	{
		mTextureHandle = _handle;
		mTexture = new Texture2DResource(_handle);
	}
}
