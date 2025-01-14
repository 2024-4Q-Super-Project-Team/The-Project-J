#include "Shared.fxh"
#include "Math.fxh"

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
    
    // ���� ���ڵ�
    if (USE_MAP(DIFFUSE_MAP) >= TRUE)
    {
        MapColor[DIFFUSE_MAP].rgb = pow(MapColor[DIFFUSE_MAP].rgb, GAMMA);
    }
    if (USE_MAP(EMISSIVE_MAP) >= TRUE)
    {
        MapColor[EMISSIVE_MAP].rgb = pow(MapColor[EMISSIVE_MAP].rgb, GAMMA);
    }
    // ������ ����
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
    
    // =====    ���� ���   =====
    float4 DirectLight  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 AmbientLight = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 F0 = lerp(Fdielectric, MapColor[DIFFUSE_MAP].rgb, MapColor[METALNESS_MAP].r);
    float3 VrefN = reflect(-V, N); // �ݻ� ����
    //float3 VrefN = 2.0 * NdotV * N - V;
    // ===== ���� ���� ��� =====
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
        
        float  ShadowScale   = CaclulateShadowScale(i, input.worldPos);
        
        DirectLight.rgb += (DifuuseBRDF + SpecularBRDF) * LightProp[i].Radiance.rgb * LightProp[i].LightIntensity * NdotL * ShadowScale;
        ACESToneMapping(DirectLight.rgb);
    }
    // ===== IBL ��� =====
    {
        // �� ������ Ư�� �� �� �����Ƿ� NdotV�� ����Ѵ�.
        float3 F = FresnelEquation(F0, NdotV);
        uint width, height, levels;
        IBLSpecularMap.GetDimensions(0, width, height, levels);
        uint SpecularTexLevels = levels;
        // Lr( View,Normal�� �ݻ纤��) �� ��ĥ�⸦ ����Ͽ� �ݻ� ���� ���ø��Ѵ�. 
        // ��ĥ�⿡ ���� ������ �ݻ� ���� ǥ���ϱ�����  LOD ���������� ����ȴ�. 
        float3 Irradiance = IBLDiffuseMap.Sample(LinearWrapSampler, N).rgb;
        float3 ReflectedLighColor = IBLSpecularMap.SampleLevel(LinearWrapSampler, VrefN, MapColor[ROUGHNESS_MAP].r * SpecularTexLevels).rgb;
        float2 SpecularBRDF = BRDFLookUpTable.Sample(LinearClampSampler, float2(NdotV, MapColor[ROUGHNESS_MAP].r)).rg;
        
        float3 DiffuseIBL = CalcuateDiffuseIBL(F, MapColor[DIFFUSE_MAP].rgb, MapColor[METALNESS_MAP].r, Irradiance);
        float3 SpecularIBL = CalcuateSpecularIBL(F0, SpecularBRDF, ReflectedLighColor);
    
        AmbientLight.rgb += (DiffuseIBL + SpecularIBL) * MapColor[AMBIENT_OCCLUSION_MAP].r * AmbientIntensity;
    }
    // �������� ���� ������ Emissive�� ���Ѵ�.
    FinalColor.rgb += MapColor[EMISSIVE_MAP].rgb;
    FinalColor.rgb += DirectLight.rgb;
    FinalColor.rgb += AmbientLight.rgb;
    // ���� ���ڵ� �۾��� ���ش�.
    FinalColor.rgb  = pow(FinalColor.rgb, 1.0 / GAMMA);
    FinalColor.a    = MapColor[OPACITY_MAP].a;
    return saturate(FinalColor);
}

