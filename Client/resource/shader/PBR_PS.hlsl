#include "Shared.fxh"

float3 CalcuateDiffuseIBL(float3 F, float3 albedo, float metalness, float3 irradiance)
{
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);
    return kd * albedo * irradiance;
}

float3 CalcuateSpecularIBL(float3 F0, float2 specularBRDF, float3 preFilteredColor)
{
    return (F0 * specularBRDF.x + specularBRDF.y) * preFilteredColor;
}

float3 CalcuateDiffuseBRDF(float3 F, float3 albedo, float metalness)
{
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);
 
    return kd * albedo / PI;
}

float3 CalcuateSpecularBRDF(float D, float3 F, float G, float NdotL, float NdotV)
{
    return (D * F * G) / max(Epsilon, 4.0 * NdotL * NdotV);
}
// cook-torrance : D
// 표면의 거칠기에 따라 Microfacet들이 얼마나  벡터와 정렬되는지 계산합니다.
float NormalDistributionFunction(float roughness, float HdotN)
{
    float A = roughness * roughness;
    float A2 = A * A;
    float D = (HdotN * HdotN) * (A2 - 1.0) + 1.0;
    return A2 / (PI * (D * D));
}
// cook-torrance : G
// Microfacet으로 인한 Occlusion으로 Shadowing과 Masking이 발생해 표면이 반사하는 빛을 줄일 수 있습니다.
float GeometryFunction(float roughness, float NdotL, float NdotV)
{
    float A = roughness + 1.0;
    float K = (A * A) / 8.0f;
    float G1 = NdotL / (NdotL * (1.0 - K) + K);
    float G2 = NdotV / (NdotV * (1.0 - K) + K);
    return G1 * G2;
}

// cook-torrance : F
// 서로 다른 각도의 표면에서 반사되는 빛의 비율을 나타냅니다.
// metalness가 1에 가까울 수록 경우 텍스처가 반사의 기본색상이 된다. 
float3 FresnelEquation( float3 F0, float HdotV)
{
    return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}

float4 main(STD_VS_OUTPUT input) : SV_TARGET
{
    float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 MapColor[MATERIAL_MAP_COUNT];
    
    MapColor[DIFFUSE_MAP]   = MaterialProp.DiffuseColor;
    MapColor[SPECULAR_MAP]  = MaterialProp.SpecularColor;
    MapColor[AMBIENT_MAP]   = MaterialProp.AmbientColor;
    MapColor[EMISSIVE_MAP]  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    MapColor[NORMAL_MAP]    = float4(0.0f, 0.0f, 0.0f, 0.0f);
    MapColor[ROUGHNESS_MAP] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[OPACITY_MAP]   = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[METALNESS_MAP] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[AMBIENT_OCCLUSION_MAP] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    [unroll]
    for (int MapType = 0; MapType < MATERIAL_MAP_COUNT; MapType++)
    {
        if (USE_MAP(MapType) >= TRUE)
        {
            MapColor[MapType] = MaterialMap[MapType].Sample(LinearWrapSampler, input.uv);
        }
    }
    if (MapColor[OPACITY_MAP].a < 0.01)
        discard;
    
    // 감마 디코딩
    if (USE_MAP(DIFFUSE_MAP) >= TRUE)
    {
        MapColor[DIFFUSE_MAP].rgb = pow(MapColor[DIFFUSE_MAP].rgb, GAMMA);
    }
    MapColor[ROUGHNESS_MAP]         *= MaterialProp.RoughnessScale;
    MapColor[METALNESS_MAP]         *= MaterialProp.MetallicScale;
    MapColor[AMBIENT_OCCLUSION_MAP] *= MaterialProp.AmbientOcclusionScale;
    // NormalMap
    float3 N = input.normal;
    if (USE_MAP(NORMAL_MAP) >= TRUE)
    {
        float3 tangentSpace = normalize(MapColor[NORMAL_MAP].rgb * 2.0f - 1.0f);
        float3x3 tangentWorld = float3x3(input.tangent, input.bitangent, input.normal);
        N = mul(tangentSpace, tangentWorld);
    }
    N = normalize(N);
    
    float3 V = normalize(CameraPosition.xyz - input.worldPos.xyz); // ViewDirection (World To Eye)
    float NdotV = saturate(dot(N, V));
    
    // =====    조명 계산   =====
    float4 DirectLight  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 AmbientLight = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 F0 = lerp(Fdielectric, MapColor[DIFFUSE_MAP].rgb, MapColor[METALNESS_MAP].r);
    float3 VrefN = reflect(-V, N); // 반사 벡터
    //float3 VrefN = 2.0 * NdotV * N - V;
    // ===== 직접 조명 계산 =====
    [unroll]
    for (int i = 0; i < NumLights; i++)
    {
        // LightDirection
        float3 L    = -normalize(LightProp[i].Direction.xyz);
        // HalfVector
        float3 H    = normalize(L + V);
        float NdotL = saturate(dot(N, L));
        float HdotN = saturate(dot(H, N));
        float HdotV = saturate(dot(H, V));
        
         // Fresnel
        float3 F = FresnelEquation(F0, HdotV);
         // Microfacet Distribution (GGX)
        float  D = NormalDistributionFunction(MapColor[ROUGHNESS_MAP].r, HdotN);
        // Geometry (Smith)
        float  G = GeometryFunction(MapColor[ROUGHNESS_MAP].r, NdotL, NdotV);
        
        float3 DifuuseBRDF  = CalcuateDiffuseBRDF(F, MapColor[DIFFUSE_MAP].rgb, MapColor[METALNESS_MAP].r);
        float3 SpecularBRDF = CalcuateSpecularBRDF(D, F, G, NdotL, NdotV);

        DirectLight.rgb += (DifuuseBRDF + SpecularBRDF) * LightProp[i].Radiance.rgb * NdotL;
    }
    // ===== IBL 계산 =====
    {
        // 빛 방향은 특정 할 수 없으므로 NdotV을 사용한다.
        float3 F = FresnelEquation(F0, NdotV);
        uint width, height, levels;
        IBLSpecularMap.GetDimensions(0, width, height, levels);
        uint SpecularTexLevels = levels;
        // Lr( View,Normal의 반사벡터) 와 거칠기를 사용하여 반사 빛을 샘플링한다. 
        // 거칠기에 따라 뭉게진 반사 빛을 표현하기위해  LOD 선형보간이 적용된다. 
        float3 Irradiance = IBLDiffuseMap.Sample(LinearWrapSampler, N).rgb;
        float3 ReflectedLighColor = IBLSpecularMap.SampleLevel(LinearWrapSampler, VrefN, MapColor[ROUGHNESS_MAP].r * SpecularTexLevels).rgb;
        float2 SpecularBRDF = BRDFLookUpTable.Sample(LinearClampSampler, float2(NdotV, MapColor[ROUGHNESS_MAP].r)).rg;
        
        float3 DiffuseIBL = CalcuateDiffuseIBL(F, MapColor[DIFFUSE_MAP].rgb, MapColor[METALNESS_MAP].r, Irradiance);
        float3 SpecularIBL = CalcuateSpecularIBL(F0, SpecularBRDF, ReflectedLighColor);
    
        AmbientLight.rgb += (DiffuseIBL + SpecularIBL) * MapColor[AMBIENT_OCCLUSION_MAP].r;
    }
    // 최종색상에 계산된 조명값과 Emissive 등을 합한다.
    if (USE_MAP(EMISSIVE_MAP) >= TRUE)
    {
        FinalColor.rgb += pow(MapColor[EMISSIVE_MAP], GAMMA);
    }
    
    FinalColor.rgb += DirectLight.rgb;
    FinalColor.rgb += AmbientLight.rgb;
    // 감마 인코드 작업을 해준다.
    FinalColor.rgb  = pow(FinalColor.rgb, 1.0 / GAMMA);
    FinalColor.a    = MapColor[OPACITY_MAP].a;
    return saturate(FinalColor);
}