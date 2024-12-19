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
		// 콜백은 호출받는 타이밍을 알아야한다.
		// SetActive(true)를 호출하자마자 호출
		virtual void _CALLBACK OnEnable() {};
		// SetActive(false)를 호출하자마자 호출
		virtual void _CALLBACK OnDisable() {};
		// 객체가 생성되자 마자 호출
		virtual void _CALLBACK OnCreate() {};
		// Destroy상태로 바꾸자 마자 호출
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
