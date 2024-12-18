#pragma once

#include <windows.h>
#include <filesystem>
#include <random>
#include <system_error>
#include <iostream>

#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include <unordered_set>

//Direct3D
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/PrimitiveBatch.h>
#include <Directxtk/VertexTypes.h>


#include <DirectXTex.h>
#include <Psapi.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
//ImgUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
//Assimp
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "../Common/Common.h"
#include "Helper/CommonEnum.h"
#include "Helper/CommonDef.h"
#include "Helper/CommonFunc.h"

#include "TimeManager/Time.h"
#include "Helper/Transform/Transform.h"

#include "Interface/IEngineCycle.h"
#include "Helper/Entity/Entity.h"
#include "Helper/Factory/Factory.h"

#include "../Window/IDisplay.h"
#include "Graphics/GraphicsInterface.h"
#include "Graphics/Graphics.h"

#include "Component/Render/Data/GraphicsEnum.h"
#include "Component/Render/Data/Cbuffer.h"

#include "Component/Render/Data/Model.h"
#include "Component/Render/Data/Mesh.h"
#include "Component/Render/Data/Material.h"
#include "Component/Render/Data/Bone.h"
#include "Component/Render/Data/Animation.h"

//Physics
#include <physx/PxPhysicsAPI.h>
#include <physx/PxPhysics.h>
using namespace physx;

