#pragma once

class GameObject;

namespace Component
{
	enum class ComponentType
	{
		Script,
		FiniteStateMachine,
		Transform,
		Camera,
		DirectionLight,
		Renderer,
		SIZE,
	};

	class ComponentBase
		: public Engine::IEngineCycle 
	{
	public:
		ComponentBase(GameObject* _owner);
		virtual ~ComponentBase() {}
	public:
		virtual void FixedUpdate() override = 0;
		virtual void PreUpdate() override = 0;
		virtual void Update() override = 0;
		virtual void PostUpdate() override = 0;
		virtual void PreRender() override = 0;
		virtual void Render(GraphicsManager* _graphicsManager) override = 0;
		virtual void PostRender() override = 0;
		virtual void Start() {}
	public:
		GameObject* const ownerObject;
	protected:
		bool		  isActive;
		ComponentType mType;
	public:
		ComponentType GetType() { return mType; }
		void SetActive(bool _active) { isActive = _active; }
		bool IsActive() { return isActive; }
	};
}
