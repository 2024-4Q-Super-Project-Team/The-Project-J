#pragma once

namespace Component
{
	class SkinningMeshRenderer;
}

namespace Graphics
{
	class ChannelResource;

	// �ϳ��� ����� ������ �ִϸ��̼� ����ü
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
	// �ִϸ��̼��� �����ϴ� �� ����� ������
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
		std::weak_ptr<AnimationResource> mOwner;		// ���� �ִϸ��̼� ����
		friend class ResourceManager;
	};

	// ���������� ���� Ŭ������, �ִϸ��̼��� ���¸� ������ ����(���� �ִϸ��̼� ����, ������ ī��Ʈ ��)
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

