#include "pch.h"
#include "ColliderManager.h"
#include "Component\Collider\Collider.h"
#include "Component\Component.h"
#include "Component\Collider\BoxCollider.h"
#include "Component\Collider\SphereCollider.h"
#include "Component\Collider\CapsuleCollider.h"
#include "Object/Object.h"
#include "Component/MonoBehaviour.h"

void ColliderManager::Update()
{
	CheckCollision();
}


void ColliderManager::SetLayerTable(int _layer1, int _layer2, bool _bCollide)
{
	if (_layer1 < 0 || _layer1 >= LAYER_MAX_SIZE) return;
	if (_layer2 < 0 || _layer2 >= LAYER_MAX_SIZE) return;

	mLayerTable[_layer1][_layer2] = _bCollide;
	mLayerTable[_layer2][_layer1] = _bCollide;
}

void ColliderManager::RemoveColliders(GameObject* _object)
{
	std::vector<Collider*> colliders = _object->GetComponents<Collider>();

	for (Collider* collider : colliders)
	{
		for (auto it = mColliders.begin(); it != mColliders.end();)
		{
			if (*it != collider) 
				++it;

			it = mColliders.erase(it);
		}
	}

}

void ColliderManager::CheckCollision()
{
	for (int i = 0; i < mColliders.size(); i++)
	{
		for (int j = i + 1; j < mColliders.size(); j++)
		{
			if (i == j) continue; //같은 콜라이더는 검사하지 않음 

			Collider* c1 = mColliders[i];
			Collider* c2 = mColliders[j];

			if (c1 == nullptr || c2 == nullptr) continue;

			if (CheckLayer(c1, c2) == false) continue; //충돌하지 않는 레이어끼리는 검사하지 않음 

			if (TestCollisionByType(c1, c2))//충돌검사 
			{
				CallWhenCollisionFuncions(c1, c2); //오브젝트 충돌시 실행될 함수 호출
			}
			else
			{
				CallOnCollisionExitFuncion(c1, c2); //충돌 안했을 때
			}
		}
	}
}

bool ColliderManager::TestCollisionByType(Collider* _c1, Collider* _c2)
{
	bool isColliding = false;

	COLLIDER_TYPE type1 = _c1->GetType();
	COLLIDER_TYPE type2 = _c2->GetType();

	switch (type1)
	{
	COLLIDER_TYPE_BOX:
		BoxCollider* box1 = dynamic_cast<BoxCollider*>(_c1);
		if (type2 == COLLIDER_TYPE_BOX)
		{
			BoxCollider* box2 = dynamic_cast<BoxCollider*>(_c2);
			isColliding = CheckCollisionBoxToBox(box1, box2);
			break;
		}
		else if (type2 == COLLIDER_TYPE_SPHERE)
		{
			SphereCollider* sphere = dynamic_cast<SphereCollider*>(_c2);
			isColliding = CheckCollisionBoxToSphere(box1, sphere);
			break;
		}
		else if (type2 == COLLIDER_TYPE_CAPSULE)
		{
			CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(_c2);
			isColliding = CheckCollisionBoxToCapsule(box1, capsule);
			break;
		}
	COLLIDER_TYPE_SPHERE:
		SphereCollider* sphere1 = dynamic_cast<SphereCollider*>(_c1);
		if (type2 == COLLIDER_TYPE_BOX)
		{
			BoxCollider* box = dynamic_cast<BoxCollider*>(_c2);
			isColliding = CheckCollisionBoxToSphere(box, sphere1);
			break;
		}
		else if (type2 == COLLIDER_TYPE_SPHERE)
		{
			SphereCollider* sphere2 = dynamic_cast<SphereCollider*>(_c2);
			isColliding = CheckCollisionSphereToSphere(sphere1, sphere2);
			break;
		}
		else if (type2 == COLLIDER_TYPE_CAPSULE)
		{
			CapsuleCollider* capsule = dynamic_cast<CapsuleCollider*>(_c2);
			isColliding = CheckCollisionSphereToCapsule(sphere1, capsule);
			break;
		}
	COLLIDER_TYPE_CAPSULE:
		CapsuleCollider* capsule1 = dynamic_cast<CapsuleCollider*>(_c1);
		if (type2 == COLLIDER_TYPE_BOX)
		{
			BoxCollider* box = dynamic_cast<BoxCollider*>(_c2);
			isColliding = CheckCollisionBoxToCapsule(box, capsule1);
			break;
		}
		else if (type2 == COLLIDER_TYPE_SPHERE)
		{
			SphereCollider* sphere = dynamic_cast<SphereCollider*>(_c2);
			isColliding = CheckCollisionSphereToCapsule(sphere, capsule1);
			break;
		}
		else if (type2 == COLLIDER_TYPE_CAPSULE)
		{
			CapsuleCollider* capsule2 = dynamic_cast<CapsuleCollider*>(_c2);
			isColliding = CheckCollisionCapsuleToCapsule(capsule1, capsule2);
			break;
		}
	}

}

bool ColliderManager::CheckLayer(Collider* _c1, Collider* _c2)
{
	int layer1 = _c1->GetLayer();
	int layer2 = _c1->GetLayer();

	return mLayerTable[layer1][layer2];
}

void ColliderManager::CallWhenCollisionFuncions(Collider* _c1, Collider* _c2)
{
	{
		GameObject* object = _c1->ownerObject;
		std::vector<Engine::MonoBehaviour*> monoBehaviour = object->GetComponents<Engine::MonoBehaviour>();
		
		for (auto mb : monoBehaviour)
		{
			bool isColliding = _c1->GetIsColliding();
			if (!isColliding) //처음 충돌한 상황
			{
				_c1->SetIsColliding(true);
				mb->OnCollisionEnter(_c1, _c2);
			}
			else //충돌 중이던 상황
			{
				mb->OnCollisionStay(_c1, _c2);
			}
		}
	}
	
	{
		GameObject* object = _c2->ownerObject;
		std::vector<Engine::MonoBehaviour*> monoBehaviour = object->GetComponents<Engine::MonoBehaviour>();

		for (auto mb : monoBehaviour)
		{
			bool isColliding = _c2->GetIsColliding();
			if (!isColliding) //처음 충돌한 상황
			{
				_c2->SetIsColliding(true);
				mb->OnCollisionEnter(_c2, _c1);
			}
			else //충돌 중이던 상황
			{
				mb->OnCollisionStay(_c2, _c1);
			}
		}
	}

}

void ColliderManager::CallOnCollisionExitFuncion(Collider* _c1, Collider* _c2)
{
	{
		GameObject* object = _c1->ownerObject;
		std::vector<Engine::MonoBehaviour*> monoBehaviour = object->GetComponents<Engine::MonoBehaviour>();
		for (auto mb : monoBehaviour)
		{
			if (_c1->GetIsColliding())
			{
				_c1->SetIsColliding(false);
				mb->OnCollisionExit(_c1, _c2);
			}
		}
	}

	{
		GameObject* object = _c2->ownerObject;
		std::vector<Engine::MonoBehaviour*> monoBehaviour = object->GetComponents<Engine::MonoBehaviour>();
		for (auto mb : monoBehaviour)
		{
			if (_c2->GetIsColliding())
			{
				_c2->SetIsColliding(false);
				mb->OnCollisionExit(_c2, _c1);
			}
		}
	}
}
