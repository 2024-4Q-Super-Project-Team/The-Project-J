#define MAX_LIGHT 10
#define MAX_BONES 150
#define MAX_BONE_WEIGHTS 4

struct STD_VS_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 uv : TEXCOORD0;
    int4   boneID : BLENDINDICES;
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
    float4 worldPos : TEXCOORD1;
};

struct SKY_VS_OUT
{
    float3 uv : TEXCOORD0;
    float4 pos : SV_POSITION;
};

struct QUAD_VS_INPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct QUAD_VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct GRID_VS_INPUT
{
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
};

struct GRID_VS_OUTPUT
{
    float4 pos      : SV_POSITION;
    float4 color    : COLOR;
};

struct DEFERRED_PS_OUT
{
    float4 albedo   : SV_Target0;
    float4 normal   : SV_Target1;
    float4 material : SV_Target2;
    float4 emissive : SV_Target3;
    float4 worldpos : SV_Target4;
};

struct OUTLINE_VS_OUTPUT
{
    float4 pos : SV_POSITION;
};


#define PI 3.141592
#define GAMMA 2.2
#define Fdielectric 0.04
#define Epsilon 0.0005

#define TRUE    1
#define FALSE   0

#define DIFFUSE_MAP     0
#define SPECULAR_MAP    1
#define AMBIENT_MAP     2
#define EMISSIVE_MAP    3
#define NORMAL_MAP      4
#define ROUGHNESS_MAP   5
#define OPACITY_MAP     6
#define METALNESS_MAP   7
#define AMBIENT_OCCLUSION_MAP 8
#define MATERIAL_MAP_COUNT 9

struct LightProperty
{
    float4 Position;
    float4 Direction;
    float4 Radiance;
    int    LightType;
    float  LightRadius;
    float  LightStrengh;
    float  LightCutOff;
    // ==== Shadow ====
    int    UseShadow;
    float  ShadowStrengh;
    float2 Padding;
    Matrix ShadowViewMatrix;
    Matrix ShadowProjectionMatrix;
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
 // 머티리얼 텍스쳐 맵 0~10까지 사용할 것
Texture2D   MaterialMap[MATERIAL_MAP_COUNT] : register(t0);
// 카메라 렌더타겟
Texture2D   CameraRenderTarget              : register(t11);
// 디퍼드용 G-Buffer
Texture2D   GBuffer_Albedo                  : register(t12);
Texture2D   GBuffer_Normal                  : register(t13);
Texture2D   GBuffer_Material                : register(t14);
Texture2D   GBuffer_Emessive                : register(t15);
Texture2D   GBuffer_WorldPos                : register(t16);
// IBL 텍스쳐 맵
TextureCube IBLEnvironmentMap               : register(t20);
TextureCube IBLDiffuseMap                   : register(t21);
TextureCube IBLSpecularMap                  : register(t22);
Texture2D   BRDFLookUpTable                 : register(t23);
// 그림자 텍스쳐
#define SHADOW_SRV_OFFSET 24
#define SHADOW_SRV_SIZE   4096.0f
Texture2D ShadowTexture[MAX_LIGHT]          : register(t24);
Texture2D particleTexture                   : register(t25);


// =================================================
// SamplerState=====================================
// =================================================
SamplerState LinearWrapSampler : register(s0);  
SamplerState LinearClampSampler : register(s1); 
SamplerComparisonState LinearComparisonSampler : register(s2);

// =================================================
// ConstantBuffer===================================
// =================================================
cbuffer TransformBuffer : register(b0)
{
    matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
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
    matrix CameraView;
    matrix ivCameraView;
    matrix CameraProjection;
    matrix ivCameraProjection;
}

cbuffer BoneMatrixBuffer : register(b3)
{
    matrix boneTransforms[MAX_BONES];
}

cbuffer LightBuffer : register(b4)
{
    LightProperty LightProp[MAX_LIGHT];
    uint NumLights;
    uint LightFlags;
    uint ShadowFlags;
    float AmbientIntensity;
}

cbuffer ParticleSizeBuffer : register(b5)
{
    float particleSize;
    float padding[3];
};

cbuffer OutlineBuffer : register(b6)
{
    float4 outlineColor;
    float outlineOffset;
    float paddingg[3];
};
