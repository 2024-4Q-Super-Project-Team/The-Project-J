#pragma once

namespace Engine
{
	class Entity
	{
	public:
		Entity() : mName(L""), mTag(L""), mState(EntityState::Create) {}
		Entity(std::wstring_view _name, std::wstring_view _tag)
			: mName(_name), mTag(_tag), mState(EntityState::Create) {
		}
	protected:
		// �ݹ��� ȣ��޴� Ÿ�̹��� �˾ƾ��Ѵ�.
		// SetActive(true)�� ȣ�����ڸ��� ȣ��
		virtual void _CALLBACK OnEnable() {};
		// SetActive(false)�� ȣ�����ڸ��� ȣ��
		virtual void _CALLBACK OnDisable() {};
		// ��ü�� �������� ���� ȣ��
		virtual void _CALLBACK OnCreate() {};
		// Destroy���·� �ٲ��� ���� ȣ��
		virtual void _CALLBACK OnDestroy() {};
	protected:
		std::wstring mName;
		std::wstring mTag;
		EntityState	 mState;
	public:
		void SetActive(bool _isActive);
		void SetDestroy();
		void SetCreate();

		void SetName(std::wstring_view _name) { mName = _name; }
		void SetTag(std::wstring_view _tag) { mTag = _tag; }
		const std::wstring& GetName()  const	{ return mName; }
		const std::wstring& GetTag()	  const		{ return mTag; }
		const EntityState&  GetState() const		{ return mState; }
	};
}
