#pragma once
#include "RendererComponent.h"

class GameObject;
class GraphicsManager;

namespace Component
{
	class SkinningMeshRenderer
		: public RendererBase
	{
		class Node;
	public:
		explicit SkinningMeshRenderer(GameObject* _owner);
		virtual ~SkinningMeshRenderer();
	public:
		virtual void FixedUpdate() override {}
		virtual void PreUpdate() override {}
		virtual void Update() override {}
		virtual void PostUpdate() override {}
		virtual void PreRender() override {}
		virtual void Render(GraphicsManager* _graphicsManager) override;
		virtual void PostRender() override {}
	public:
		bool SetModel(std::shared_ptr<Graphics::ModelInfo> _spModel);
		bool SetAnimation(const std::wstring& _key);
		inline Graphics::AnimationState* GetAnimationState() { return mAnimationState; }
		inline Graphics::BoneState* GetBoneState() { return mBoneState; }
	private:
		Node* mRootNode;
		// 렌더러가 렌더링 중인 대상 모델
		std::shared_ptr<Graphics::ModelInfo>						mModel;
		// 렌더러가 모델 렌더링에 사용하는 머티리얼, 메쉬
		std::unordered_map<std::wstring, Graphics::MaterialState*>	mMaterials;
		std::unordered_map<std::wstring, Graphics::MeshState*>		mMeshs;
		// 렌더러가 모델의 현재 재생중인 애니메이션 상태정보
		Graphics::AnimationState*	 mAnimationState;
		Graphics::BoneState*		 mBoneState;

		Node* CreateNodeFromModelNode(Node* _parent, Graphics::ModelNodeInfo* _modelNode);
		void CalculateAnimation();
		void CalculateNodeTransform(Node* _parent);

		friend class Node;
	};

	class SkinningMeshRenderer::Node
	{
	public:
		explicit Node(std::wstring_view _name, Node* _parent, SkinningMeshRenderer* _owner);
		virtual ~Node();
	public:
		void Calculate();
		void Render(GraphicsManager* _graphicsManager);
	public:
		std::wstring	    mName;
		Node*				mParent;
		std::vector<Node*>	mChildren = {};
		Matrix mLocalMatrix = Matrix::Identity;
		Matrix mWorldMatrix = Matrix::Identity;
		// 노드의 메쉬, 머티리얼 상태. 메쉬가 머티리얼 정보 소유중
		std::vector<Graphics::MeshState*> mMeshStates;
		SkinningMeshRenderer* const mOwner;
		void CalculateNodeTransform();
		void CalculateNodeAnimation();
	private:
		template <typename T>
		void CalculateAnimationKey(float _time
			, std::vector<::Graphics::ChannelInfo::KeyInfo<T>>& _keys
			, T& _value)
		{
			UINT CurrIndex = 0;
			UINT NextIndex = 1;
			// 
			while (_time > _keys[NextIndex].Time)
			{
				CurrIndex++;
				NextIndex++;
				// 다음 인덱스가 keys의 최대 인덱스를 넘어서면 다음 인덱스는 0인거다. 
				// 근데 넘을 일이 있음? 흠
				// ㅇㅅㅇ.
				if (NextIndex >= _keys.size())
				{
					NextIndex = 0;
					break;
				}
			}
			float Ratio = (_time - _keys[CurrIndex].Time) / (_keys[NextIndex].Time - _keys[CurrIndex].Time);
			_value = T::Lerp(_keys[CurrIndex].Value, _keys[NextIndex].Value, Ratio);
		}
	};
}

