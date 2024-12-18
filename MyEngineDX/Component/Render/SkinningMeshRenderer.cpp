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
			// 루트노드는 오브젝트의 월드매트릭스를 가져와서 쓴다.
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
			// 렌더러에도 저장
			mMaterials[pMaterialState->GetName()] = pMaterialState;
			mMeshs[pMeshState->GetName()] = pMeshState;
			// 노드에 메쉬, 머티리얼 정보 저장
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
			// 나중엔 따로 모아서 정렬해야한다.
			// GraphicsManager가 이 역할을 하면 될듯?
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
			// 현재 애니메이션이 있으면 ?
			if (mOwner->mAnimationState->GetCurrentAnimationResource())
			{
				mWorldMatrix = mLocalMatrix * mParent->mWorldMatrix;
			}
			else
			{ // 없으면 노드의 행렬을 사용한다.
				const Matrix& nodeMatrix = pModel->GetModelNodeResource(mName)->mTransformation;
				mWorldMatrix = nodeMatrix * mParent->mWorldMatrix;
			}
		}
		else
		{	// 루트노드는 오브젝트의 월드좌표를 로컬로 가지고 있는다.
			// 그러므로 월드 = 로컬
			mWorldMatrix = mLocalMatrix;
		}

		// 본 매트릭스 세팅
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
			// 채널이 있으면 연산을 하고 아니면 패스
			if (pCurrChannel)
			{
				// 포지션, 회전, 스케일별로 keyTime이 다를 일이 있나? 일단 Assimp가 나눠서 파싱했기 때문에 이렇게 짰음
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


