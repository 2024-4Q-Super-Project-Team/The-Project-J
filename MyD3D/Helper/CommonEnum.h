#pragma once


enum class eCBufferType
{
    Transform,
    Material,
    Camera,
    BoneMatrix,
    Light,
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
    SIZE,
};
#define VS_TYPE_COUNT static_cast<UINT>(eVertexShaderType::SIZE)

enum class ePixelShaderType
{
    BLINN_PHONG,
    FOWARD_PBR,
    DEFERRED_PBR,
    SKYBOX,
    SPRITE,
    G_BUFFER,
    SIZE,
};
#define PS_TYPE_COUNT static_cast<UINT>(ePixelShaderType::SIZE)

enum class eBlendType
{
    OPAQUE_BLEND,
    TRANSPARENT_BLEND,
    SIZE,
};
#define BLEND_TYPE_COUNT static_cast<UINT>(eBlendType::SIZE)

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