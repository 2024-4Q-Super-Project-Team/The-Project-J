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

private:
	PxFoundation* mFoundation;
	bool  recordMemoryAllocations = true;
	PxPvd* mPvd;
	PxPhysics* mPhysics;
	PxMaterial* mDefaultMaterial;

	static PickingRay ray;
	PxCudaContextManager* mCudaContextManager;
	PxCudaContext* mCudaContext;
};

