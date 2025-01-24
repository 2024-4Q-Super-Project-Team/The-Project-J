#include "pch.h"
#include "BoxCollider.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"

BoxCollider::BoxCollider(Object* _owner) :Collider(_owner)
{
	SetEID("BoxCollider");
	mGeometry = PxBoxGeometry(PxVec3(mInitialSize.x, mInitialSize.y, mInitialSize.z));
	mShape = GameManager::GetPhysicsManager()->GetPhysics()
		->createShape(mGeometry, *GameManager::GetPhysicsManager()->GetDefaultMaterial(), true);
	mShape->userData = this;
	mShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	
	mOBB.Center = gameObject->transform->position;
	mOBB.Extents = mInitialSize;
	mOBB.Orientation = Quaternion::Identity;

	mExtents = mInitialSize;
}

void BoxCollider::Start()
{
	Collider::Start();
}

void BoxCollider::Tick()
{
	Collider::Tick();
}

void BoxCollider::FixedUpdate()
{
	Collider::FixedUpdate();
}

void BoxCollider::PreUpdate()
{
	Collider::PreUpdate();
}

void BoxCollider::Update()
{
	Collider::Update();
}

void BoxCollider::PostUpdate()
{
	Collider::PostUpdate();
}

void BoxCollider::PreRender()
{
	Collider::PreRender();
}

void BoxCollider::Render()
{
	Collider::Render();
}

void BoxCollider::Draw(Camera* _camera)
{
	if (EditorManager::mEditorCamera.mIsColliderRendering)
	{
		_camera->PushWireList(this);
	}
}

void BoxCollider::PostRender()
{
	Collider::PostRender();
}

void BoxCollider::EditorUpdate()
{
	if (EditorManager::mEditorCamera.mIsColliderRendering)
	{
		EditorManager::mEditorCamera.PushWireList(this);
	}
}

void BoxCollider::EditorRender()
{
}

json BoxCollider::Serialize()
{
	json ret;

	ret["id"] = GetId();
	ret["name"] = "BoxCollider";


	ret["isTrigger"] = mIsTrigger;
	ret["position"] = { mPosition.x, mPosition.y, mPosition.z };
	ret["rotation"] = { mRotation.x, mRotation.y, mRotation.z };
	ret["extents"] = { mExtents.x, mExtents.y, mExtents.z };

	return ret;
}

void BoxCollider::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	mIsTrigger = j["isTrigger"].get<bool>();
	mPosition.x = j["position"][0].get<float>();
	mPosition.y = j["position"][1].get<float>();
	mPosition.z = j["position"][2].get<float>();
	mRotation.x = j["rotation"][0].get<float>();
	mRotation.y = j["rotation"][1].get<float>();
	mRotation.z = j["rotation"][2].get<float>();
	mExtents.x = j["extents"][0].get<float>();
	mExtents.y = j["extents"][1].get<float>();
	mExtents.z = j["extents"][2].get<float>();

	SetExtents();
	SetRotation();
	SetLocalPosition();

	mOBB.Center = mPosition;
	mOBB.Orientation = mQuatRotation;
	mOBB.Extents = mExtents;
}

void BoxCollider::DrawWire()
{
	// 원본 OBB(로컬 좌표계 기준)
	DirectX::BoundingOrientedBox localOBB = mOBB;
	// 월드 변환 행렬
	XMMATRIX worldMatrix = gameObject->transform->GetWorldMatrix();
	// 월드 좌표계로 변환된 OBB
	DirectX::BoundingOrientedBox transformedOBB;
	localOBB.Transform(transformedOBB, worldMatrix);
	Debug::Draw(DebugRenderer::GetBatch(), transformedOBB, mBaseColor);
}

void BoxCollider::SetExtents()
{
	mGeometry = (PxVec3(mExtents.x / 2.f, mExtents.y / 2.f, mExtents.z / 2.f));
	mShape->setGeometry(mGeometry);
}

void BoxCollider::EditorRendering(EditorViewerType _type)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	ImGui::Text("Position : ");
	if (ImGui::DragFloat3((uid + "Position").c_str(), &mPosition.x, 0.1f, -1000.f, 1000.f))
	{
		SetLocalPosition();
		mOBB.Center = mPosition;
	}

	ImGui::Text("Rotation : ");
	if (ImGui::DragFloat3((uid + "Rotation").c_str(), &mRotation.x, 0.1f, -360.f, 360.f))
	{
		SetRotation();
		mOBB.Orientation = mQuatRotation;
	}

	ImGui::Text("Extents : ");
	if (ImGui::DragFloat3((uid + "Extents").c_str(), &mExtents.x, 0.1f, 0.f, 100.f))
	{
		SetExtents();
		mOBB.Extents = mExtents;
	}

	ImGui::Separator();

	ImGui::Text("isTrigger : "); ImGui::SameLine();
	if (ImGui::Checkbox(("##isTrigger" + uid).c_str(), (bool*)&mIsTrigger))
	{
		SetIsTrigger();
	}

}