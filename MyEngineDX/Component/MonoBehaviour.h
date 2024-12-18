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
		
	public: // MonoBehaviour�޼ҵ�====================
		//
		// ������Ʈ �˻�===============================
		// ���� ���带 �������� ������Ʈ�� �̸��� �±װ� �Ѵ� ���� ������Ʈ�� ��ȯ
		GameObject* FindObject(std::wstring_view _name, std::wstring_view _tag);
		// ���� ���带 �������� ������Ʈ �̸����� �˻��Ͽ� ���� ó���� ������ ������Ʈ�� ��ȯ
		GameObject* FindObjectWithName(std::wstring_view _name);
		// ���� ���带 �������� ������Ʈ�� �±׷� �˻��Ͽ� ���� ������Ʈ���� �迭�� ��ȯ
		std::vector<GameObject*> FindObjectsWithTag(std::wstring_view _tag);
		// ���� ���带 �������� ������Ʈ�� �̸����� �˻��Ͽ� ���� ������Ʈ���� �迭�� ��ȯ
		std::vector<GameObject*> FindObjectsWithName(std::wstring_view _name);
		// ===========================================
		// 
		// ������Ʈ ���� �� ����========================
		GameObject* Instantiate(Resource::Prefab* _pInstant);
		GameObject* Instantiate(Resource::Prefab* _pInstant, Vector3 _position);
		void Destroy(GameObject* _object);
		// ===========================================
		// 
		// CallBack�Լ�===============================
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