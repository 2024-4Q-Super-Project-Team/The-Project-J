#include "pch.h"
#include "TriangleCollider.h"
#include "Physics/PhysicsManager.h"

TriangleCollider::TriangleCollider(Object* _owner) : Collider(_owner)
{
	SetEID("TriangleCollider");
}

TriangleCollider::~TriangleCollider()
{
	if(mTmesh)
		mTmesh->release();
}

void TriangleCollider::Start()
{
	Collider::Start();


	mMesh = ResourceManager::GetResource<MeshResource>(mMeshHandle);


	if (mMesh == nullptr) return;

	int count = mMesh->mVertices.size();
	std::vector<PxVec3> pxVertices;
	pxVertices.reserve(count);

	for (int i = 0; i < count; i++)
	{
		Vertex vertex = mMesh->mVertices[i];
		pxVertices.push_back(PxVec3(
			vertex.Position.x,
			vertex.Position.y,
			vertex.Position.z));
	}

	int indicesCount = mMesh->mIndices.size();
	std::vector<PxU32> pxIndices;
	pxIndices.reserve(indicesCount);

	for (int i = 0; i < indicesCount; i++)
	{
		pxIndices.push_back(mMesh->mIndices[i]);
	}

	if (indicesCount > 0)
	{
		mTmesh = GameManager::GetPhysicsManager()->CreateTriangleMesh(count, &pxVertices[0], indicesCount / 3, &pxIndices[0]);
		mTriangleGeom = PxTriangleMeshGeometry(mTmesh);


		if (!mTmesh)
		{
			printf("Error: Failed to create PxTriangleMesh!\n");
			return;

		}
		Vector3 meshSize = gameObject->transform->scale;
		PxVec3 scale = PxVec3(meshSize.x, meshSize.y, meshSize.z);
		mTriangleGeom.scale.scale = scale;

		mShape = GameManager::GetPhysicsManager()->GetPhysics()
			->createShape(mTriangleGeom, *GameManager::GetPhysicsManager()->GetDefaultMaterial(), true);
		mShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);

		AddShapeToRigidbody();
	}
}

void TriangleCollider::Tick()
{
	Collider::Tick();
}

void TriangleCollider::FixedUpdate()
{
	Collider::FixedUpdate();
}

void TriangleCollider::PreUpdate()
{
	Collider::PreUpdate();
}

void TriangleCollider::Update()
{
	Collider::Update();
}

void TriangleCollider::PostUpdate()
{
	Collider::PostUpdate();
}

void TriangleCollider::PreRender()
{
	Collider::PreRender();
}

void TriangleCollider::Render()
{
	Collider::Render();
}

void TriangleCollider::Draw(Camera* _camera)
{
	Collider::Draw(_camera);
}

void TriangleCollider::PostRender()
{
	Collider::PostRender();
}

void TriangleCollider::EditorUpdate()
{
	Collider::EditorUpdate();
}

void TriangleCollider::EditorGlobalUpdate()
{
	gameObject->GetOwnerWorld()->
		mNeedResourceHandleTable.insert(mMeshHandle.GetParentkey());
}

void TriangleCollider::EditorRender()
{
	Collider::EditorRender();
}

void _CALLBACK TriangleCollider::OnEnable()
{
	mMesh = ResourceManager::GetResource<MeshResource>(mMeshHandle);
	return void _CALLBACK();
}

void _CALLBACK TriangleCollider::OnDisable()
{
	mMesh = nullptr;
	return void _CALLBACK();
}

void _CALLBACK TriangleCollider::OnDestroy()
{
	return void _CALLBACK();
}

json TriangleCollider::Serialize()
{
	json ret;

	ret["id"] = GetId();
	ret["name"] = "TriangleCollider";
	ret["mesh handle"] = mMeshHandle.Serialize();

	return ret;
}

void TriangleCollider::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if (j.contains("mesh handle"))
	{
		mMeshHandle.Deserialize(j["mesh handle"]);
		mMesh = ResourceManager::GetResource<MeshResource>(mMeshHandle);
	}
}

void TriangleCollider::DrawWire()
{
}

void TriangleCollider::SetPosition()
{
	//PxTransform currentTransform = mShape->getLocalPose();
	//mShape->setLocalPose(PxTransform(PxVec3(mPosition.x, mPosition.y, mPosition.z), currentTransform.q));
}

void TriangleCollider::SetRotation()
{
	//PxTransform currentTransform = mShape->getLocalPose();
	//Vector3 rot = gameObject->transform->GetEulerAngles();
	//mQuatRotation = Quaternion::CreateFromYawPitchRoll(mRotation.y + rot.y, mRotation.x + rot.x, mRotation.z + rot.z);
	//Quaternion PxQuatRotation = Quaternion::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
	//PxQuat pxRot;
	//memcpy_s(&pxRot, sizeof(float) * 4, &PxQuatRotation, sizeof(float) * 4);
	//mShape->setLocalPose(PxTransform(currentTransform.p, pxRot));
}

void TriangleCollider::EditorRendering(EditorViewerType _type)
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
