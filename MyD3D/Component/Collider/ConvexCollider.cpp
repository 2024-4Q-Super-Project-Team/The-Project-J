#include "pch.h"
#include "ConvexCollider.h"
#include "Physics/PhysicsManager.h"

ConvexCollider::ConvexCollider(Object* _owner) : Collider(_owner)
{
	SetEID("ConvexCollider");
}

void ConvexCollider::Start()
{
	Collider::Start();

	MeshRenderer* mr = gameObject->GetComponent<MeshRenderer>();
	if (mr == nullptr) return;

	MeshResource* mesh = mr->GetMesh();
	if (mesh == nullptr) return;

	std::vector<PxVec3> pxVertices;
	int count = mesh->mVertices.size();

	for (int i = 0; i < count; i++)
	{
		Vertex vertex = mesh->mVertices[i];
		pxVertices.push_back(PxVec3(
			vertex.Position.x,
			vertex.Position.y,
			vertex.Position.z));
	}

	if (count > 0)
	{
		PxConvexMesh* cmesh = GameManager::GetPhysicsManager()->CreateConvexMesh(count, &pxVertices[0]);
		mConvexGeom = PxConvexMeshGeometry(cmesh);

		Vector3 meshSize = gameObject->transform->scale;
		PxVec3 scale = PxVec3(meshSize.x, meshSize.y, meshSize.y);
		
		Quaternion quatRot = gameObject->transform->rotation;
		PxQuat rotation = PxQuat(quatRot.x, quatRot.y, quatRot.z, quatRot.w);
		
		PxMeshScale meshScale(PxVec3(1,1,1), rotation);
		mConvexGeom.scale = meshScale;

		mShape = GameManager::GetPhysicsManager()->GetPhysics()
			->createShape(mConvexGeom, *GameManager::GetPhysicsManager()->GetDefaultMaterial(), true);
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);

		AddShapeToRigidbody();
	}
}

void ConvexCollider::Tick()
{
	Collider::Tick();
}

void ConvexCollider::FixedUpdate()
{
	Collider::FixedUpdate();
}

void ConvexCollider::PreUpdate()
{
	Collider::PreUpdate();
}

void ConvexCollider::Update()
{
	Collider::Update();
}

void ConvexCollider::PostUpdate()
{
	Collider::PostUpdate();
}

void ConvexCollider::PreRender()
{
	Collider::PreRender();
}

void ConvexCollider::Render()
{
	Collider::Render();
}

void ConvexCollider::Draw(Camera* _camera)
{
	Collider::Draw(_camera);
}

void ConvexCollider::PostRender()
{
	Collider::PostRender();
}

void ConvexCollider::EditorUpdate()
{
	Collider::EditorUpdate();
}

void ConvexCollider::EditorRender()
{
	Collider::EditorRender();
}

json ConvexCollider::Serialize()
{
	json ret;

	ret["id"] = GetId();
	ret["name"] = "ConvexCollider";

	return ret;
}

void ConvexCollider::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

}

void ConvexCollider::DrawWire()
{
}

void ConvexCollider::SetPosition()
{
	PxTransform currentTransform = mShape->getLocalPose();
	mShape->setLocalPose(PxTransform(PxVec3(mPosition.x, mPosition.y, mPosition.z), currentTransform.q));
}

void ConvexCollider::SetRotation()
{
	PxTransform currentTransform = mShape->getLocalPose();
	Vector3 rot = gameObject->transform->GetEulerAngles();
	mQuatRotation = Quaternion::CreateFromYawPitchRoll(mRotation.y + rot.y, mRotation.x + rot.x, mRotation.z + rot.z);
	Quaternion PxQuatRotation = Quaternion::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
	PxQuat pxRot;
	memcpy_s(&pxRot, sizeof(float) * 4, &PxQuatRotation, sizeof(float) * 4);
	mShape->setLocalPose(PxTransform(currentTransform.p, pxRot));
}

void ConvexCollider::EditorRendering(EditorViewerType _type)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

}
