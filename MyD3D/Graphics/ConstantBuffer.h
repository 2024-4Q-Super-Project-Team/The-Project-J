#pragma once

struct MaterialProperty
{
    // ³­¹Ý»ç ºû
    ColorF	DiffuseRGB  = { 1.0f,1.0f,1.0f,1.0f };
    // ÁÖº¯±¤ ºû
    ColorF	AmbientRGB  = { 1.0f,1.0f,1.0f,1.0f };
    // Á¤¹Ý»ç ºû
    ColorF	SpecularRGB = { 1.0f,1.0f,1.0f,1.0f };

    FLOAT	RoughnessScale = 0.0f;
    FLOAT   MetallicScale = 0.0f;
    FLOAT   AmbienOcclusionScale = 0.0f;
    FLOAT   Padding;
};

struct LightProperty
{
    Vector4 Position    = { 0.0f,0.0f,0.0f,1.0f };
    Vector4 Direction   = { 0.0f,0.0f,0.0f,0.0f };
    ColorF  Radiance    = { 1.0f,1.0f,1.0f,1.0f };
    ColorF  DiffuseRGB  = { 1.0f,1.0f,1.0f,1.0f };
    ColorF  AmbientRGB  = { 1.0f,1.0f,1.0f,1.0f };
    ColorF  SpecularRGB = { 1.0f,1.0f,1.0f,1.0f };
    INT     LightType = 0;
    FLOAT   AmbientStrength = 1.0f;
    Vector2 Padding;
};

struct TransformCBuffer
{
    Matrix World;
    Matrix View;
    Matrix Projection;
};

struct MaterialCBuffer
{
    MaterialProperty MatProp;
    UINT	         UseMapFlag;
    Vector3          Padding;
    BOOL GetUsingMap(eMaterialMapType _type);
    void SetUsingMap(eMaterialMapType _type, BOOL _bValue);
    void ToggleUsingMap(eMaterialMapType _type);
};

struct CameraCBuffer
{
    Vector4 Position;
    //....
};

#define MAX_BONE_COUNT 128
struct BoneMatrixCBuffer
{
    Matrix  finalBoneMatrix[MAX_BONE_COUNT];
};

#define MAX_LIGHT_COUNT 10
struct LightCBuffer
{
    LightProperty LightProp[MAX_LIGHT_COUNT];
    INT           NumLight = 0;
    Vector3       Padding;
};

