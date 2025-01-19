#pragma once
class PickingRay
{
public:
	PxRaycastBuffer Raycast();
	void SetMainCamera(Camera* _camera) { mCamera = _camera; }

private:
	Ray GeneratePickingRay(Vector2 _mousePos);

private:
	Camera* mCamera;
};
