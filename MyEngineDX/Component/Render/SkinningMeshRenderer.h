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
		// �������� ������ ���� ��� ��
		std::shared_ptr<Graphics::ModelInfo>						mModel;
		// �������� �� �������� ����ϴ� ��Ƽ����, �޽�
		std::unordered_map<std::wstring, Graphics::MaterialState*>	mMaterials;
		std::unordered_map<std::wstring, Graphics::MeshState*>		mMeshs;
		// �������� ���� ���� ������� �ִϸ��̼� ��������
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
		// ����� �޽�, ��Ƽ���� ����. �޽��� ��Ƽ���� ���� ������
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
				// ���� �ε����� keys�� �ִ� �ε����� �Ѿ�� ���� �ε����� 0�ΰŴ�. 
				// �ٵ� ���� ���� ����? ��
				// ������.
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

