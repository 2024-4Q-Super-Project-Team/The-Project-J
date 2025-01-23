#include "pch.h"
#include "SphereCollider.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"

SphereCollider::SphereCollider(Object* _owner) : Collider(_owner)
{
	SetEID("SphereCollider");
	mGeometry = PxSphereGeometry(mInitialRadius);
	mShape = GameManager::GetPhysicsManager()->GetPhysics()
		->createShape(mGeometry, *GameManager::GetPhysicsManager()->GetDefaultMaterial());
	mShape->userData = this;
	mShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	mBS.Center = gameObject->transform->position;
	mBS.Radius = mInitialRadius;


	mRadius = mInitialRadius;
}

void SphereCollider::Start()
{
	Collider::Start();
}

void SphereCollider::Tick()
{
	Collider::Tick();
}

void SphereCollider::FixedUpdate()
{
	Collider::FixedUpdate();
}

void SphereCollider::PreUpdate()
{
	Collider::PreUpdate();
}

void SphereCollider::Update()
{
	Collider::Update();
}

void SphereCollider::PostUpdate()
{
	Collider::PostUpdate();
}

void SphereCollider::PreRender()
{
	Collider::PreRender();
}

void SphereCollider::Render()
{
	Collider::Render();
}

void SphereCollider::Draw(Camera* _camera)
{
	if (Collider::bDrawMode)
		_camera->PushWireList(this);
}

void SphereCollider::PostRender()
{
	Collider::PostRender();
}

void SphereCollider::EditorUpdate()
{
}

void SphereCollider::EditorRender()
{
}

json SphereCollider::Serialize()
{
	json ret;
	ret["id"] = GetId();
	ret["name"] = "Transform";

	ret["isTrigger"] = mIsTrigger;
	ret["position"] = { mPosition.x, mPosition.y, mPosition.z };
	ret["rotation"] = { mRotation.x, mRotation.y, mRotation.z };
	ret["radius"] = mRadius;

	return ret;
}

void SphereCollider::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	mIsTrigger = j["isTrigger"].get<bool>();
	mPosition.x = j["position"][0].get<float>();
	mPosition.y = j["position"][1].get<float>();
	mPosition.z = j["position"][2].get<float>();
	mRotation.x = j["rotation"][0].get<float>();
	mRotation.y = j["rotation"][1].get<float>();
	mRotation.z = j["rotation"][2].get<float>();
	mRadius = j["radius"].get<float>();


	SetRadius();
	SetRotation();
	SetLocalPosition();

	mBS.Center = mPosition;
	mBS.Radius = mRadius;
}

void SphereCollider::DrawObject(Matrix& _view, Matrix& _projection)
{
}

void SphereCollider::DrawWire()
{
	// 원본 구체(로컬 좌표계 기준)
	DirectX::BoundingSphere localSphere = mBS;
	XMMATRIX worldMatrix = gameObject->transform->GetWorldMatrix();
	DirectX::BoundingSphere transformedSphere;
	localSphere.Transform(transformedSphere, worldMatrix);
	Debug::Draw(DebugRenderer::GetBatch(), transformedSphere, mBaseColor);
}

void SphereCollider::SetRadius()
{
	mGeometry.radius = mRadius;
}

void SphereCollider::EditorRendering(EditorViewerType _type)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	ImGui::Text("Position : ");
	if (ImGui::DragFloat3((uid + "Position").c_str(), &mPosition.x, 0.1f, -1000.f, 1000.f))
	{
		SetLocalPosition();
		mBS.Center = mPosition;
	}

	ImGui::Text("Rotation : ");
	ImGui::DragFloat3((uid + "Rotation").c_str(), &mRotation.x, 0.1f, -360.f, 360.f);


	ImGui::Text("Radius : ");
	if (ImGui::DragFloat((uid + "Radius").c_str(), &mRadius, 0.1f, 0.f, 100.f))
	{
		mBS.Radius = mRadius;
	}

	ImGui::Separator();

	ImGui::Text("isTrigger : "); ImGui::SameLine();
	if (ImGui::Checkbox(("##isTrigger" + uid).c_str(), (bool*)&mIsTrigger))
	{
		SetIsTrigger();
	}
}
