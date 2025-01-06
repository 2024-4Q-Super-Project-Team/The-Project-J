#pragma once

#define SHADER_STAGE_SIZE static_cast<UINT>(eShaderStage::ALL)

enum class eCBufferType
{
    Transform,
    Material,
    Camera,
    BoneMatrix,
    Light,
    SIZE
};
#define CBUFFER_SIZE static_cast<UINT>(eCBufferType::SIZE)

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
    SIZE,
};