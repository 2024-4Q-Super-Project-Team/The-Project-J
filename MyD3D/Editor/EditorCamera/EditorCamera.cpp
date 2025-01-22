#include "pch.h"
#include "EditorCamera.h"
#include "Editor/EditorManager.h"
#include "ViewportScene/ViewportManager.h"
#include "ViewportScene/ViewportScene.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "Graphics/GraphicsManager.h"
#include "Component/Renderer/MeshRenderer/MeshRenderer.h"
#include "Component/Renderer/SkinnedMeshRenderer/SkinnedMeshRenderer.h"

#include "Component/Camera/SkyBox/SkyBox.h"

#define CAMERA_MOVE_SPEED (mCameraMoveSpeed * Time::GetUnScaledDeltaTime())

void EditorCamera::EditorUpdate()
{
    UpdatePosition();
    UpdateRotation();
    UpdateMatrix();
    UpdateCameraBuffer();
    UpdateCamera();
}

void EditorCamera::EditorRender()
{
    //if (!EditorManager::GetFocusViewport()) return;
    //if (!EditorManager::GetFocusViewport()->GetWorldManager()) return;
    //World* world = EditorManager::GetFocusViewport()->GetWorldManager()->GetActiveWorld();

    ExcuteDrawList();
}

void EditorCamera::UpdatePosition()
{
    if (Input::IsKeyHold(Key::LCONTROL))
    {
        // ���� ��ǥ�迡���� �̵� ���� ���� ���
        Vector3 forward = mDirection;                           // ���� ����

        // �̵� ó��
        //if (Input::IsKeyHold('A'))
        //{
        //    mPosition += -right * CAMERA_MOVE_SPEED; // ���� �̵�
        //}
        //if (Input::IsKeyHold('D'))
        //{
        //    mPosition += right * CAMERA_MOVE_SPEED;  // ������ �̵�
        //}
        if (Input::IsKeyHold('W'))
        {
            mPosition += forward * CAMERA_MOVE_SPEED; // ���� �̵�
        }
        if (Input::IsKeyHold('S'))
        {
            mPosition += -forward * CAMERA_MOVE_SPEED; // �Ĺ� �̵�
        }
    }
}

void EditorCamera::UpdateRotation()
{
    if (Input::IsKeyHold(Key::LCONTROL))
    {
        if (Input::IsMouseHold(Mouse::RIGHT) || Input::IsMouseHold(Mouse::MID))
        {
            Input::ShowMouseCursor(false);
            // ���콺 �̵��� ��������
            float deltaX = Input::GetDeltaMousePos().x * Time::GetUnScaledDeltaTime();
            float deltaY = -Input::GetDeltaMousePos().y * Time::GetUnScaledDeltaTime();

            // ī�޶� ȸ�� �ӵ��� ���� ������ ����
            deltaX *= mCameraRotateSpeed;
            deltaY *= mCameraRotateSpeed;

            // Y�� ȸ�� (Yaw)
            Matrix rotationYaw = Matrix::CreateRotationY(deltaX);
            mDirection = Vector3::TransformNormal(mDirection, rotationYaw);

            /// X�� ȸ�� (Pitch)
            Vector3 right = mUp.Cross(mDirection);
            right.Normalize();
            Matrix rotationPitch = Matrix::CreateFromAxisAngle(right, -deltaY);
            Vector3 newDirection = mDirection.TransformNormal(mDirection, rotationPitch);

            // ���� ȸ�� ���� (ī�޶� �������� �ʵ���)
            if (abs(newDirection.Dot(mUp)) < 0.98f) // Dot�� �ν��Ͻ� �޼���
            {
                mDirection = newDirection;
                mDirection.Normalize();
            }
        }
    }
    else
    {
        Input::ShowMouseCursor(true);
    }
}

void EditorCamera::UpdateMatrix()
{
    Vector3 pos = mPosition;
    Vector3 dir = mDirection;
    Vector3 at = pos + dir;

    POINT size = EditorManager::GetFocusViewport()->GetIWindow()->GetSize();

    FLOAT aspectRatio = (FLOAT)size.x / (FLOAT)size.y;

    mViewMatrix         = XMMatrixLookAtLH(pos, at, mUp);
    mProjectionMatrix   = XMMatrixPerspectiveFovLH(
        mFovAngle,
        aspectRatio,
        mProjectionNear,
        mProjectionFar);
}

void EditorCamera::UpdateCameraBuffer()
{
    mCameraCBuffer.Position.x = mPosition.x;
    mCameraCBuffer.Position.y = mPosition.y;
    mCameraCBuffer.Position.z = mPosition.z;
    mCameraCBuffer.Position.w = 1.0f;
    mCameraCBuffer.View = XMMatrixTranspose(mViewMatrix);
    mCameraCBuffer.Projection = XMMatrixTranspose(mProjectionMatrix);
    mCameraCBuffer.InverseView = DirectX::XMMatrixInverse(nullptr, mViewMatrix);
    mCameraCBuffer.InverseView = XMMatrixTranspose(mCameraCBuffer.InverseView);
    mCameraCBuffer.InverseProjection = DirectX::XMMatrixInverse(nullptr, mProjectionMatrix);
    mCameraCBuffer.InverseProjection = XMMatrixTranspose(mCameraCBuffer.InverseProjection);
    // ī�޶� ������� ���ε�
    GraphicsManager::GetConstantBuffer(eCBufferType::Camera)->UpdateGPUResoure(&mCameraCBuffer);
}

void EditorCamera::UpdateCamera()
{
    mDrawedMeshCount = 0;
    mDrawedLightCount = 0;
    auto size = EditorManager::mFocusViewport->GetIWindow()->GetSize();
    mSize = Vector2((FLOAT)size.x, (FLOAT)size.y);
    if (mMainViewport == nullptr)
    {
        mMainViewport = EditorManager::GetFocusViewport()->GetMainViewport();
    }
    if (mMainRenderTarget == nullptr)
    {
        mMainRenderTarget = EditorManager::GetFocusViewport()->GetMainRenderTarget();
    }
}

void EditorCamera::PushDrawList(IRenderContext* _renderContext)
{
    if (_renderContext == nullptr) return;
    eBlendModeType blendMode = _renderContext->GetBlendMode();
    mDrawQueue[static_cast<UINT>(blendMode)].push_back(_renderContext);
}

void EditorCamera::PushLightList(Light* _lightComponent)
{
    if (_lightComponent == nullptr) return;
    if (mLightCBuffer.NumLight < MAX_LIGHT_COUNT)
    {
        mLightCBuffer.LightProp[mLightCBuffer.NumLight] = _lightComponent->GetProperty();
        mLightCBuffer.NumLight++;
        mSceneLights.push_back(_lightComponent);
    }
}

void EditorCamera::ExcuteDrawList()
{
    GraphicsManager::GetConstantBuffer(eCBufferType::Light)->UpdateGPUResoure(&mLightCBuffer);
    if (mMainViewport && mMainRenderTarget)
    {
        if (mSize.Length() > 0.05f)
        {
            mMainRenderTarget->BeginDraw();
            mMainRenderTarget->Clear();

            DrawShadow();

            mMainViewport->Bind();

            DrawObject();

            if (mIsSkyBoxRendering)
            {
                SkyBox::GetDefaultSkyBox()->Draw(mViewMatrix, mProjectionMatrix, mProjectionFar);
            }

            mMainRenderTarget->EndDraw();


            DrawSwapChain();
        }
    }
    // ���� ����Ʈ�� �ʱ�ȭ�Ѵ�.
    mSceneLights.clear();
    mLightCBuffer.NumLight = 0;
}

void EditorCamera::DrawShadow()
{
    GraphicsManager::GetVertexShader(eVertexShaderType::SHADOW)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::G_BUFFER)->Reset();

    for (int i = 0; i < mSceneLights.size(); i++)
    {
        if (mSceneLights[i]->GetProperty().UseShadow)
        {
            auto rt = mSceneLights[i]->GetShadowRenderTarget();
            auto ShaderSRV = rt->GetSRV(rt->GetDSV());
            ShaderSRV->Reset();
            ShaderSRV->SetBindSlot(24 + i);
            // �ش� ����Ʈ�� ���� ��, ����Ʈ ���� ���ε����ش�.
            mSceneLights[i]->GetShadowRenderTarget()->BeginDraw();
            mSceneLights[i]->GetShadowRenderTarget()->Clear();
            // �� ������Ʈ�� ���� ���� ���� Draw�� �����Ѵ�
            for (auto& drawQueue : mDrawQueue)
            {
                for (auto& drawInfo : drawQueue)
                {
                    drawInfo->DrawShadow(mSceneLights[i]);
                    ++mDrawedLightCount;
                }
            }
            mSceneLights[i]->GetShadowRenderTarget()->EndDraw();
            ShaderSRV->Bind();
        }
    }
}

void EditorCamera::DrawObject()
{
    GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::FOWARD_PBR)->Bind();

    ////////////////////////////////////////////////////////////////
    // OpaQue Blend
    ////////////////////////////////////////////////////////////////
    GraphicsManager::GetBlendState(eBlendStateType::DEFAULT)->Bind();
    for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::OPAQUE_BLEND])
    {
        drawInfo->DrawObject(mViewMatrix, mProjectionMatrix);
    }
    mDrawQueue[(UINT)eBlendModeType::OPAQUE_BLEND].clear();
    ////////////////////////////////////////////////////////////////
    // Transparent Blend
    ////////////////////////////////////////////////////////////////
    GraphicsManager::GetBlendState(eBlendStateType::ALPHA)->Bind();
    for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::TRANSPARENT_BLEND])
    {
        drawInfo->DrawObject(mViewMatrix, mProjectionMatrix);
    }
    mDrawQueue[(UINT)eBlendModeType::TRANSPARENT_BLEND].clear();

    GraphicsManager::GetBlendState(eBlendStateType::ALPHA)->Reset();
}

void EditorCamera::DrawSwapChain()
{
    // QuadFrame Pass
    GraphicsManager::GetVertexShader(eVertexShaderType::SPRITE)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::SPRITE)->Bind();
    D3DGraphicsDefault::GetQuadFrameVertexBuffer()->Bind();
    D3DGraphicsDefault::GetQuadFrameIndexBuffer()->Bind();

    mMainRenderTarget->BindAllSRV();

    D3DGraphicsRenderer::DrawCall(6, 0, 0);

    mMainRenderTarget->ResetAllSRV();
}

void EditorCamera::EditorRendering(EditorViewerType _viewerType)
{
    std::string uid = "##" + std::to_string(reinterpret_cast<uintptr_t>(this));

    {
        ImGui::Text("Position : ");
        ImGui::DragFloat3((uid + "Position").c_str(), &mPosition.x, 0.1f);
    }
    {
        ImGui::Text("Direction : ");
        ImGui::DragFloat3((uid + "Direction").c_str(), &mDirection.x, 0.05f);
        mDirection.Normalize();
    }
    {
        ImGui::Text("Up : ");
        ImGui::DragFloat3((uid + "Up").c_str(), &mUp.x, 0.05f);
        mDirection.Normalize();
    }

    ImGui::Text("fovAngle : ");
    ImGui::SliderFloat((uid + "fovAngle").c_str(), mFovAngle, 1.0f, Degree::MaxDegree);
    ImGui::Text("Near : ");
    ImGui::SliderFloat((uid + "Near").c_str(), &mProjectionNear, 0.1f, 1000.0f);
    ImGui::Text("Far : ");
    ImGui::SliderFloat((uid + "Far").c_str(), &mProjectionFar, 1.0f, 100000.0f);
    ImGui::NewLine();
    ImGui::Text(("DrawedMeshCount : " + std::to_string(mDrawedMeshCount)).c_str());
    ImGui::Text(("DrawedLightCount : " + std::to_string(mDrawedLightCount)).c_str());
    ImGui::NewLine();
    ImGui::Checkbox(("Rendering SkyBox" + uid).c_str(), &mIsSkyBoxRendering);
}

