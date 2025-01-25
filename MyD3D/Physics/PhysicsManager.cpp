#include "pch.h"
#include "PhysicsManager.h"

PhysicsManager::~PhysicsManager()
{
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

    mMaterials["Default"] = mPhysics->createMaterial(0.5f, 0.5f, 0.f);
    mMaterials[u8"¾óÀ½"] = mPhysics->createMaterial(0.01f, 0.01f, 0.f);

   //PxCudaContextManagerDesc cudaContextManagerDesc;
   //mCudaContextManager = PxCreateCudaContextManager(*mFoundation, cudaContextManagerDesc);
   //mCudaContext = mCudaContextManager->getCudaContext();
}

void PhysicsManager::Finalization()
{
    for (auto& m : mMaterials)
        m.second->release();
    mPhysics->release(); 
    mPvd->release();
    mFoundation->release();
    //mCudaContextManager->release();
}

PxMaterial* PhysicsManager::CreateMaterial(std::string name, float staticFriction, float dynamicFriction, float restituion)
{
    PxMaterial* newMat = mPhysics->createMaterial(staticFriction, dynamicFriction, restituion);
    mMaterials[name] = newMat;
    return newMat;
}
