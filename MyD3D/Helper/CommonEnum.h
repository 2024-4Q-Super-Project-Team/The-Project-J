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
    PBR,
    SKYBOX,
    SPRITE,
    SIZE,
};
#define PS_TYPE_COUNT static_cast<UINT>(ePixelShaderType::SIZE)

enum class eBlendingMode
{
    OPAQUE_BLEND,
    CUTOUT_BLEND,
    TRANSPARENT_BLEND,
    POSTPROCESS_BLEND,
    SIZE,
};
#define RENDERING_MODE_COUNT static_cast<UINT>(eBlendingMode::SIZE)