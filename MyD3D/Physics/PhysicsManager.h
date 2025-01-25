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
	PxMaterial* GetDefaultMaterial() { return mDefaultMaterial; }
	static PickingRay GetPickingRay() { return ray; }
	PxCudaContextManager* GetCudaManager() { return mCudaContextManager; }
	PxCudaContext* GetCudaContext() { return mCudaContext; }

	/*Material
	staticFriction(정지마찰계수): 값이 더 클수록 정지상태로부터 미끄러지기 위해 많은 힘이 필요함. 0~1
	dynamicFriction(동적마찰계수) : 값이 더 클수록 움직이는 상태에서 더 많은 저항을 받음. 0~1
	restituion(반발계수) : 충돌시 물체의 반발 정도. 클수록 탄성 충돌. 0~1*/
	PxMaterial* CreateMaterial(std::wstring name, float staticFriction, float dynamicFriction, float restituion);
	PxMaterial* GetMaterial(std::wstring name) { return mMaterials[name]; }
private:
	PxFoundation* mFoundation;
	bool  recordMemoryAllocations = true;
	PxPvd* mPvd;
	PxPhysics* mPhysics;

	static PickingRay ray;
	PxCudaContextManager* mCudaContextManager;
	PxCudaContext* mCudaContext;

	//Materials
	PxMaterial* mDefaultMaterial;
	std::unordered_map<std::wstring, PxMaterial*> mMaterials;
};

