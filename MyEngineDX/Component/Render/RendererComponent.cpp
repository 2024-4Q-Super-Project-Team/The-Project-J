#include "pch.h"
#include "RendererComponent.h"
#include "Data/Mesh.h"
#include "Data/Material.h"
#include "Data/Bone.h"
#include "Data/Animation.h"
#include "Data/Model.h"

namespace Component
{
	RendererBase::RendererBase(GameObject* _owner) : ComponentBase(_owner)
	{
	}
	RendererBase::~RendererBase()
	{
	}
}