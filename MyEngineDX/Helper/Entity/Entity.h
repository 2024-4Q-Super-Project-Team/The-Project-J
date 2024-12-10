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
		virtual void _CALLBACK OnEnable() {};
		virtual void _CALLBACK OnDisable() {};
		virtual void _CALLBACK OnCreate() {};
		virtual void _CALLBACK OnDestroy() {};
	protected:
		std::wstring mName;
		std::wstring mTag;
		EntityState	 mState;
	public:
		void SetName(std::wstring_view _name) { mName = _name; }
		void SetTag(std::wstring_view _tag) { mTag = _tag; }
		void SetActive(bool _isActive);
		void SetDestroy();

		const std::wstring& GetName()	 { return mName; }
		const std::wstring& GetTag()	 { return mTag; }
		const EntityState&  GetState()	 { return mState; }
	};
}
