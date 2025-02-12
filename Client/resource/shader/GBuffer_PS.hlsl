#include "Shared.fxh"

DEFERRED_PS_OUT main(STD_VS_OUTPUT input)
{
    DEFERRED_PS_OUT output;
    
    float4 MapColor[MATERIAL_MAP_COUNT];
    
    MapColor[DIFFUSE_MAP]   = MaterialProp.DiffuseColor;
    MapColor[SPECULAR_MAP]  = MaterialProp.SpecularColor;
    MapColor[AMBIENT_MAP]   = MaterialProp.AmbientColor;
    MapColor[EMISSIVE_MAP]  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    MapColor[NORMAL_MAP]    = float4(0.0f, 0.0f, 0.0f, 1.0f);
    MapColor[ROUGHNESS_MAP] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[OPACITY_MAP]   = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[METALNESS_MAP] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[AMBIENT_OCCLUSION_MAP] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    [unroll]
    for (int MapType = 0; MapType < MATERIAL_MAP_COUNT; MapType++)
    {
        if (USE_MAP(MapType) >= TRUE && HAS_MAP(MapType) >= TRUE)
        {
            MapColor[MapType] = MaterialMap[MapType].Sample(LinearWrapSampler, input.uv);
        }
    }
    
    //////////////////////////////////////////////////
    // Albedo = rgb(Albedo), a(Opacity)
    //////////////////////////////////////////////////
    output.albedo.rgb   = MapColor[DIFFUSE_MAP].rgb; // 감마 디코드해서 넘기기
    output.albedo.a     = MapColor[OPACITY_MAP].a;
    if (USE_MAP(DIFFUSE_MAP) >= TRUE && HAS_MAP(DIFFUSE_MAP) >= TRUE)
    {
        output.albedo.rgb = pow(output.albedo.rgb, GAMMA);
    }
    
    //////////////////////////////////////////////////
    // Normal = rgb(Normal), a(Depth)
    //////////////////////////////////////////////////
    output.normal.rgb = input.normal;
    if (USE_MAP(NORMAL_MAP) >= TRUE && HAS_MAP(NORMAL_MAP) >= TRUE)
    {
        float3 normalTex = normalize(MapColor[NORMAL_MAP].rgb * 2.0f - 1.0f);
        float3x3 TBN = float3x3(input.tangent, input.bitangent, input.normal);
        output.normal.rgb = mul(normalTex, TBN);
    }
    output.normal.rgb = normalize(output.normal.rgb);
    output.normal.a = input.pos.z;
    
    //////////////////////////////////////////////////
    // Material = r(Metallic), g(Specualr), b(Roughness), a(AmbienOcclusion)
    //////////////////////////////////////////////////
    output.material.r = MapColor[METALNESS_MAP].r * MaterialProp.MetallicScale;
    output.material.g = MapColor[SPECULAR_MAP].r;
    output.material.b = MapColor[ROUGHNESS_MAP].r * MaterialProp.RoughnessScale;
    output.material.a = MapColor[AMBIENT_OCCLUSION_MAP].r * MaterialProp.AmbientOcclusionScale;

    //////////////////////////////////////////////////
    // Emissive = rgb(Emissive), 
    //////////////////////////////////////////////////
    output.emissive.rgb = MapColor[EMISSIVE_MAP].rgb;
    if (USE_MAP(EMISSIVE_MAP) >= TRUE && HAS_MAP(EMISSIVE_MAP) >= TRUE)
    {
        output.emissive.rgb = pow(output.emissive.rgb, GAMMA);
    }
    //////////////////////////////////////////////////
    // World = rgba(WorldPosition)
    //////////////////////////////////////////////////
    output.worldpos = input.worldPos;
    
    return output;
}