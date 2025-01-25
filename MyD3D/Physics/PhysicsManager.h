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
	staticFriction(�����������): ���� �� Ŭ���� �������·κ��� �̲������� ���� ���� ���� �ʿ���. 0~1
	dynamicFriction(�����������) : ���� �� Ŭ���� �����̴� ���¿��� �� ���� ������ ����. 0~1
	restituion(�ݹ߰��) : �浹�� ��ü�� �ݹ� ����. Ŭ���� ź�� �浹. 0~1*/
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

