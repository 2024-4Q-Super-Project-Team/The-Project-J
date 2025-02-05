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
#include "EditorGridDrawer.h"

EditorCamera::EditorCamera()
{
}

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
    if (GameManager::GetRunType() == eEngineRunType::EDITOR_MODE)
    {
        ExcuteDrawList();
    }
}

#define CAMERA_MOVE_ACCELERATION (mCameraMoveSpeed * Time::GetUnScaledDeltaTime())
void EditorCamera::UpdatePosition()
{
    if (Input::IsKeyHold(Key::LCONTROL))
    {
        if (Input::IsKeyHold('W'))
        {
            mPosition += mRotation.Forward() * CAMERA_MOVE_ACCELERATION;
        }
        if (Input::IsKeyHold('S'))
        {
            mPosition -= mRotation.Forward() * CAMERA_MOVE_ACCELERATION;
        }
        if (Input::IsKeyHold('A'))
        {
            mPosition += mRotation.Right() * CAMERA_MOVE_ACCELERATION;
        }
        if (Input::IsKeyHold('D'))
        {
            mPosition -= mRotation.Right() * CAMERA_MOVE_ACCELERATION;
        }
        if (Input::IsKeyHold('Q'))
        {
            mPosition += mRotation.Up() * CAMERA_MOVE_ACCELERATION;
        }
        if (Input::IsKeyHold('E'))
        {
            mPosition -= mRotation.Up() * CAMERA_MOVE_ACCELERATION;
        }
        if (Input::GetWheelDeltaPos() != 0)
        {
            mPosition += mRotation.Forward() * Input::GetWheelDeltaPos() * CAMERA_MOVE_ACCELERATION;
        }
    }
}

void EditorCamera::UpdateRotation()
{
    if (Input::IsKeyHold(Key::LCONTROL) && (Input::IsMouseHold(Mouse::RIGHT) || Input::IsMouseHold(Mouse::MID)))
    {
        Input::ShowMouseCursor(false);
        // 마우스 이동량 가져오기
        float deltaX = Input::GetDeltaMousePos().x * mCameraRotateSpeed * Time::GetUnScaledDeltaTime();
        float deltaY = -Input::GetDeltaMousePos().y * mCameraRotateSpeed * Time::GetUnScaledDeltaTime();

        mDirection.x += deltaX;
        mDirection.y += deltaY;
        mDirection.y = Clamp(mDirection.y, -mCameraMaxAngle, mCameraMaxAngle);
    }
    else
    {
        Input::ShowMouseCursor(true);
    }
    mRotation = XMMatrixRotationRollPitchYaw(
        mDirection.y,
        mDirection.x,
        0.0f
    );
}

void EditorCamera::UpdateMatrix()
{
    Vector3 pos = mPosition;
    Vector3 dir = mRotation.Forward();
    Vector3 at = pos + dir;

    POINT size = EditorManager::GetFocusViewport()->GetIWindow()->GetSize();

    FLOAT aspectRatio = (FLOAT)size.x / (FLOAT)size.y;

    mViewMatrix         = XMMatrixLookAtLH(pos, at, Vector3::Up);
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
    // 카메라 상수버퍼 바인딩
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

void EditorCamera::PushWireList(IRenderContext* _renderContext)
{
    if (_renderContext == nullptr) return;
    eBlendModeType blendMode = eBlendModeType::WIREFRAME_BELND;
    mDrawQueue[static_cast<UINT>(blendMode)].push_back(_renderContext);
}

void EditorCamera::PushOutlineDrawList(IRenderContext* _renderContext)
{
    if (_renderContext == nullptr) return;
    eBlendModeType blendMode = eBlendModeType::OUTLINE_BLEND;
    mDrawQueue[static_cast<UINT>(blendMode)].push_back(_renderContext);
}

void EditorCamera::ExcuteDrawList()
{
    GraphicsManager::GetConstantBuffer(eCBufferType::Light)->UpdateGPUResoure(&mLightCBuffer);
    if (mMainViewport && mMainRenderTarget)
    {
        if (mSize.Length() > 0.05f)
        {
            mMainRenderTarget->BeginDraw();
            mMainRenderTarget->Clear(ColorF::Gray());

            DrawShadow();

            mMainViewport->Bind();

            EditorGridDrawer::DrawGrid();
            DrawObject();

            GraphicsManager::GetRasterizerState(eRasterizerStateType::BACKFACE_CULLING)->Bind();
            if (mIsSkyBoxRendering)
            {
                SkyBox::GetDefaultSkyBox()->Draw(mViewMatrix, mProjectionMatrix, mProjectionFar);
            }

            mMainRenderTarget->EndDraw();

            DrawSwapChain();

            DrawWire();
        }
    }
    // 조명 리스트를 초기화한다.
    mSceneLights.clear();
    mLightCBuffer.NumLight = 0;
}

void EditorCamera::ClearDrawList()
{
    for (auto& drawInfo : mDrawQueue)
    {
        mDrawQueue->clear();
    }
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
            if (rt && rt->GetDSV())
            {
                auto ShaderSRV = rt->GetSRV(rt->GetDSV());
                ShaderSRV->Reset();
                ShaderSRV->SetBindSlot(24 + i);
                // 해당 라이트의 뎁스 뷰, 뷰포트 등을 바인드해준다.
                mSceneLights[i]->GetShadowRenderTarget()->BeginDraw();
                mSceneLights[i]->GetShadowRenderTarget()->Clear();
                mSceneLights[i]->GetShadowViewport()->Bind();
                // 각 오브젝트에 대한 깊이 버퍼 Draw를 수행한다
                for (auto& drawQueue : mDrawQueue)
                {
                    for (auto& drawInfo : drawQueue)
                    {
                        drawInfo->DrawShadow(mSceneLights[i]);
                    }
                }
                mSceneLights[i]->GetShadowRenderTarget()->EndDraw();
                ShaderSRV->Bind();
            }
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

void EditorCamera::DrawWire()
{
    DebugRenderer::BeginDraw();
    DebugRenderer::UpdateViewProjection(mViewMatrix, mProjectionMatrix);

    for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::WIREFRAME_BELND])
    {
        drawInfo->DrawWire();
    }
    mDrawQueue[(UINT)eBlendModeType::WIREFRAME_BELND].clear();

    DebugRenderer::EndDraw();
}

void EditorCamera::DrawOutlineList()
{
    GraphicsManager::GetBlendState(eBlendStateType::DEFAULT)->Bind();
    GraphicsManager::GetRasterizerState(eRasterizerStateType::NONE_CULLING)->Bind();
    GraphicsManager::GetSamplerState(eSamplerStateType::LINEAR_WRAP)->Bind();
    GraphicsManager::GetVertexShader(eVertexShaderType::STANDARD)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::FOWARD_PBR)->Bind();
    // 이거 블렌드 스테이트 바꿔야되나? 굳이? 일단 해봅세
    GraphicsManager::GetDepthStencilState(eDepthStencilStateType::STENCIL_WRITE)->Bind();
    mMainRenderTarget->ClearStencil();
    for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::OUTLINE_BLEND])
    {
        drawInfo->DrawObject(mViewMatrix, mProjectionMatrix);
    }
    GraphicsManager::GetDepthStencilState(eDepthStencilStateType::STENCIL_READ)->Bind();

    GraphicsManager::GetVertexShader(eVertexShaderType::OUTLINE)->Bind();
    GraphicsManager::GetPixelShader(ePixelShaderType::OUTLINE)->Bind();

    for (auto& drawInfo : mDrawQueue[(UINT)eBlendModeType::OUTLINE_BLEND])
    {
        drawInfo->DrawObject(mViewMatrix, mProjectionMatrix);
    }
    mDrawQueue[(UINT)eBlendModeType::OUTLINE_BLEND].clear();

    GraphicsManager::GetDepthStencilState(eDepthStencilStateType::DEFAULT)->Bind();
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
    }
    ImGui::Separator();
    {
        ImGui::Text("fovAngle : ");
        ImGui::SliderFloat((uid + "fovAngle").c_str(), mFovAngle, 1.0f, Degree::MaxDegree);
        ImGui::Text("Near : ");
        ImGui::SliderFloat((uid + "Near").c_str(), &mProjectionNear, 0.1f, 1000.0f);
        ImGui::Text("Far : ");
        ImGui::SliderFloat((uid + "Far").c_str(), &mProjectionFar, 1.0f, 100000.0f);
        ImGui::NewLine();
        ImGui::Text(("DrawedMeshCount : " + std::to_string(mDrawedMeshCount)).c_str());
        ImGui::Text(("DrawedLightCount : " + std::to_string(mDrawedLightCount)).c_str());
    }
    ImGui::Separator();
    ImGui::Text("Camera MoveSpeed : ");
    ImGui::SliderFloat((uid + "MoveSpeed").c_str(), &mCameraMoveSpeed, 1.0f, 10000.0f);
    ImGui::Text("Camera RotateSpeed : ");
    ImGui::SliderFloat((uid + "RotateSpeed").c_str(), &mCameraRotateSpeed, 1.0f, 20.0f);
    ImGui::Separator();
    ImGui::Checkbox(("Rendering SkyBox" + uid).c_str(), &mIsSkyBoxRendering);
    ImGui::Checkbox(("Rendering Collider" + uid).c_str(), &mIsColliderRendering);
    ImGui::Separator();
    //ImGui::SliderFloat((uid + "GridSize").c_str(), &Editor::GuizmoGridDrawer::mGridSize, 1.0f, 100000.0f);
    ImGui::Separator();
}

