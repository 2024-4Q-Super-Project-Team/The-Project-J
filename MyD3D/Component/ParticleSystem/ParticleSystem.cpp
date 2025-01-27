#include "pch.h"
#include "ParticleSystem.h"
#include "Physics/PhysicsManager.h"
#include "World/World.h"
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

	gameObject->GetOwnerWorld()->AddPxActor(mParticleSystem);

	mParticleBuffer = GameManager::GetPhysicsManager()->GetPhysics()
		->createParticleBuffer(9000, 10, GameManager::GetPhysicsManager()->GetCudaManager());

	// PxPBDMaterial ����
	PxPBDMaterial* defaultMat = GameManager::GetPhysicsManager()->GetPhysics()
		->createPBDMaterial(
			0.1f,  // ���� ���
			0.05f, // ���� ���
			0.0f,  // ������
			0.01f, // ����
			0.9f,  // �ҿ뵹�� ����
			0.1f,  // ǥ�� ���
			0.7f,  // ���շ�
			0.5f,  // ��·�
			0.1f, // ���� ����
			1.0f,  // CFL ���
			0.0f   // �߷�
		);

	// ��ƼŬ ������ ����
	const PxU32 particlePhase = mParticleSystem->createPhase(
		defaultMat,
		PxParticlePhaseFlags(PxParticlePhaseFlag::eParticlePhaseFluid | PxParticlePhaseFlag::eParticlePhaseSelfCollide)
	);

	srand(static_cast<unsigned int>(time(0)));
	int particleCount = 9000; // ���� ���� ��
	PxVec3 spawnCenter(0.0f, 10.0f, 0.0f); // ���Ⱑ ���̴� �߽� ��ġ
	float spawnRadius = 5.0f; // �ʱ� ��ġ������ �ݰ�
	float riseSpeed = 7.0f; // ������ ��� �ӵ�
	float horizontalSpread = 2.0f; // ������ Ȯ�� ����
	float timeOffsetRange = 150.0f; // �ð� ������ ����

	for (int i = 0; i < particleCount; ++i)
	{
		float timeOffset = (rand() % 100 / 100.0f) * timeOffsetRange;

		// �ʱ� ��ġ ���� (spawnCenter ��ó�� ����)
		mPositionsHost[i] = PxVec4(
			spawnCenter.x + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // X�� �߽� ��ó
			spawnCenter.y + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Y�� �߽� ��ó
			spawnCenter.z + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Z�� �߽� ��ó
			1.0f
		);

		// �ʱ� �ӵ� ���� (Y�� ���, X/Z�� �ణ�� Ȯ��)
		mVelocitiesHost[i] = PxVec4(
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // X�� Ȯ��
			riseSpeed + timeOffset * 0.1f,                    // Y�� ��� �ӵ� (�ð� ������ �ݿ�)
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // Z�� Ȯ��
			0.0f
		);
	}

	PxVec4* bufferPos = mParticleBuffer->getPositionInvMasses();
	GameManager::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)bufferPos, mPositionsHost, 9000 * sizeof(PxVec4), 0);
	mParticleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_POSITION);


	PxVec4* bufferVel = mParticleBuffer->getVelocities();
	GameManager::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyHtoDAsync((CUdeviceptr)(bufferVel), mVelocitiesHost, 9000 * sizeof(PxVec4), 0);
	mParticleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_VELOCITY);

	PxU32* bufferPhases = mParticleBuffer->getPhases();
	GameManager::GetPhysicsManager()->GetCudaContext()->memcpyHtoDAsync((CUdeviceptr)(bufferPhases), mPhasesHost, 9000 * sizeof(PxU32), 0);
	mParticleBuffer->raiseFlags(PxParticleBufferFlag::eUPDATE_PHASE);

	mParticleBuffer->setNbActiveParticles(9000);
	mParticleSystem->addParticleBuffer(mParticleBuffer);

	// ���� �� �Ÿ��� �Ӽ� ����
	const PxReal particleSpacing = 0.01f;  // ���� �� �Ÿ�
	const PxReal fluidDensity = 100.f;    // ������ �е�
	const PxReal restOffset = 0.5f * particleSpacing / 0.6f;
	const PxReal solidRestOffset = restOffset;
	const PxReal fluidRestOffset = restOffset * 0.6f;

	mParticleSystem->setRestOffset(restOffset);
	mParticleSystem->setContactOffset(restOffset + 0.01f); // ���� �� ���� �ּ�ȭ
	mParticleSystem->setParticleContactOffset(restOffset + 0.02f); // �ε巯�� ����
	mParticleSystem->setSolidRestOffset(solidRestOffset);
	mParticleSystem->setFluidRestOffset(fluidRestOffset);


	unsigned int stride = sizeof(PxVec4);
	unsigned int count = maxParticleCount; //TODO : �˳��ϰ� max count�� �Ͽ����� �׽�Ʈ �� ��ƼŬ ������ŭ���� �������� �Ǵ� 

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //vertex buffer �뵵
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = stride * count;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = mPositionsHost; //ù��° �������� �ּ�

	mVertexBuffer = new D3DGraphicsVertexBuffer(&desc, &data);
	mVertexBuffer->Create();

	mVertexBuffer->SetStride(sizeof(PxVec4));
	mVertexBuffer->SetOffset(0);

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
	GameManager::GetPhysicsManager()->GetCudaManager()->acquireContext();

	PxVec4* bufferPos = mParticleBuffer->getPositionInvMasses();
	GameManager::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyDtoHAsync(
		mPositionsHost, (CUdeviceptr)bufferPos, sizeof(PxVec4) * 9000, 0);


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = D3DGraphicsRenderer::GetDevicecontext()->Map(
		mVertexBuffer->mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
	);

	GameManager::GetPhysicsManager()->GetCudaManager()->releaseContext();

	if (SUCCEEDED(hr)) {
		memcpy(mappedResource.pData, mPositionsHost, sizeof(PxVec4) * 9000);
		D3DGraphicsRenderer::GetDevicecontext()->Unmap(mVertexBuffer->mBuffer, 0);
	}

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

	ImGui::Text(u8"��ƼŬ ���� : ");
	ImGui::DragInt((uid + "Particle Count").c_str(), (int*)&mParticleCount, 100, 0, maxParticleCount);

	ImGui::Separator();
	ImGui::Text("Material Setting : ");
	ImGui::Separator();

	ImGui::Text(u8"���� ���:");
	ImGui::DragFloat((uid + "mFriction").c_str(), &mFriction, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"���� ���:");
	ImGui::DragFloat((uid + "mDamping").c_str(), &mDamping, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"������:");
	ImGui::DragFloat((uid + "mAdhesion").c_str(), &mAdhesion, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"����:");
	ImGui::DragFloat((uid + "mViscosity").c_str(), &mViscosity, 0.001f, 0.0f, 1.0f, "%.3f");

	ImGui::Text(u8"�ҿ뵹�� ����:");
	ImGui::DragFloat((uid + "mVorticity").c_str(), &mVorticity, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"ǥ�� ���:");
	ImGui::DragFloat((uid + "mSurfaceTension").c_str(), &mSurfaceTension, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"���շ�:");
	ImGui::DragFloat((uid + "mCohesion").c_str(), &mCohesion, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"��·�:");
	ImGui::DragFloat((uid + "mBuoyancy").c_str(), &mBuoyancy, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"���� ����:");
	ImGui::DragFloat((uid + "mAirResistance").c_str(), &mAirResistance, 0.01f, 0.0f, 1.0f, "%.2f");

	ImGui::Text(u8"CFL ���:");
	ImGui::DragFloat((uid + "mCflCoefficient").c_str(), &mCflCoefficient, 0.01f, 0.0f, 10.0f, "%.2f");

	ImGui::Text(u8"�߷�:");
	ImGui::DragFloat((uid + "mGravity").c_str(), &mGravity, 0.01f, -10.0f, 10.0f, "%.2f");
}

Vector3 ParticleSystem::GetDistanceFromCamera(Camera* _camera)
{
	return _camera->GetDistance(gameObject->transform);
}

void ParticleSystem::DrawObject(Matrix& _view, Matrix& _projection)
{
	D3DGraphicsRenderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//// ���ҽ� ���ε�
	GraphicsManager::GetVertexShader(eVertexShaderType::PARTICLE)->Bind();
	GraphicsManager::GetGeometryShader(eGeometryShaderType::PARTICLE)->Bind();
	GraphicsManager::GetPixelShader(ePixelShaderType::PARTICLE)->Bind();
	mTexture->Texture->SetBindSlot(25);
	mTexture->Texture->SetBindStage(eShaderStage::PS);
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
	
	D3DGraphicsRenderer::SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GraphicsManager::GetGeometryShader(eGeometryShaderType::PARTICLE)->Reset();
}

void ParticleSystem::DrawShadow(Light* _pLight)
{
}

void ParticleSystem::DrawWire()
{
}

void ParticleSystem::SetMaterial()
{
	if (mMaterial)
		mMaterial->release();

	// PxPBDMaterial ����
	mMaterial = GameManager::GetPhysicsManager()->GetPhysics()
		->createPBDMaterial(
			mFriction,				// ���� ���
			mDamping,				// ���� ���
			mAdhesion,				// ������
			mViscosity,				// ����
			mVorticity,				// �ҿ뵹�� ����
			mSurfaceTension,		// ǥ�� ���
			mCohesion,				// ���շ�
			mBuoyancy,				// ��·�
			mAirResistance,			// ���� ����
			mCflCoefficient,		// CFL ���
			mGravity				// �߷�
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
	unsigned int count = maxParticleCount; //TODO : �˳��ϰ� max count�� �Ͽ����� �׽�Ʈ �� ��ƼŬ ������ŭ���� �������� �Ǵ� 

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //vertex buffer �뵵
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.ByteWidth = stride * count;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = mPositionsHost; //ù��° �������� �ּ�

	mVertexBuffer = new D3DGraphicsVertexBuffer(&desc, &data);
	mVertexBuffer->Create();

	mVertexBuffer->SetStride(sizeof(PxVec4));
	mVertexBuffer->SetOffset(0);
}

void ParticleSystem::DefaultFogMove()
{
	srand(static_cast<unsigned int>(time(0)));
	int particleCount = mParticleCount; // ���� ���� ��
	PxVec3 spawnCenter(0.0f, 10.0f, 0.0f); // ���Ⱑ ���̴� �߽� ��ġ
	float spawnRadius = 5.0f; // �ʱ� ��ġ������ �ݰ�
	float riseSpeed = 7.0f; // ������ ��� �ӵ�
	float horizontalSpread = 2.0f; // ������ Ȯ�� ����
	float timeOffsetRange = 150.0f; // �ð� ������ ����

	for (int i = 0; i < particleCount; ++i)
	{
		float timeOffset = (rand() % 100 / 100.0f) * timeOffsetRange;

		// �ʱ� ��ġ ���� (spawnCenter ��ó�� ����)
		mPositionsHost[i] = PxVec4(
			spawnCenter.x + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // X�� �߽� ��ó
			spawnCenter.y + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Y�� �߽� ��ó
			spawnCenter.z + ((rand() % 100 / 100.0f - 0.5f) * spawnRadius), // Z�� �߽� ��ó
			1.0f
		);

		// �ʱ� �ӵ� ���� (Y�� ���, X/Z�� �ణ�� Ȯ��)
		mVelocitiesHost[i] = PxVec4(
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // X�� Ȯ��
			riseSpeed + timeOffset * 0.1f,                    // Y�� ��� �ӵ� (�ð� ������ �ݿ�)
			(rand() % 100 / 100.0f - 0.5f) * horizontalSpread, // Z�� Ȯ��
			0.0f
		);
	}

	// ���� �� �Ÿ��� �Ӽ� ����
	const PxReal particleSpacing = 0.01f;  // ���� �� �Ÿ�
	const PxReal fluidDensity = 100.f;    // ������ �е�
	const PxReal restOffset = 0.5f * particleSpacing / 0.6f;
	const PxReal solidRestOffset = restOffset;
	const PxReal fluidRestOffset = restOffset * 0.6f;

	mParticleSystem->setRestOffset(restOffset);
	mParticleSystem->setContactOffset(restOffset + 0.01f); // ���� �� ���� �ּ�ȭ
	mParticleSystem->setParticleContactOffset(restOffset + 0.02f); // �ε巯�� ����
	mParticleSystem->setSolidRestOffset(solidRestOffset);
	mParticleSystem->setFluidRestOffset(fluidRestOffset);
}

void ParticleSystem::FromGPUToHost()
{
	GameManager::GetPhysicsManager()->GetCudaManager()->acquireContext();

	PxVec4* bufferPos = mParticleBuffer->getPositionInvMasses();
	GameManager::GetPhysicsManager()->GetCudaManager()->getCudaContext()->memcpyDtoHAsync(
		mPositionsHost, (CUdeviceptr)bufferPos, sizeof(PxVec4) * mParticleCount, 0);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = D3DGraphicsRenderer::GetDevicecontext()->Map(
		mVertexBuffer->mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
	);
	if (SUCCEEDED(hr)) {
		memcpy(mappedResource.pData, mPositionsHost, sizeof(PxVec4) * mParticleCount);
		D3DGraphicsRenderer::GetDevicecontext()->Unmap(mVertexBuffer->mBuffer, 0);
	}

	GameManager::GetPhysicsManager()->GetCudaManager()->releaseContext();

}