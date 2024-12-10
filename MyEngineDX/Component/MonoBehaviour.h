#pragma once
class Collider;
class GameObject;
class WorldManager;

namespace Engine
{
	class MonoBehaviour
	{
	public:
		MonoBehaviour();
		virtual ~MonoBehaviour();
		//============MonoBehaviour메소드=============
		//================CallBack함수================
		virtual void _CALLBACK OnEnable() {};
		virtual void _CALLBACK OnDisable() {};
		virtual void _CALLBACK OnDestroy() {};

		virtual void _CALLBACK OnCollisionEnter(Collider* _origin, Collider* _destination) {};
		virtual void _CALLBACK OnCollisionStay(Collider* _origin, Collider* _destination) {};
		virtual void _CALLBACK OnCollisionExit(Collider* _origin, Collider* _destination) {};

		virtual void _CALLBACK OnMouseEnter() {};
		virtual void _CALLBACK OnMouseStay() {};
		virtual void _CALLBACK OnMouseExit() {};

		virtual void _CALLBACK OnAnimationPlay() {};
		virtual void _CALLBACK OnAnimationStop() {};
		virtual void _CALLBACK OnAnimationStart() {};
		virtual void _CALLBACK OnAnimationEnd() {};
	protected:

	};
}