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
	PxMaterial* GetDefaultMaterial() { return mMaterials["Default"]; }
	static PickingRay GetPickingRay() { return ray; }
	PxCudaContextManager* GetCudaManager() { return mCudaContextManager; }
	PxCudaContext* GetCudaContext() { return mCudaContext; }

	/*Material
	staticFriction(�����������): ���� �� Ŭ���� �������·κ��� �̲������� ���� ���� ���� �ʿ���. 0~1
	dynamicFriction(�����������) : ���� �� Ŭ���� �����̴� ���¿��� �� ���� ������ ����. 0~1
	restituion(�ݹ߰��) : �浹�� ��ü�� �ݹ� ����. Ŭ���� ź�� �浹. 0~1*/
	PxMaterial* CreateMaterial(std::string name, float staticFriction, float dynamicFriction, float restituion);
	PxMaterial* GetMaterial(std::string name) { return mMaterials[name]; }
	std::unordered_map<std::string, PxMaterial*> GetMaterials() { return mMaterials; }
private:
	PxFoundation* mFoundation;
	bool  recordMemoryAllocations = true;
	PxPvd* mPvd;
	PxPhysics* mPhysics;

	static PickingRay ray;
	PxCudaContextManager* mCudaContextManager;
	PxCudaContext* mCudaContext;

	//Materials
	std::unordered_map<std::string, PxMaterial*> mMaterials;
};

