#include "pch.h"
#include "World.h"
#include "GameManager/GameManager.h"
#include "Physics/PhysicsManager.h"

DXWorld::DXWorld(std::wstring_view _name, std::wstring_view _tag)
	: Entity(_name, _tag)
	, mWorldTransform(new Transform(this))
{
	PxPhysics* physics = GameManager::GetPhysicsManager()->GetPhysics();
	PxSceneDesc sceneDesc(physics->getTolerancesScale());
	
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	mScene = physics->createScene(sceneDesc);
}

DXWorld::~DXWorld()
{
	SAFE_DELETE_VECTOR(mObjectGroups)
	SAFE_DELETE(mWorldTransform)
}

void DXWorld::Tick()
{
	// 생성 삭제 정렬 처리
	UpdateGroup();
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Tick())
}

void DXWorld::FixedUpdate()
{
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, FixedUpdate())
	// 이거 오브젝트 FixedUpdate 후에 하는게 맞나여? 혹시나해서 확인 부탁여
	if (mScene)
	{
		mScene->simulate(GameManager::GetFixedUpdateTick());
		mScene->fetchResults(true);
	}
}

void DXWorld::PreUpdate()
{
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PreUpdate());
}

void DXWorld::Update()
{
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Update());
}

void DXWorld::PostUpdate()
{
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PostUpdate());
}

void DXWorld::PreRender()
{
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PreRender());
}

void DXWorld::Render(GraphicsManager* _graphicsManager)
{
	if (mWorldTransform)
	{
		mWorldTransform->UpdateMatrix();
	}
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Render(_graphicsManager));
}

void DXWorld::PostRender()
{
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, PostRender());
}

ObjectGroup* DXWorld::CreateObjectGroup(std::wstring_view _name, std::wstring_view _tag)
{
	ObjectGroup* instance = GetObjectGroup(_name);
	if (instance == nullptr)
	{
		ObjectGroup* instance = new ObjectGroup(_name, _tag);
		mObjectGroups.push_back(instance);
	}
	return instance;
}

void DXWorld::CreateObjectGroup(ObjectGroup* _recvGroup)
{
	if (_recvGroup == nullptr)
		return;
	ObjectGroup* instance = GetObjectGroup(_recvGroup->GetName());
	// 받은 그룹이 현재 컨테이너에 없다면 그대로 옮겨온다.
	if (instance == nullptr) 
	{
		// 받은 그룹이 속한 월드의 그룹컨테이너에서 해당 그룹을 지우고 여기로 옮긴다.
		auto itr = FIND_CONTAINER(_recvGroup->GetWorld()->mObjectGroups,
			[&_recvGroup](ObjectGroup* _group) {
			return (_recvGroup == _group);
			});
		if (FIND_SUCCES(itr, mObjectGroups))
		{
			_recvGroup->GetWorld()->mObjectGroups.erase(itr);
			mObjectGroups.push_back(_recvGroup);
			_recvGroup->SetCreate();
		}
	}
	// 이미 있으면 오브젝트들을 가져와서 뒤로 붙인다.
	// 근데 여기서 의문은 오브젝트들을 비활성화해서 가져와야하나?
	else
	{
		mObjectGroups.insert(mObjectGroups.end(), _recvGroup->GetWorld()->mObjectGroups.begin(), _recvGroup->GetWorld()->mObjectGroups.end());
	}
}

ObjectGroup* DXWorld::GetObjectGroup(std::wstring_view _name)
{
	auto itr = FIND_CONTAINER(mObjectGroups,
		[&_name](ObjectGroup* _group) {
			return (_group->GetName() == _name.data());
		});
	return (FIND_SUCCES(itr, mObjectGroups)) ? (*itr) : nullptr;
}

void DXWorld::UpdateGroup()
{
	// 1. 정렬 처리 (Destroy 상태를 맨 뒤로 보냄)
	// 좌측 대상이 Destroy면 뒤로 보내야하고, 우측이 Destroy면 넘어가야함
	std::sort(mObjectGroups.begin(), mObjectGroups.end(),
		[](const ObjectGroup* left, const ObjectGroup* right) {
			if (left->GetState() == EntityState::Destroy && right->GetState() != EntityState::Destroy)
				return false;
			if (left->GetState() != EntityState::Destroy && right->GetState() == EntityState::Destroy)
				return true;
			return left->GetOrder() < right->GetOrder(); // 오름차순
		}
	);
	// 2. 삭제 및 생성 처리
	for (auto itr = mObjectGroups.begin(); itr != mObjectGroups.end();)
	{
		// 삭제
		if ((*itr)->GetState() == EntityState::Destroy)
		{
			itr = mObjectGroups.erase(itr);
			continue;
		}
		// 생성
		if ((*itr)->GetState() == EntityState::Create)
		{
			(*itr)->SetActive(true);
		}
		++itr;
	}
}
