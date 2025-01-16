#pragma once

struct MaterialProperty
{
    // 난반사 빛
    ColorF	DiffuseRGB = { 1.0f,1.0f,1.0f,1.0f };
    // 주변광 빛
    ColorF	AmbientRGB = { 1.0f,1.0f,1.0f,1.0f };
    // 정반사 빛
    ColorF	SpecularRGB = { 1.0f,1.0f,1.0f,1.0f };
    // 거칠기 기본 값 0.5
    FLOAT	RoughnessScale = 0.5f;
    // 금속성 기본 값 0.0
    FLOAT   MetallicScale = 0.0f;
    // AO강도
    FLOAT   AmbienOcclusionScale = 0.0f;
    FLOAT   Padding;
};

struct LightProperty
{
    Vector4 Position = { 0.0f,0.0f,0.0f,1.0f };
    Vector4 Direction = { 0.0f,0.0f,0.0f,0.0f };
    ColorF  Radiance = { 1.0f,1.0f,1.0f,1.0f };
    INT     LightType = 0;
    FLOAT   LightRange = 1.0f; // Point and Spot lights only
    FLOAT   LightStrengh = 1.0f; 
    // ==== Shadow ====
	BOOL    UseShadow = TRUE;
    FLOAT   ShadowStrengh = 1.0f;
	Vector3 Padding;

    Matrix  ShadowView = Matrix::Identity;
    Matrix  ShadowProjection = Matrix::Identity;
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
    Matrix  View;
    Matrix  InverseView;
    Matrix  Projection;
    Matrix  InverseProjection;
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
    // 조명 수
    UINT          NumLight = 0;
    UINT          LightFlags;
    UINT          ShadowFlags;
    // 환경광 강도
    FLOAT         AmbientIntensity = 1.0f;

    BOOL GetLightFlag(UINT _flag);
    void SetLightFlag(UINT _flag, BOOL _bValue);
    BOOL GetShadowFlag(UINT _flag);
    void SetShadowFlag(UINT _flag, BOOL _bValue);
};

#define LIGHT_FLAG_SHADOW_USE_PCF 1