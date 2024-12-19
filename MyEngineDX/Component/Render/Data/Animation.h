#pragma once

namespace Component
{
	class SkinningMeshRenderer;
}

namespace Graphics
{
	class ChannelResource;

	// 하나의 모션을 가지는 애니메이션 구조체
	class AnimationResource : public IGraphicsResource
	{
	public:
		explicit AnimationResource(std::wstring_view _name);
		virtual ~AnimationResource();
	public:
		void  SetFramePerSecond(float _fps) { mFramePerSecond = _fps; }
		float GetTotalFrame() { return mTotalFrame; }
		float GetFramePerSecond() { return mFramePerSecond; }	
		float GetAnimationTotalTime() { return mTotalFrame / mFramePerSecond; }
		ChannelResource* GetChannel(std::wstring_view _key);
	private:
		float mTotalFrame;
		float mFramePerSecond;
		std::unordered_map<std::wstring, ChannelResource> mChannels;
		friend class ResourceManager;
	};
	// 애니메이션을 구성하는 각 노드의 정보들
	class ChannelResource : public IGraphicsResource
	{
	public:
		template <typename TYPE>
		class KeyResource
		{
		public:
			KeyResource(TYPE _val, float _time)
				: Value(_val), Time(_time) {}
			~KeyResource() = default;
			TYPE		Value;
			float		Time;
		};
	public:
		explicit ChannelResource(std::weak_ptr<AnimationResource> _wpAnimResource, std::wstring_view _name);
		virtual ~ChannelResource();
	public:
		std::vector<KeyResource<Vector3>>		mPositionKeys;
		std::vector<KeyResource<Quaternion>>	mRotationKeys;
		std::vector<KeyResource<Vector3>>		mScalingKeys;
	private:	
		std::weak_ptr<AnimationResource> mOwner;		// 속한 애니메이션 정보
		friend class ResourceManager;
	};

	// 렌더러에서 쓰는 클래스로, 애니메이션의 상태를 가지고 있음(현재 애니메이션 정보, 프레임 카운트 등)
	class AnimationState
	{
	public:
		explicit AnimationState();
		virtual ~AnimationState();
	public:
		void UpdateState();
	public:
		void SetAnimation(AnimationResource* _pAnim);
		void SetTimer(float _time);
	private:
		AnimationResource*	mCurrAnimationResource;
		float			mTimer = 0.0f;
	public:
		float			GetNormalizedTime();
		AnimationResource*	GetCurrentAnimationResource() { return mCurrAnimationResource; }
		const Matrix&	GetAnimationMatrix();
		int				GetCurrentAnimationFrame();
		float			GetTimer();
	};
}

