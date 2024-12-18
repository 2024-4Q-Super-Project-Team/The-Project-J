#pragma once
#include "Component/Component.h"
#include "Data/Mesh.h"
#include "Data/Material.h"
#include "Data/Bone.h"
#include "Data/Animation.h"
#include "Data/Model.h"

class GameObject;
class GraphicsManager;

namespace Component
{
	class RendererBase
		: public ComponentBase
	{
	public:
	public:
		explicit RendererBase(GameObject* _owner);
		virtual ~RendererBase();
	public:
		virtual void Tick() override {}
		virtual void FixedUpdate() override {}
		virtual void PreUpdate() override {}
		virtual void Update() override {}
		virtual void PostUpdate() override {}
		virtual void PreRender() override {}
		virtual void Render(GraphicsManager* _graphicsManager) override {}
		virtual void PostRender() override {}
	private:

	};
}

