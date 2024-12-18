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
	// ���� ���� ���� ó��
	UpdateGroup();
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, Tick())
}

void DXWorld::FixedUpdate()
{
	FOR_LOOP_ARRAY_ENTITY(mObjectGroups, FixedUpdate())
	// �̰� ������Ʈ FixedUpdate �Ŀ� �ϴ°� �³���? Ȥ�ó��ؼ� Ȯ�� ��Ź��
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
	// ���� �׷��� ���� �����̳ʿ� ���ٸ� �״�� �Űܿ´�.
	if (instance == nullptr) 
	{
		// ���� �׷��� ���� ������ �׷������̳ʿ��� �ش� �׷��� ����� ����� �ű��.
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
	// �̹� ������ ������Ʈ���� �����ͼ� �ڷ� ���δ�.
	// �ٵ� ���⼭ �ǹ��� ������Ʈ���� ��Ȱ��ȭ�ؼ� �����;��ϳ�?
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
	// 1. ���� ó�� (Destroy ���¸� �� �ڷ� ����)
	// ���� ����� Destroy�� �ڷ� �������ϰ�, ������ Destroy�� �Ѿ����
	std::sort(mObjectGroups.begin(), mObjectGroups.end(),
		[](const ObjectGroup* left, const ObjectGroup* right) {
			if (left->GetState() == EntityState::Destroy && right->GetState() != EntityState::Destroy)
				return false;
			if (left->GetState() != EntityState::Destroy && right->GetState() == EntityState::Destroy)
				return true;
			return left->GetOrder() < right->GetOrder(); // ��������
		}
	);
	// 2. ���� �� ���� ó��
	for (auto itr = mObjectGroups.begin(); itr != mObjectGroups.end();)
	{
		// ����
		if ((*itr)->GetState() == EntityState::Destroy)
		{
			itr = mObjectGroups.erase(itr);
			continue;
		}
		// ����
		if ((*itr)->GetState() == EntityState::Create)
		{
			(*itr)->SetActive(true);
		}
		++itr;
	}
}
