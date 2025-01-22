#include "pch.h"
#include "ParticleSystem.h"
#include "Physics/PhysicsManager.h"
#include "physx/cudamanager/PxCudaContextManager.h"
#include "physx/cudamanager/PxCudaContext.h"
#include "World/World.h"
#include "ObjectGroup/ObjectGroup.h"
#include "Graphics/GraphicsManager.h"
#include "Resource/Graphics/Texture/Texture.h"

ParticleSystem::ParticleSystem(Object* _owner)
    : Component(_owner)
{
	SetEID("ParticleSystem");
	ResourceHandle texHandle = {
			eResourceType::Texture2DResource,
			L"Test_Particle_Texture",
			L"",
			L"resource/texture/ParticleSample/fire_01.png"
	};
	mTexture = new Texture2DResource(texHandle);

	mParticleSystem = GameManager::GetPhysicsManager()->GetPhysics()
		->createPBDParticleSystem(*GameManager::GetPhysicsManager()->GetCudaManager(), 96);

	mParticleBuffer = GameManager::GetPhysicsManager()->GetPhysics()
		->createParticleBuffer(maxParticleCount, 10, GameManager::GetPhysicsManager()->GetCudaManager());

	gameObject->GetOwnerObjectGroup()->GetWorld()->AddPxActor(mParticleSystem);

	//초기 설정 
	SetMaterial();
	DefaultFogMove();
	SetBuffers();
}

ParticleSystem::~ParticleSystem()
{
	mParticleSystem->release();
	mParticleBuffer->release();
	SAFE_DELETE(mVertexBuffer);
}

void ParticleSystem::Start()
{
}

void ParticleSystem::Tick()
{
}

void ParticleSystem::FixedUpdate()
{
}

void ParticleSystem::PreUpdate()
{
}

void ParticleSystem::Update()
{
}

void ParticleSystem::PostUpdate()
{
}

void ParticleSystem::PreRender()
{
}

void ParticleSystem::Render()
{
}

void ParticleSystem::Draw(Camera* _camera)
{
	_camera->PushDrawList(this);
}

void ParticleSystem::PostRender()
{
}

void ParticleSystem::EditorUpdate()
{
}

void ParticleSystem::EditorRender()
{
	if(mTexture)
		EditorManager::mEditorCamera.PushDrawList(this);
}

json ParticleSystem::Serialize()
{
    return json();
}

void ParticleSystem::Deserialize(json& j)
{
}

void ParticleSystem::EditorRendering(EditorViewerType _viewerType)
{
	std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

	ImGui::Text(u8"파티클 개수 : ");
	ImGui::DragInt((uid + "Particle Count").c_str(), (int*)&mParticleCount, 100, 0, maxParticleCount);

	ImGui::Separator();
	ImGui::Text("Material Setting : ");
	ImGui::Separator();

	ImGui::Text(u8"마찰 계수:");
	ImGui::DragFloat((uid + "mFriction").c_str(), &mFriction, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"감쇠 계수:");
	ImGui::DragFloat((uid + "mDamping").c_str(), &mDamping, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"접착력:");
	ImGui::DragFloat((uid + "mAdhesion").c_str(), &mAdhesion, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"점성:");
	ImGui::DragFloat((uid + "mViscosity").c_str(), &mViscosity, 0.001f, 0.0f, 1.0f, "%.3f");

	ImGui::Text(u8"소용돌이 강도:");
	ImGui::DragFloat((uid + "mVorticity").c_str(), &mVorticity, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"표면 장력:");
	ImGui::DragFloat((uid + "mSurfaceTension").c_str(), &mSurfaceTension, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"결합력:");
	ImGui::DragFloat((uid + "mCohesion").c_str(), &mCohesion, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"상승력:");
	ImGui::DragFloat((uid + "mBuoyancy").c_str(), &mBuoyancy, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"공기 저항:");
	ImGui::DragFloat((uid + "mAirResistance").c_str(), &mAirResistance, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"CFL 계수:");
	ImGui::DragFloat((uid + "mCflCoefficient").c_str(), &mCflCoefficient, 0.01f, 0.0f, 10.0f, "%.2f");

	ImGui::Text(u8"중력:");
	ImGui::DragFloat((uid + "mGravity").c_str(), &mGravity, 0.01f, -10.0f, 10.0f, "%.2f");
}

Vector3 ParticleSystem::GetDistanceFromCamera(Camera* _camera)
{
	return _camera->GetDistance(gameObject->transform);
}

void ParticleSystem::DrawObject(Matrix& _view, Matrix& _projection)
{
	D3DGraphicsRenderer::SetTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	//// 리소스 바인딩
	GraphicsManager::GetVertexShader(eVertexShaderType::PARTICLE)->Bind();
	GraphicsManager::GetGeometryShader(eGeometryShaderType::PARTICLE)->Bind();
	GraphicsManager::GetPixelShader(ePixelShaderType::PARTICLE)->Bind();
	mTexture->Texture->Bind();
	
	TransformCBuffer cb;
	cb.World = gameObject->transform->GetWorldMatrix();
	cb.View = XMMatrixTranspose(_view);
	cb.Projection = XMMatrixTranspose(_projection);
	
	ParticleSizeCBuffer pcb;
	pcb.particleSize = mTextureSize;
	
	mVertexBuffer->Bind();
	GraphicsManager::GetConstantBuffer(eCBufferType::Transform)->UpdateGPUResoure(&cb);
	GraphicsManager::GetConstantBuffer(eCBufferType::ParticleSize)->UpdateGPUResoure(&pcb);
	
	D3DGraphicsRenderer::DrawVertexCall(mParticleCount, 0);
	
	D3DGraphicsRenderer::SetTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsManager::GetGeometryShader(eGeometryShaderType::PARTICLE)->Reset();
}

void ParticleSystem::DrawShadow(Light* _pLight)
{
}

void ParticleSystem::SetMaterial()
{
	if (mMaterial)
		mMaterial->release();

	// PxPBDMaterial 생성
	mMaterial = GameManager::GetPhysicsManager()->GetPhysics()
		->createPBDMaterial(
			mFriction,				// 마찰 계수
			mDamping,				// 감쇠 계수
			mAdhesion,				// 접착력
			mViscosity,				// 점성
			mVorticity,				// 소용돌이 강도
			mSurfaceTension,		// 표면 장력
			mCohesion,				// 결합력
			mBuoyancy,				// 상승력
			mAirResistance,			// 공기 저항
			mCflCoefficient,		// CFL 계수
			mGravity				// 중력
		);
}

void ParticleSystem::SetBuffers()
{
	PxVec4* bufferPos = mParticleBuffer->getPositionInvMasses();
	GameManager::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)bufferPos, mPositionsHost, mParticleCount * sizeof(PxVec4), 0);
	mParticleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_POSITION);


	PxVec4* bufferVel = mParticleBuffer->getVelocities();
	GameManager::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)(bufferVel), mVelocitiesHost, mParticleCount * sizeof(PxVec4), 0);
	mParticleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_VELOCITY);

	PxU32* bufferPhases = mParticleBuffer->getPhases();
	GameManager::GetPhysicsManager()->GetCudaContext()->memcpyHtoDAsync((CUdeviceptr)(bufferPhases), mPhasesHost, mParticleCount * sizeof(PxU32), 0);
	mParticleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_PHASE);

	mParticleBuffer->setNbActiveParticles(mParticleCount);
	mParticleSystem->addParticleBuffer(mParticleBuffer);


	//Create Vertex Buffer
	if (mVertexBuffer)
		SAFE_DELETE(mVertexBuffer);
	
	unsigned int stride = sizeof(PxVec4);
	unsigned int count = maxParticleCount; //TODO : 넉넉하게 max count로 하였으나 테스트 후 파티클 개수만큼으로 조정할지 판단 

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_IMMUTABLE;//IMMUTABLE : GPU can only read, CPU cannot access
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //vertex buffer 용도
	desc.ByteWidth = stride * count;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = mPositionsHost; //첫번째 데이터의 주소

	mVertexBuffer = new D3DGraphicsVertexBuffer(&desc, &data);
	mVertexBuffer->Create();

	mVertexBuffer->SetStride(sizeof(PxVec4));
	mVertexBuffer->SetOffset(0);
}

void ParticleSystem::DefaultFogMove()
{
	srand(static_cast<unsigned int>(time(0)));
	int particleCount = mParticleCount; // 연기 입자 수
	PxVec3 spawnCenter(0.0f, 10.0f, 0.0f); // 연기가 모이는 중심 위치
	float spawnRadius = 5.0f; // 초기 위치에서의 반경
	float riseSpeed = 7.0f; // 연기의 상승 속도
	float horizontalSpread = 2.0f; // 연기의 확산 정도
	float timeOffsetRange = 150.0f; // 시간 오프셋 범위

	for (int i = 0; i < particleCount; ++i)
	{
		float timeOffset = (rand() % 100 / 100.0f) * timeOffsetRange;

		// 초기 위치 설정 (spawnCenter 근처에 모임)
		mPositionsHost[i] = PxVec4(
			spawnCenter.x + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // X축 중심 근처
			spawnCenter.y + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Y축 중심 근처
			spawnCenter.z + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Z축 중심 근처
			1.0f
		);

		// 초기 속도 설정 (Y축 상승, X/Z는 약간의 확산)
		mVelocitiesHost[i] = PxVec4(
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // X축 확산
			riseSpeed + timeOffset * 0.1f,                    // Y축 상승 속도 (시간 오프셋 반영)
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // Z축 확산
			0.0f
		);
	}

	// 입자 간 거리와 속성 설정
	const PxReal particleSpacing = 0.01f;  // 입자 간 거리
	const PxReal fluidDensity = 100.f;    // 연기의 밀도
	const PxReal restOffset = 0.5f * particleSpacing / 0.6f;
	const PxReal solidRestOffset = restOffset;
	const PxReal fluidRestOffset = restOffset * 0.6f;

	mParticleSystem->setRestOffset(restOffset);
	mParticleSystem->setContactOffset(restOffset + 0.01f); // 입자 간 접촉 최소화
	mParticleSystem->setParticleContactOffset(restOffset + 0.02f); // 부드러운 접촉
	mParticleSystem->setSolidRestOffset(solidRestOffset);
	mParticleSystem->setFluidRestOffset(fluidRestOffset);
}

void ParticleSystem::FromBufferToGPU()
{
	GameManager::GetPhysicsManager()->GetCudaManager()->acquireContext();

	PxVec4* bufferPos = mParticleBuffer->getPositionInvMasses();
	GameManager::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyDtoHAsync(
		mPositionsHost, (CUdeviceptr)bufferPos, sizeof(PxVec4) * mParticleCount, 0);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = D3DGraphicsRenderer::GetDevicecontext()->Map(
		mVertexBuffer->mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
	);

	GameManager::GetPhysicsManager()->GetCudaManager()->releaseContext();

	if (SUCCEEDED(hr)) {
		memcpy(mappedResource.pData, mPositionsHost, sizeof(PxVec4) * mParticleCount);
		D3DGraphicsRenderer::GetDevicecontext()->Unmap(mVertexBuffer->mBuffer, 0);
	}
}

void ParticleSystem::FromGPUToBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = D3DGraphicsRenderer::GetDevicecontext()->Map(
		mVertexBuffer->mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
	);

	GameManager::GetPhysicsManager()->GetCudaManager()->releaseContext();

	if (SUCCEEDED(hr)) {
		memcpy(mappedResource.pData, mPositionsHost, sizeof(PxVec4) * maxParticleCount);
		D3DGraphicsRenderer::GetDevicecontext()->Unmap(mVertexBuffer->mBuffer, 0);
	}
}

