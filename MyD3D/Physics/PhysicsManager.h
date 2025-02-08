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
	staticFriction(정지마찰계수): 값이 더 클수록 정지상태로부터 미끄러지기 위해 많은 힘이 필요함. 0~1
	dynamicFriction(동적마찰계수) : 값이 더 클수록 움직이는 상태에서 더 많은 저항을 받음. 0~1
	restituion(반발계수) : 충돌시 물체의 반발 정도. 클수록 탄성 충돌. 0~1*/
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
	// PxQueryFilterCallback의 필터 함수 구현
	PxQueryHitType::Enum preFilter(
		const PxFilterData& filterData,
		const PxShape* shape,
		const PxRigidActor* actor,
		PxHitFlags& queryFlags) override
	{
		// Trigger Shape는 제외
		if (shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
		{
			return PxQueryHitType::eNONE; // 충돌 무시
		}

		// Trigger가 아니면 충돌 처리
		return PxQueryHitType::eBLOCK;
	}

	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit, const PxShape* shape, const PxRigidActor* actor)
	{
		return PxQueryHitType::eBLOCK;
	}

};
