#include "pch.h"
#include "Collider.h"

void Collider::FixedUpdate()
{

}

void Collider::PreUpdate()
{
}

void Collider::Update()
{
}

void Collider::PostUpdate()
{
}

void Collider::PreRender()
{
}

void Collider::Render(GraphicsManager* _graphicsManager)
{
	if (mDebugMode)
	{
		Draw(_graphicsManager);
	}
}

void Collider::PostRender()
{
}
