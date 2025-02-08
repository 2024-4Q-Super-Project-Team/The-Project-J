#pragma once

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

#include "PickingRay.h"

class PhysicsManager
{
public:
	~PhysicsManager();

public:
	void Initialize();
	void Finalization();

	PxPhysics* GetPhysics() const { return mPhysics; }
	PxSimulationEventCallback* GetCallback() { return mEventCallback; }
	static PickingRay GetPickingRay() { return ray; }
	PxCudaContextManager* GetCudaManager() { return mCudaContextManager; }
	PxCudaContext* GetCudaContext() { return mCudaContext; }

	/*Material
	staticFriction(�����������): ���� �� Ŭ���� �������·κ��� �̲������� ���� ���� ���� �ʿ���. 0~1
	dynamicFriction(�����������) : ���� �� Ŭ���� �����̴� ���¿��� �� ���� ������ ����. 0~1
	restituion(�ݹ߰��) : �浹�� ��ü�� �ݹ� ����. Ŭ���� ź�� �浹. 0~1*/
	PxMaterial* CreateMaterial(std::string name, float staticFriction, float dynamicFriction, float restituion);
	PxMaterial* GetMaterial(std::string name) { return mMaterials[name]; }	
	PxMaterial* GetDefaultMaterial() { return mMaterials["Default"]; }
	std::unordered_map<std::string, PxMaterial*> GetMaterials() { return mMaterials; }

	PxConvexMesh* CreateConvexMesh(PxU32 numVerts, const PxVec3* verts);
	PxTriangleMesh* CreateTriangleMesh(PxU32 numVerts, const PxVec3* verts, PxU32 numTriangles, const PxU32* indices);

private:
	PxFoundation* mFoundation;
	bool  recordMemoryAllocations = true;
	PxPvd* mPvd;
	PxPhysics* mPhysics;
	PxSimulationEventCallback* mEventCallback;

	static PickingRay ray;
	PxCudaContextManager* mCudaContextManager;
	PxCudaContext* mCudaContext;

	//Materials
	std::unordered_map<std::string, PxMaterial*> mMaterials;
};


class CustomRaycastFilter : public PxQueryFilterCallback
{
public:
	// PxQueryFilterCallback�� ���� �Լ� ����
	PxQueryHitType::Enum preFilter(
		const PxFilterData& filterData,
		const PxShape* shape,
		const PxRigidActor* actor,
		PxHitFlags& queryFlags) override
	{
		// Trigger Shape�� ����
		if (shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
		{
			return PxQueryHitType::eNONE; // �浹 ����
		}

		// Trigger�� �ƴϸ� �浹 ó��
		return PxQueryHitType::eBLOCK;
	}

	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
	{
		return PxQueryHitType::eBLOCK;
	}

};
