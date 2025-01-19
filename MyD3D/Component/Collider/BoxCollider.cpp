#include "pch.h"
#include "BoxCollider.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"

BoxCollider::BoxCollider(Object* _owner) :Collider(_owner)
{
	mGeometry = PxBoxGeometry(PxVec3(mInitialSize.x, mInitialSize.y, mInitialSize.z));
	mShape = GameManager::GetPhysicsManager()->GetPhysics()
		->createShape(mGeometry, *GameManager::GetPhysicsManager()->GetDefaultMaterial());
	mShape->userData = this;
	mShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	UpdateOBB();
}

void BoxCollider::Start()
{
}

void BoxCollider::Tick()
{
}

void BoxCollider::FixedUpdate()
{
}

void BoxCollider::PreUpdate()
{
}

void BoxCollider::Update()
{
}

void BoxCollider::PostUpdate()
{
}

void BoxCollider::PreRender()
{
}

void BoxCollider::Render()
{
}

void BoxCollider::Draw(Camera* _camera)
{
	_camera->PushDrawList(this);
}

void BoxCollider::PostRender()
{
}

json BoxCollider::Serialize()
{
	json ret;

	ret["position"] = { mPosition.x, mPosition.y, mPosition.z };
	ret["rotation"] = { mRotation.x, mRotation.y, mRotation.z };
	ret["extents"] = { mExtents.x, mExtents.y, mExtents.z };


	return ret;
}

void BoxCollider::Deserialize(json& j)
{
}

void BoxCollider::DrawMesh(Camera* _camera)
{
#ifdef _DEBUG
	GraphicsManager::DebugDrawBegin();
	Debug::Draw(GraphicsManager::GetBatch(), mOBB, mBaseColor);
	GraphicsManager::DebugDrawEnd();
#endif
}

void BoxCollider::SetExtents()
{
	mGeometry.halfExtents = PxVec3(mExtents.x / 2.f, mExtents.y / 2.f, mExtents.z / 2.f );
	mOBB.Extents = mExtents;
}

void BoxCollider::UpdateOBB()
{
#ifdef _DEBUG

	Matrix localTransform = XMMatrixScaling(mOBB.Extents.x, mOBB.Extents.y, mOBB.Extents.z)
		* XMMatrixRotationQuaternion(mQuatRotation)
		* XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

	Matrix objectTransform = gameObject->transform->GetWorldMatrix();

	mOBB.Transform(mOBB, objectTransform * localTransform);

#endif
}

void BoxCollider::EditorRendering(EditorViewerType _type)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));
    if (ImGui::TreeNodeEx(("BoxComponent" + uid).c_str(), EDITOR_FLAG_MAIN))
    {
        ImGui::Separator();

        ImGui::Text("Position : ");
		if (ImGui::DragFloat3((uid + "Position").c_str(), &mPosition.x, 0.f, 0.f, 0.f))
		{
			SetLocalPosition();
			UpdateOBB();
		}

        ImGui::Text("Rotation : ");
		if (ImGui::DragFloat3((uid + "Rotation").c_str(), &mRotation.x, 0.f, 0.f, 0.f))
		{
			SetRotation();
			UpdateOBB();
		}

        ImGui::Text("Extents : ");
		if (ImGui::DragFloat3((uid + "Extents").c_str(), &mExtents.x, 0.f, 0.f, 0.f))
		{
			SetExtents();
			UpdateOBB();
		}

        ImGui::Separator();

		if (ImGui::Checkbox(("isTrigger" + uid).c_str(), (bool*)&mIsTrigger))
		{
			SetIsTrigger();
		}
        ImGui::Text("isTrigger : ");

        ImGui::TreePop();
    }
}
