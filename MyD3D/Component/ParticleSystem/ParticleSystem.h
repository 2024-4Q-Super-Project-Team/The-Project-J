#pragma once
#include "Component/Component.h"

#define maxParticleCount 9000


class Texture2DResource;

struct funcArgs
{
	PxPBDParticleSystem* ps;
	PxVec4* positions;
	PxVec4* velocities;
	int particleCount;
};

class ParticleSystem 
	: public Component
	, public IRenderContext
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
public:
	virtual eBlendModeType GetBlendMode() override { return eBlendModeType::TRANSPARENT_BLEND; }
	virtual eRasterizerStateType GetCullingMode() override { return eRasterizerStateType::BACKFACE_CULLING; }
	virtual Vector3 GetDistanceFromCamera(Camera* _camera) override;
	virtual void DrawObject(Matrix& _view, Matrix& _projection) override;
	virtual void DrawShadow(Light* _pLight) override;
	virtual void DrawWire() override;

	virtual void EditorGlobalUpdate() override;
public:
	void SetTexture(ResourceHandle _handle);

private:
	//Init Or Renew
	void SetMaterial();
	void SetBuffers();

	void RegisterFunctions();

private:
	static void DefaultFogMove(funcArgs args);

	//Update
	void FromGPUToHost();
private:
	unsigned int mParticleCount = 300;

	PxPBDParticleSystem* mParticleSystem;
	PxParticleBuffer* mParticleBuffer;
	PxVec4 mPositionsHost[maxParticleCount];
	PxVec4 mVelocitiesHost[maxParticleCount];
	PxU32 mPhasesHost[maxParticleCount];

	//PBDMaterial
	PxPBDMaterial* mMaterial;
	float mFriction = 0.1f;           // 마찰 계수
	float mDamping = 0.05f;           // 감쇠 계수
	float mAdhesion = 0.0f;           // 접착력
	float mViscosity = 0.01f;         // 점성
	float mVorticity = 0.9f;          // 소용돌이 강도
	float mSurfaceTension = 0.1f;     // 표면 장력
	float mCohesion = 0.7f;           // 결합력
	float mBuoyancy = 0.5f;           // 상승력
	float mAirResistance = 0.1f;      // 공기 저항
	float mCflCoefficient = 1.0f;     // CFL 계수
	float mGravity = 0.0f;            // 중력

	ResourceHandle mTextureHandle;
	Texture2DResource*  mTexture;
	int mTextureSize = 5;

	class D3DGraphicsVertexBuffer* mVertexBuffer = nullptr;

	int mFuncIdx = 0;
	std::function<void(funcArgs)> mNowFunc;
	static std::unordered_map<std::string, std::function<void(funcArgs)>> particleFuncMap;
};

