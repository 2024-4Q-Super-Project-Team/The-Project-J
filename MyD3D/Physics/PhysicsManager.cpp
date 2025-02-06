#include "pch.h"
#include "PhysicsManager.h"
#include "Physics/PhysicsEventCallback.h"

PhysicsManager::~PhysicsManager()
{
}


void PhysicsManager::Initialize()
{
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
        gDefaultErrorCallback);
    assert(mFoundation, "PxCreateFoundation failed");

    mPvd = PxCreatePvd(*mFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
    mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
		PxTolerancesScale(), recordMemoryAllocations, mPvd);
    if (!mPhysics)
        assert(mFoundation, "PxCreatePhysics failed");

    mEventCallback = new PhysicsEventCallback;

    mMaterials["Default"] = mPhysics->createMaterial(7.0f, 7.0f, 0.f);
    mMaterials[u8"¾óÀ½"] = mPhysics->createMaterial(0.01f, 0.01f, 0.f);

   PxCudaContextManagerDesc cudaContextManagerDesc;
   mCudaContextManager = PxCreateCudaContextManager(*mFoundation, cudaContextManagerDesc);
   mCudaContext = mCudaContextManager->getCudaContext();
}

void PhysicsManager::Finalization()
{
    for (auto& m : mMaterials)
        m.second->release();
    mPhysics->release(); 
    mPvd->release();
    mFoundation->release();
    mCudaContextManager->releaseContext();
    SAFE_DELETE(mEventCallback);
}

PxMaterial* PhysicsManager::CreateMaterial(std::string name, float staticFriction, float dynamicFriction, float restituion)
{
    PxMaterial* newMat = mPhysics->createMaterial(staticFriction, dynamicFriction, restituion);
    mMaterials[name] = newMat;
    return newMat;
}

PxConvexMesh* PhysicsManager::CreateConvexMesh(PxU32 numVerts, const PxVec3* verts)
{
	PxTolerancesScale tolerances;
	PxCookingParams params(tolerances);

	// Use the new (default) PxConvexMeshCookingType::eQUICKHULL
	params.convexMeshCookingType = PxConvexMeshCookingType::eQUICKHULL;

	// If the gaussMapLimit is chosen higher than the number of output vertices, no gauss map is added to the convex mesh data (here 256).
	// If the gaussMapLimit is chosen lower than the number of output vertices, a gauss map is added to the convex mesh data (here 16).
	params.gaussMapLimit = 16;

	// Setup the convex mesh descriptor
	PxConvexMeshDesc desc;

	// We provide points only, therefore the PxConvexFlag::eCOMPUTE_CONVEX flag must be specified
	desc.points.data = verts;
	desc.points.count = numVerts;
	desc.points.stride = sizeof(PxVec3);
	desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
	desc.vertexLimit = 255;

	PxU32 meshSize = 0;
	PxConvexMesh* convex = NULL;

	// Serialize the cooked mesh into a stream.
	PxDefaultMemoryOutputStream outStream;
	bool res = PxCookConvexMesh(params, desc, outStream);
	PX_UNUSED(res);
	PX_ASSERT(res);
	meshSize = outStream.getSize();

	// Create the mesh from a stream.
	PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());
	convex = mPhysics->createConvexMesh(inStream);
	PX_ASSERT(convex);


	return convex;
}

PxTriangleMesh* PhysicsManager::CreateTriangleMesh(PxU32 numVerts, const PxVec3* verts, PxU32 numTriangles, const PxU32* indices)
{
	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = numVerts;
	meshDesc.points.data = verts;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.triangles.count = numTriangles;
	meshDesc.triangles.data = indices;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);

	PxTolerancesScale scale;
	PxCookingParams params(scale);

	// Create BVH33 midphase
	params.midphaseDesc = PxMeshMidPhase::eBVH33;

	// setup common cooking params
	params.suppressTriangleMeshRemapTable = true;
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	params.meshPreprocessParams &= ~static_cast<PxMeshPreprocessingFlags>(PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);
	params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eCOOKING_PERFORMANCE;
	params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.55f;

#if defined(PX_CHECKED) || defined(PX_DEBUG)

	PX_ASSERT(PxValidateTriangleMesh(params, meshDesc));

#endif // DEBUG

	PxTriangleMesh* triMesh = NULL;
	PxU32 meshSize = 0;
	triMesh = PxCreateTriangleMesh(params, meshDesc, mPhysics->getPhysicsInsertionCallback());

	return triMesh;
}