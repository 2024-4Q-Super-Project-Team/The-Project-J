#pragma once
#define NOMINMAX

#include <windows.h>
#include <filesystem>
#include <random>
#include <system_error>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <wrl/client.h> 
#include <type_traits>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include <unordered_set>

// FMOD
#include "ThirdParty/fmod/include/fmod.hpp"
#include "ThirdParty/fmod/include/fmod_studio.hpp"
#include "ThirdParty/fmod/include/fmod_common.h"
#include "ThirdParty/fmod/include/fmod_codec.h"
#pragma comment (lib, "../MyD3D/ThirdParty/fmod/fmod_vc.lib")
#pragma comment (lib, "../MyD3D/ThirdParty/fmod/fmodstudioL_vc.lib")
//Direct3D
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>
#include <Directxtk/PrimitiveBatch.h>
#include <Directxtk/VertexTypes.h>
#include <DirectXTex.h>
#include <Psapi.h>
using namespace DirectX;
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#define IMGUI_DEFINE_MATH_OPERATORS
//ImgUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
//Imguizmo
#include "Thirdparty\imguizmo\ImGuizmo.h"
#include "Thirdparty\imguizmo\ImSequencer.h"
#include "Thirdparty\imguizmo\ImZoomSlider.h"
#include "Thirdparty\imguizmo\ImCurveEdit.h"
#include "Thirdparty\imguizmo\GraphEditor.h"
#include "Thirdparty\imguizmo\ImGradient.h"
// SimpleMath
#include <directxtk/SimpleMath.h>
using namespace DirectX::SimpleMath;
//Assimp
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
//Physics
#include <physx/PxPhysicsAPI.h>
#include "physx/cudamanager/PxCudaContextManager.h"
#include "physx/cudamanager/PxCudaContext.h"

using namespace physx;
//json
#include <nlohmann/json.hpp>
using json = nlohmann::json;
// UI Text
#include <tchar.h>
#include <cstdarg>  // 가변 인자 처리
#include <stdexcept>
#include <directxtk/SpriteBatch.h>
#include <directxtk/SpriteFont.h>
#include "../Window/framework.h"
// Helper
#include "Helper/Data/ColorF.h"
#include "Helper/Data/Angles.h"
#include "Helper/CommonDef.h"
#include "Helper/CommonEnum.h"
#include "Helper/CommonFunc.h"
#include "Helper/Time/Time.h"
#include "Helper/Input/Input.h"
#include "Helper/Math/Math.h"
#include "Helper/Math/Random.h"
#include "Helper/Dotween/DotTween.h"
#include "Helper/Dotween/EasingFunction.h"

#include "Graphics/Graphics.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/GraphicsFramework.h"

#include "AudioSystem/AudioFramework.h"

#include "Component/ComponentFactory/ComponentFactory.h"
#include "Manager/GameManager.h"
#include "Editor/EditorManager.h"
#include "UIManager/UIManager.h"
#include "Save/SaveManager.h"

//Debug Draw
#include "Graphics/Extra/DebugDraw.h"
#include "Directxtk/VertexTypes.h"
#include "Directxtk/Effects.h"
#include "Directxtk/CommonStates.h"
using namespace DirectX::DX11;

#define NULLID 9999999
