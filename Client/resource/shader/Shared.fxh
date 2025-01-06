#define MAX_LIGHT 10
#define MAX_BONES 100
#define MAX_BONE_WEIGHTS 4

struct STD_VS_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 uv : TEXCOORD0;
    int4 boneID : BLENDINDICES;
    float4 boneWeight : BLENDWEIGHT;
};

struct STD_VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 uv : TEXCOORD0;
    float4 worldPos : POSITION;
};

struct SKY_VS_OUT
{
    float3 uv : TEXCOORD0;
    float4 pos : SV_POSITION;
};

#define PI 3.141592
#define GAMMA 2.2
#define Fdielectric 0.04
#define Epsilon 0.00001

#define TRUE    1
#define FALSE   0

#define DIFFUSE_MAP 0
#define SPECULAR_MAP 1
#define AMBIENT_MAP 2
#define EMISSIVE_MAP 3
#define NORMAL_MAP 4
#define ROUGHNESS_MAP 5
#define OPACITY_MAP 6
#define METALNESS_MAP 7
#define AMBIENT_OCCLUSION_MAP 8
#define MATERIAL_MAP_COUNT 9

struct LightProperty
{
    float4 Position;
    float4 Direction;
    float4 Radiance;
    float4 DiffuseColor;
    float4 AmbientColor;
    float4 SpecularColor;
    int LightType;
    float AmbientStrength;
    float2 Padding;
};

struct MaterialProperty
{
    float4 DiffuseColor;
    float4 AmbientColor;
    float4 SpecularColor;
    float RoughnessScale;
    float MetallicScale;
    float AmbientOcclusionScale;
    float padding;
};

// =================================================
// Texture==========================================
// =================================================
Texture2D MaterialMap[MATERIAL_MAP_COUNT] : register(t0); // 머티리얼 텍스쳐 맵 0~8
// IBL 텍스쳐 맵
TextureCube IBLEnvironmentMap   : register(t20);
TextureCube IBLDiffuseMap       : register(t21);
TextureCube IBLSpecularMap      : register(t22);
Texture2D   BRDFLookUpTable     : register(t23);

// =================================================
// SamplerState=====================================
// =================================================
SamplerState LinearWrapSampler : register(s0);  
SamplerState LinearClampSampler : register(s1); 

// =================================================
// ConstantBuffer===================================
// =================================================
cbuffer TransformBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

// 매크로: 특정 플래그 확인
#define USE_MAP(x) ((UseMapFlags & (1 << x)) != 0)
cbuffer MaterialBuffer : register(b1)
{
    MaterialProperty MaterialProp;
    uint UseMapFlags; // 비트 플래그
}

cbuffer CameraBuffer : register(b2)
{
    float4 CameraPosition;
}

cbuffer BoneMatrixBuffer : register(b3)
{
    matrix boneTransforms[MAX_BONES];
}

cbuffer LightBuffer : register(b4)
{
    LightProperty LightProp[MAX_LIGHT];
    int NumLights;
    float3 padding;
}