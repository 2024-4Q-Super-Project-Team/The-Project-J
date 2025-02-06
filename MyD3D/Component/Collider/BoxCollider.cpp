#include "pch.h"
#include "BoxCollider.h"
#include "Physics/PhysicsManager.h"
#include "Graphics/GraphicsManager.h"

BoxCollider::BoxCollider(Object* _owner) :Collider(_owner)
{
	SetEID("BoxCollider");

	mExtents = mInitialSize;
	mGeometry = PxBoxGeometry(PxVec3(mExtents.x, mExtents.y, mExtents.z));
	mShape = GameManager::GetPhysicsManager()->GetPhysics()
		->createShape(mGeometry, *GameManager::GetPhysicsManager()->GetDefaultMaterial(), true);
	mShape->userData = this;
	mShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);
	
	mOBB.Center = gameObject->transform->position;
	mOBB.Orientation = Quaternion::Identity;
	mOBB.Extents = mExtents;

}

void BoxCollider::Start()
{
	Collider::Start();

	SetExtents();
	SetRotation();
	SetPosition();
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

	SetExtents();
	SetRotation();
	SetPosition();
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
	SetExtents();
	SetRotation();
	SetPosition();
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
	ret["material"] = mMaterialIdx;
	return ret;
}

void BoxCollider::Deserialize(json& j)
{
	SetId(j["id"].get<unsigned int>());

	if(j.contains("isTrigger"))
		mIsTrigger = j["isTrigger"].get<bool>();

	if (j.contains("position"))
	{
		mPosition.x = j["position"][0].get<float>();
		mPosition.y = j["position"][1].get<float>();
		mPosition.z = j["position"][2].get<float>();
		mOBB.Center = mPosition;
	}
	if (j.contains("rotation"))
	{
		mRotation.x = j["rotation"][0].get<float>();
		mRotation.y = j["rotation"][1].get<float>();
		mRotation.z = j["rotation"][2].get<float>();
		mOBB.Orientation = mQuatRotation;
	}
	if (j.contains("extents"))
	{
		mExtents.x = j["extents"][0].get<float>();
		mExtents.y = j["extents"][1].get<float>();
		mExtents.z = j["extents"][2].get<float>();
		mOBB.Extents.x = gameObject->transform->scale.x * mExtents.x;
		mOBB.Extents.y = gameObject->transform->scale.y * mExtents.y;
		mOBB.Extents.z = gameObject->transform->scale.z * mExtents.z;
	}

	if (j.contains("material"))
	{
		mMaterialIdx = j["material"];
		SetMaterial(mMaterials[mMaterialIdx]);
	}

	SetExtents();
	SetRotation();
	SetPosition();
}

void BoxCollider::DrawWire()
{
	mOBB.Center = gameObject->transform->GetWorldPosition() + mPosition;
	mOBB.Orientation = gameObject->transform->GetWorldRotation() *  mQuatRotation;
	mOBB.Extents = gameObject->transform->GetWorldScale() * mExtents;
	Debug::Draw(DebugRenderer::GetBatch(), mOBB, mBaseColor);
}

void BoxCollider::SetPosition()
{
	PxTransform currentTransform = mShape->getLocalPose();
	mShape->setLocalPose(PxTransform(PxVec3(mPosition.x, mPosition.y, mPosition.z), currentTransform.q));
	
}

void BoxCollider::SetRotation()
{
	PxTransform currentTransform = mShape->getLocalPose();
	Vector3 rot = gameObject->transform->GetEulerAngles();
	mQuatRotation = Quaternion::CreateFromYawPitchRoll(mRotation.y + rot.y, mRotation.x + rot.x, mRotation.z + rot.z);
	Quaternion PxQuatRotation = Quaternion::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
	PxQuat pxRot;
	memcpy_s(&pxRot, sizeof(float) * 4, &PxQuatRotation, sizeof(float) * 4);
	mShape->setLocalPose(PxTransform(currentTransform.p, pxRot));
}

void BoxCollider::SetExtents()
{
	Vector3 size = gameObject->transform->scale;
	mGeometry = PxBoxGeometry(PxVec3(size.x * mExtents.x, size.y * mExtents.y, size.z * mExtents.z));
	mShape->setGeometry(mGeometry);
}

Vector3 BoxCollider::GetSize()
{
	return mExtents * gameObject->transform->scale;
}

void BoxCollider::EditorRendering(EditorViewerType _type)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	ImGui::Text("Position : ");
	ImGui::DragFloat3((uid + "Position").c_str(), &mPosition.x, 0.1f, -90000.f, 90000.f);

	ImGui::Text("Rotation : ");
	ImGui::DragFloat3((uid + "Rotation").c_str(), &mRotation.x, 0.1f, -360.f, 360.f);

	ImGui::Text("Extents : ");
	ImGui::DragFloat3((uid + "Extents").c_str(), &mExtents.x, 0.1f, 0.1f, 100.f);

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

	SetExtents();
	SetRotation();
	SetPosition();
}