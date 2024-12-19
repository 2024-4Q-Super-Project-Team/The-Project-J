#pragma once

class GraphicsManager;

namespace Engine
{
	class IEngineCycle
	{
		virtual void Tick() = 0;
		virtual void FixedUpdate() = 0;
		virtual void PreUpdate() = 0;
		virtual void Update() = 0;
		virtual void PostUpdate() = 0;
		virtual void PreRender() = 0;
		virtual void Render(GraphicsManager* _graphicsManager) = 0;
		virtual void PostRender() = 0;
	};

	// 호출 순서: Tick->FixedUpdate->PreUpdate->Update->PostUpdate->PreRender->Render->PostRender->
	// Tick은 FixedUpdate보다도 먼저 호출을 보장함.
}