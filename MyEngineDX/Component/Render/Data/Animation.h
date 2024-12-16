#pragma once

namespace Component
{
	class SkinningMeshRenderer;
}

namespace Graphics
{
	class ChannelInfo;

	// 하나의 모션을 가지는 애니메이션 구조체
	class AnimationInfo : public IGraphicsResource
	{
	public:
		explicit AnimationInfo(std::wstring_view _name);
		virtual ~AnimationInfo();
	public:
		inline void  SetFramePerSecond(float _fps) { mFramePerSecond = _fps; }
		inline float GetTotalFrame() { return mTotalFrame; }
		inline float GetFramePerSecond() { return mFramePerSecond; }	
		inline float GetAnimationTotalTime() { return mTotalFrame / mFramePerSecond; }
		ChannelInfo* GetChannel(std::wstring_view _key);
	private:
		float mTotalFrame;
		float mFramePerSecond;
		std::unordered_map<std::wstring, ChannelInfo> mChannels;
		friend class ResourceManager;
	};
	// 애니메이션을 구성하는 각 노드의 정보들
	class ChannelInfo : public IGraphicsResource
	{
	public:
		template <typename TYPE>
		class KeyInfo
		{
		public:
			KeyInfo(TYPE _val, float _time)
				: Value(_val), Time(_time) {}
			~KeyInfo() = default;
			TYPE		Value;
			float		Time;
		};
	public:
		explicit ChannelInfo(std::weak_ptr<AnimationInfo> _wpAnimInfo, std::wstring_view _name);
		virtual ~ChannelInfo();
	public:
		std::vector<KeyInfo<Vector3>>		mPositionKeys;
		std::vector<KeyInfo<Quaternion>>	mRotationKeys;
		std::vector<KeyInfo<Vector3>>		mScalingKeys;
	private:	
		std::weak_ptr<AnimationInfo> mOwner;		// 속한 애니메이션 정보
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
		inline void SetAnimation(AnimationInfo* _pAnim);
		inline void SetTimer(float _time);
	private:
		AnimationInfo*	mCurrAnimationInfo;
		float			mTimer = 0.0f;
	public:
		inline float			GetNormalizedTime();
		inline AnimationInfo*	GetCurrentAnimationInfo() { return mCurrAnimationInfo; }
		inline const Matrix&	GetAnimationMatrix();
		inline int				GetCurrentAnimationFrame();
		inline float			GetTimer();
	};
}

