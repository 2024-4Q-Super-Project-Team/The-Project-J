#pragma once


enum class eCBufferType
{
    Transform,
    Material,
    Camera,
    BoneMatrix,
    Light,
    ParticleSize,
    SIZE
};
#define CBUFFER_TYPE_COUNT static_cast<UINT>(eCBufferType::SIZE)

enum class eMaterialMapType
{
    DIFFUSE,
    SPECULAR,
    AMBIENT,
    EMISSIVE,
    NORMAL,
    ROUGHNESS,
    OPACITY,
    METALNESS,
    AMBIENT_OCCLUSION,
    SIZE
};
#define MATERIAL_MAP_SIZE static_cast<UINT>(eMaterialMapType::SIZE)

enum class eSamplerStateType
{
    LINEAR_WRAP,
    LINEAR_CLAMP,
    LINEAR_COMPARISON,
    SIZE,
};
#define SAMPLER_STATE_TYPE_COUNT static_cast<UINT>(eSamplerStateType::SIZE)

enum class eVertexShaderType
{
    STANDARD,
    SKYBOX,
    SPRITE,
    SHADOW,
    PARTICLE,
    GRID,
    SIZE,
};
#define VS_TYPE_COUNT static_cast<UINT>(eVertexShaderType::SIZE)

enum class eGeometryShaderType
{
    PARTICLE,
    SIZE
};
#define GS_TYPE_COUNT static_cast<UINT>(eVertexShaderType::SIZE)

enum class ePixelShaderType
{
    BLINN_PHONG,
    FOWARD_PBR,
    DEFERRED_PBR,
    SKYBOX,
    SPRITE,
    G_BUFFER,
    PARTICLE,
    GRID,
    SIZE,
};
#define PS_TYPE_COUNT static_cast<UINT>(ePixelShaderType::SIZE)

enum class eBlendStateType
{
    //FOWARD_OPAQUE,        // 포워드 렌더링용 Opaque블렌드
    //DEFERRED_OPAQUE,      // 디퍼드 렌더링용 Opaque블렌드(rgba값을 그대로 사용)
    //FOWARD_TRANSPARENT,   // 포워드 렌더링용 Transparent블렌드(디퍼드도 이걸 사용)
    DEFAULT,
    ALPHA,
    SIZE,
};
#define BLEND_STATE_TYPE_COUNT static_cast<UINT>(eBlendStateType::SIZE)

enum class eRasterizerStateType
{
    NONE_CULLING,
    BACKFACE_CULLING,
    FRONTFACE_CULLING,
    SIZE,
};
#define RASTERIZER_STATE_TYPE_COUNT static_cast<UINT>(eRasterizerStateType::SIZE)

enum class eDepthStencilStateType
{
    DEFAULT,       
    SIZE,
};
#define DEPTHSTENCIL_STATE_TYPE_COUNT static_cast<UINT>(eDepthStencilStateType::SIZE)

enum class eBlendModeType
{
    OPAQUE_BLEND,           // Opaque블렌드
    TRANSPARENT_BLEND,      // Transparent블렌드
    WIREFRAME_BELND,        // WireFrame블렌드
    SIZE,
};
#define BLEND_MODE_TYPE_COUNT static_cast<UINT>(eBlendModeType::SIZE)

enum class eGBufferType
{
    ALBEDO,
    NORMAL,
    MATERIAL,
    EMESSIVE,
    WORLD_POSITION,
    SIZE,
};
#define GBUFFER_TYPE_COUNT static_cast<UINT>(eGBufferType::SIZE)