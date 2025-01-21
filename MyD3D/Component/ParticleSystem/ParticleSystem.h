#pragma once
#include "Component/Component.h"

#define maxParticleCount 3000

class ParticleSystem : public Component
{
public:
	explicit ParticleSystem(Object* _owner);
	virtual ~ParticleSystem();
public:
	virtual void Start();
	virtual void Tick();
	virtual void FixedUpdate();
	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();
	virtual void PreRender();
	virtual void Render();
	virtual void Draw(Camera* _camera);
	virtual void PostRender();
	// Editor Only
	virtual void EditorUpdate();
	virtual void EditorRender();

public:
	virtual json Serialize();
	virtual void Deserialize(json& j);

public:
	virtual void EditorRendering(EditorViewerType _viewerType) override;

private:
	//Init Or Renew
	void SetMaterial();
	void SetBuffers();
	void DefaultFogMove();

	//Update
	void FromBufferToGPU();
	void FromGPUToBuffer();
private:
	unsigned int mParticleCount = 100;

	PxPBDParticleSystem* mParticleSystem;
	PxParticleBuffer* mParticleBuffer;
	PxVec4 mPositionsHost[maxParticleCount];
	PxVec4 mVelocitiesHost[maxParticleCount];
	PxU32 mPhasesHost[maxParticleCount];

	//PBDMaterial
	PxPBDMaterial* mMaterial;
	float mFriction = 0.1f;           // ���� ���
	float mDamping = 0.05f;           // ���� ���
	float mAdhesion = 0.0f;           // ������
	float mViscosity = 0.01f;         // ����
	float mVorticity = 0.9f;          // �ҿ뵹�� ����
	float mSurfaceTension = 0.1f;     // ǥ�� ���
	float mCohesion = 0.7f;           // ���շ�
	float mBuoyancy = 0.5f;           // ��·�
	float mAirResistance = 0.1f;      // ���� ����
	float mCflCoefficient = 1.0f;     // CFL ���
	float mGravity = 0.0f;            // �߷�


	class D3DGraphicsVertexBuffer* mVertexBuffer{};
};

