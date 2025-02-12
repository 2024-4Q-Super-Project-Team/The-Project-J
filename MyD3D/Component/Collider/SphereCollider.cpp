#include "pch.h"
#include "SphereCollider.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"

SphereCollider::SphereCollider(Object* _owner) : Collider(_owner)
{
	SetEID("SphereCollider");
	mGeometry = PxSphereGeometry(mInitialRadius);
	mShape = GameManager::GetPhysicsManager()->GetPhysics()
		->createShape(mGeometry, *GameManager::GetPhysicsManager()->GetDefaultMaterial(),true);
	mShape->userData = this;
	mShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

	mBS.Center = gameObject->transform->position;
	mBS.Radius = mInitialRadius;
	mRadius = mInitialRadius;

}

void SphereCollider::Start()
{
	Collider::Start();

	SetPosition();
	SetRadius();
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

	SetRadius();
	SetRotation();
	SetPosition();
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
#ifdef _DEBUG
	//if (EditorManager::mEditorCamera.mIsColliderRendering)
	//{
	//	_camera->PushWireList(this);
	//}
#endif
}

void SphereCollider::PostRender()
{
	Collider::PostRender();
}

void SphereCollider::EditorUpdate()
{
	if (EditorManager::mEditorCamera.mIsColliderRendering)
	{
		EditorManager::mEditorCamera.PushWireList(this);
	}
	SetRadius();
	SetRotation();
	SetPosition();
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

	if(j.contains("isTrigger"))
		mIsTrigger = j["isTrigger"].get<bool>();

	if (j.contains("position"))
	{
		mPosition.x = j["position"][0].get<float>();
		mPosition.y = j["position"][1].get<float>();
		mPosition.z = j["position"][2].get<float>();
	}
	if (j.contains("rotation"))
	{
		mRotation.x = j["rotation"][0].get<float>();
		mRotation.y = j["rotation"][1].get<float>();
		mRotation.z = j["rotation"][2].get<float>();
	}

	if(j.contains("radius"))
		mRadius = j["radius"].get<float>();

	SetRadius();
	SetRotation();
	SetPosition();

	mBS.Center = mPosition;
	mBS.Radius = mRadius;
}

void SphereCollider::DrawObject(Matrix& _view, Matrix& _projection)
{
}

void SphereCollider::DrawWire()
{
	mBS.Center = gameObject->transform->GetWorldPosition() + mPosition;
	mBS.Radius = gameObject->transform->GetWorldScale().x * mRadius;
	Debug::Draw(DebugRenderer::GetBatch(), mBS, mBaseColor);
}

void SphereCollider::SetPosition()
{
	PxTransform currentTransform = mShape->getLocalPose();
	mShape->setLocalPose(PxTransform(PxVec3(mPosition.x, mPosition.y, mPosition.z), currentTransform.q));
}

void SphereCollider::SetRotation()
{
}

void SphereCollider::SetRadius()
{
	float radius = gameObject->transform->GetWorldScale().x * mRadius;

	mGeometry = PxSphereGeometry(radius);
	mShape->setGeometry(mGeometry);
}

void SphereCollider::EditorRendering(EditorViewerType _type)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	ImGui::Text("Position : ");
	ImGui::DragFloat3((uid + "Position").c_str(), &mPosition.x, 0.1f, -1000.f, 1000.f);

	ImGui::Text("Rotation : ");
	ImGui::DragFloat3((uid + "Rotation").c_str(), &mRotation.x, 0.1f, -360.f, 360.f);

	ImGui::Text("Radius : ");
	ImGui::DragFloat((uid + "Radius").c_str(), &mRadius, 0.1f, 0.f, 100.f);


	ImGui::Separator();

	ImGui::Text("isTrigger : "); ImGui::SameLine();
	if (ImGui::Checkbox(("##isTrigger" + uid).c_str(), (bool*)&mIsTrigger))
	{
		SetIsTrigger();
	}

	std::vector<const char*> ccharMaterial;
	for (auto& mat : mMaterials)
	{
		ccharMaterial.push_back(mat.c_str());
	}

	if (ImGui::Combo((uid + "Dynamic Items").c_str(), &mMaterialIdx, ccharMaterial.data(), static_cast<int>(ccharMaterial.size())))
	{
		SetMaterial(mMaterials[mMaterialIdx]);
	}
}
