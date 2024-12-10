#pragma once

#include <windows.h>
#include <filesystem>
#include <random>
#include <system_error>

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
#include <DirectXTex.h>
#include <Psapi.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
//ImgUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "../Common/Common.h"
#include "Helper/CommonEnum.h"
#include "Helper/CommonDef.h"

#include "TimeManager/Time.h"
#include "Helper/Transform/Transform3D.h"

#include "Interface/IEngineCycle.h"
#include "Helper/Entity/Entity.h"
#include "Helper/Factory/Factory.h"

#include "../Window/IDisplay.h"
#include "Graphics/Graphics.h"