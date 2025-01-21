#include "pch.h"
#include "PickingRay.h"
#include "ViewportScene/ViewportManager.h"
#include "ViewportScene/ViewportScene.h"
#include "World/World.h"

PxRaycastBuffer PickingRay::Raycast()
{
    Ray ray = GeneratePickingRay(Input::GetCurrentMousePos());

    PxRaycastBuffer hitBuffer;
    float maxDistance = 1000.f;

    PxVec3 pxRayOrigin(ray.position.x, ray.position.y, ray.position.z);
    PxVec3 pxRayDirection(ray.direction.x, ray.direction.y, ray.direction.z);

    if (GameManager::GetCurrentWorld()->GetPxScene()
        ->raycast(pxRayOrigin, pxRayDirection, maxDistance, hitBuffer))
    {
        return hitBuffer;
    }
}

Ray PickingRay::GeneratePickingRay(Vector2 _mousePos)
{
    Vector2 ndcPos;
    ViewportManager::GetActiveViewport()->GetIWindow()
        ->GetNDCCoord(_mousePos.x, _mousePos.y, ndcPos.x, ndcPos.y);

    Vector4 rayClip(ndcPos.x, ndcPos.y, -1.0f, 1.0f);

    Matrix view = mCamera->GetView();
    Matrix projection = mCamera->GetProjection();

    Matrix invProjection = XMMatrixInverse(nullptr, projection);
    Matrix invView = XMMatrixInverse(nullptr, view);
    Vector4 rayEye = XMVector4Transform(rayClip, invProjection);
    rayEye.z = 1.0f;
    rayEye.w = 0.0f;

    Vector4 rayWorld = XMVector4Transform(rayEye, invView);

    Ray ray;
    ray.position = mCamera->gameObject->transform->GetWorldPosition();
    ray.direction = XMVector3Normalize(Vector3(rayWorld.x, rayWorld.y, rayWorld.z));

    return ray;
}
