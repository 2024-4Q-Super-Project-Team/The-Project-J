#include "pch.h"
#include "ConvexCollider.h"
#include "Physics/PhysicsManager.h"

ConvexCollider::ConvexCollider(Object* _owner) : Collider(_owner)
{
	SetEID("ConvexCollider");
}

ConvexCollider::~ConvexCollider()
{
	if(mCmesh)
		mCmesh->release();
}

void ConvexCollider::Start()
{
	Collider::Start();


	mMesh = ResourceManager::GetResource<MeshResource>(mMeshHandle);


	if (mMesh == nullptr) return;

	std::vector<PxVec3> pxVertices;
	int count = mMesh->mVertices.size();

	for (int i = 0; i < count; i++)
	{
		Vertex vertex = mMesh->mVertices[i];
		pxVertices.push_back(PxVec3(
			vertex.Position.x,
			vertex.Position.y,
			vertex.Position.z));
	}

	if (count > 0)
	{
		mCmesh = GameManager::GetPhysicsManager()->CreateConvexMesh(count, &pxVertices[0]);
		mConvexGeom = PxConvexMeshGeometry(mCmesh);

		Vector3 meshSize = gameObject->transform->scale;
		PxVec3 scale = PxVec3(meshSize.x, meshSize.y, meshSize.y);
		mConvexGeom.scale.scale = scale;

		mShape = GameManager::GetPhysicsManager()->GetPhysics()
			->createShape(mConvexGeom, *GameManager::GetPhysicsManager()->GetDefaultMaterial(), true);
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);

		AddShapeToRigidbody();
	}

	SetPosition();
	SetRotation();
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

void ConvexCollider::EditorGlobalUpdate()
{
	gameObject->GetOwnerWorld()->
		mNeedResourceHandleTable.insert(mMeshHandle.GetParentkey());
}

void ConvexCollider::EditorRender()
{
	Collider::EditorRender();
}

void _CALLBACK ConvexCollider::OnEnable()
{
	mMesh = ResourceManager::GetResource<MeshResource>(mMeshHandle);
	return void _CALLBACK();
}

void _CALLBACK ConvexCollider::OnDisable()
{
	mMesh = nullptr;
	return void _CALLBACK();
}

void _CALLBACK ConvexCollider::OnDestroy()
{
	return void _CALLBACK();
}

json ConvexCollider::Serialize()
{
	json ret;

	ret["id"] = GetId();
	ret["name"] = "ConvexCollider";
	ret["mesh handle"] = mMeshHandle.Serialize();

	return ret;
}

void ConvexCollider::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if (j.contains("mesh handle"))
	{
		mMeshHandle.Deserialize(j["mesh handle"]);
		mMesh = ResourceManager::GetResource<MeshResource>(mMeshHandle);
	}
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

	ImGui::Text("Mesh : ");

	std::string widgetID = "NULL Mesh";
	if (mMesh)
	{
		mMesh->EditorRendering(EditorViewerType::DEFAULT);
		widgetID = mMesh->GetEID();
		mMesh->EditorRendering(EditorViewerType::INSPECTOR);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, EDITOR_COLOR_NULL);
		ImGui::Selectable(widgetID.c_str(), false, ImGuiSelectableFlags_Highlight);
		EDITOR_COLOR_POP(1);
	}

	if (EditorDragNDrop::ReceiveDragAndDropResourceData<MeshResource>(widgetID.c_str(), &mMeshHandle))
	{
		if (mMeshHandle.GetResourceType() == eResourceType::MeshResource)
		{
			mMesh = ResourceManager::GetResource<MeshResource>(mMeshHandle);
		}
	}
}
