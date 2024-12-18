#include "pch.h"
#include "SkinningMeshRenderer.h"
#include "Object/Object.h"
#include "Graphics/GraphicsManager.h"

namespace Component
{
	SkinningMeshRenderer::SkinningMeshRenderer(GameObject* _owner)
		: RendererBase(_owner)
	{
		mType = ComponentType::SkinningMeshRenderer;
	}

	SkinningMeshRenderer::~SkinningMeshRenderer()
	{
	}
	void SkinningMeshRenderer::Render(GraphicsManager* _graphicsManager)
	{
		mAnimationState->UpdateState();
		if (mRootNode)
		{
			// ��Ʈ���� ������Ʈ�� �����Ʈ������ �����ͼ� ����.
			mRootNode->mLocalMatrix = ownerObject->transform->GetWorldMatrix();
		}

	}
	bool SkinningMeshRenderer::SetModel(std::shared_ptr<Graphics::ModelResource> _spModel)
	{
		if (_spModel.get())
		{
			mModel = _spModel;
			mMaterials.clear();
			mMeshs.clear();
			if (mModel.get())
			{
				mRootNode = CreateNodeFromModelNode(mRootNode, mModel.get()->mRootNode);
			}
			return true;
		}
		return false;
	}
	SkinningMeshRenderer::Node* SkinningMeshRenderer::CreateNodeFromModelNode(Node* _parent, Graphics::ModelNodeResource* _modelNode)
	{
		if (nullptr == _modelNode)
			return nullptr;
		SkinningMeshRenderer::Node* Node = new SkinningMeshRenderer::Node(_modelNode->mNodeName, _parent, this);
		for (int i = 0; i < _modelNode->mMeshResources.size(); ++i)
		{
			Graphics::MaterialState* pMaterialState 
				= new Graphics::MaterialState(_modelNode->mMaterials[i]);
			Graphics::MeshState* pMeshState
				= new Graphics::MeshState(_modelNode->mMeshResources[i], pMaterialState);
			// ���������� ����
			mMaterials[pMaterialState->GetName()] = pMaterialState;
			mMeshs[pMeshState->GetName()] = pMeshState;
			// ��忡 �޽�, ��Ƽ���� ���� ����
			Node->mMeshStates.push_back(pMeshState);
		}
		for (auto node : _modelNode->mChildren)
		{
			Node->mChildren.push_back(
				CreateNodeFromModelNode(Node, node));
		}
		return Node;
	}
	void SkinningMeshRenderer::CalculateAnimation()
	{

	}
	void SkinningMeshRenderer::CalculateNodeTransform(Node* _node)
	{
	}
	bool SkinningMeshRenderer::SetAnimation(const std::wstring& _key)
	{
		Graphics::AnimationResource* wpAnim = mModel.get()->GetAnimation(_key);
		if (wpAnim)
		{
			mAnimationState->SetAnimation(wpAnim);
			mAnimationState->SetTimer(0.0f);
			return true;
		}
		return false;
	}

	SkinningMeshRenderer::Node::Node(std::wstring_view _name, Node* _parent, SkinningMeshRenderer* _owner)
		: mName(_name), mParent(_parent), mOwner(_owner)
	{
	}
	SkinningMeshRenderer::Node::~Node()
	{
		for (auto child : mChildren)
			delete child;
	}
	void SkinningMeshRenderer::Node::Calculate()
	{
		CalculateNodeAnimation();
		CalculateNodeTransform();
	}
	void SkinningMeshRenderer::Node::Render(GraphicsManager* _graphicsManager)
	{
		Graphics::Renderer* renderer = _graphicsManager->GetRenderer();
		if (renderer)
		{
			// ���߿� ���� ��Ƽ� �����ؾ��Ѵ�.
			// GraphicsManager�� �� ������ �ϸ� �ɵ�?
			for (auto*& MeshState : mMeshStates)
			{
				Graphics::MaterialState* pMaterialState = MeshState->mMaterialState;
				for (int i = 0; i < static_cast<UINT>(Graphics::eMaterialMapType::SIZE); ++i)
				{
					if (TRUE == pMaterialState->UseMaterialMapType((Graphics::eMaterialMapType)i))
						renderer->BindTextrue(
							Graphics::eShaderStage::PS,
							i,
							pMaterialState->GetMapTexture((Graphics::eMaterialMapType)i).get()
						);
				}
			}
		}
	}
	void SkinningMeshRenderer::Node::CalculateNodeTransform()
	{
		if (mParent)
		{
			Graphics::ModelResource* pModel = mOwner->mModel.get();
			// ���� �ִϸ��̼��� ������ ?
			if (mOwner->mAnimationState->GetCurrentAnimationResource())
			{
				mWorldMatrix = mLocalMatrix * mParent->mWorldMatrix;
			}
			else
			{ // ������ ����� ����� ����Ѵ�.
				const Matrix& nodeMatrix = pModel->GetModelNodeResource(mName)->mTransformation;
				mWorldMatrix = nodeMatrix * mParent->mWorldMatrix;
			}
		}
		else
		{	// ��Ʈ���� ������Ʈ�� ������ǥ�� ���÷� ������ �ִ´�.
			// �׷��Ƿ� ���� = ����
			mWorldMatrix = mLocalMatrix;
		}

		// �� ��Ʈ���� ����
		Graphics::BoneResource* bone = mOwner->mModel.get()->GetBoneResource(mName);
		if (bone)
		{
			mOwner->mBoneState->SetBoneMatrix(bone, mWorldMatrix);
		}
	}


	void SkinningMeshRenderer::Node::CalculateNodeAnimation()
	{
		Graphics::AnimationState* pAnimState = mOwner->mAnimationState;
		Graphics::AnimationResource* pCurrAnim = pAnimState->GetCurrentAnimationResource();
		if (pCurrAnim)
		{
			Graphics::ChannelResource* pCurrChannel = pCurrAnim->GetChannel(mName);
			// ä���� ������ ������ �ϰ� �ƴϸ� �н�
			if (pCurrChannel)
			{
				// ������, ȸ��, �����Ϻ��� keyTime�� �ٸ� ���� �ֳ�? �ϴ� Assimp�� ������ �Ľ��߱� ������ �̷��� ®��
				Vector3		Position;
				CalculateAnimationKey(pAnimState->GetTimer(), pCurrChannel->mPositionKeys, Position);
				Quaternion	Rotation;
				CalculateAnimationKey(pAnimState->GetTimer(), pCurrChannel->mRotationKeys, Rotation);
				Vector3		Scale;
				CalculateAnimationKey(pAnimState->GetTimer(), pCurrChannel->mScalingKeys, Scale);

				mLocalMatrix = Matrix::CreateScale(Position) *
					Matrix::CreateFromQuaternion(Rotation) *
					Matrix::CreateTranslation(Scale);
			}
		}
	}
}


