#include "pch.h"
#include "PhysicsManager.h"

PhysicsManager::~PhysicsManager()
{
    mPhysics->release();
    mFoundation->release();
}


void PhysicsManager::Initialize()
{
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
        gDefaultErrorCallback);
    assert(mFoundation, "PxCreateFoundation failed");

    mPvd = PxCreatePvd(*mFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
        PxTolerancesScale(), recordMemoryAllocations, mPvd);
    if (!mPhysics)
        assert(mFoundation, "PxCreatePhysics failed");

    mDefaultMaterial = mPhysics->createMaterial(0.5f, 0.5f, 0.f);

    //PxCudaContextManagerDesc cudaContextManagerDesc;
    //mCudaContextManager = PxCreateCudaContextManager(*mFoundation, cudaContextManagerDesc);
    //mCudaContext = mCudaContextManager->getCudaContext();
}

void PhysicsManager::Finalization()
{
    mFoundation->release();
    mPvd->release();
    mPhysics->release();
    mDefaultMaterial->release();
    //mCudaContextManager->release();
}
