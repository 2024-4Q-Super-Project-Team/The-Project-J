#include "Shared.fxh"

float4 main(STD_VS_OUTPUT input) : SV_TARGET
{
    float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 MapColor[MATERIAL_MAP_COUNT];
    
    MapColor[DIFFUSE_MAP]   = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[SPECULAR_MAP]  = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[AMBIENT_MAP]   = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[EMISSIVE_MAP]  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    MapColor[NORMAL_MAP]    = float4(0.0f, 0.0f, 0.0f, 0.0f);
    MapColor[ROUGHNESS_MAP] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[OPACITY_MAP]   = float4(1.0f, 1.0f, 1.0f, 1.0f);
    MapColor[METALNESS_MAP] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    for (int MapType = 0; MapType < MATERIAL_MAP_COUNT; MapType++)
    {
        if (USE_MAP(MapType) == TRUE)
        {
            MapColor[MapType] = MaterialMap[MapType].Sample(LinearWrapSampler, input.uv);
        }
    }
    
    // NormalMap
    float N = input.normal;
    if (USE_MAP(NORMAL_MAP) == TRUE)
    {
        float3 tangentSpace = MapColor[NORMAL_MAP].rgb * 2.0f - 1.0f;
        float3x3 tangentMatrix = float3x3(input.tangent, input.bitangent, input.normal);
        N = normalize(mul(tangentSpace, tangentMatrix));
    }
    // 조명 계산
    float4 DiffuseLight  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 AmbientLight  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 SpecularLight = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 EmissiveLight = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // ViewDirection (World To Eye)
    float3 V = normalize(CameraPosition.xyz - input.worldPos.xyz);
    
    for (int i = 0; i < NumLights; i++)
    {
        // LightDirection (Light To Wolrd)
        float3 L = normalize(-LightProp[i].Direction);
        // HalfVector
        float3 H = normalize(L + V);
        float NDotL = max(dot(N, L), 0.0f);
        float HDotN = max(dot(H, N), 0.0f);
        // N·L * 광원 Diff * 머티리얼 Diff * 머티리얼 Diff_Map(표면 색상)
        DiffuseLight = NDotL * MaterialProp.DiffuseColor * pow(MapColor[DIFFUSE_MAP], GAMMA);
        // 주변광 확산율 * 광원 Ambi * 머티리얼 Ambi * 머티리얼 Ambi_Map
        AmbientLight = MaterialProp.AmbientColor * MapColor[AMBIENT_MAP];
        // ( H·N (or R·V) ^ 머티리얼 Shin(광택) ) * 광원 Spec * 머티리얼 Spec
        SpecularLight = pow(HDotN, MaterialProp.RoughnessScale) * MaterialProp.SpecularColor * MapColor[SPECULAR_MAP];
        // 머티리얼 Emissive_Map
        EmissiveLight = MapColor[EMISSIVE_MAP];
        FinalColor += (DiffuseLight + AmbientLight + SpecularLight + EmissiveLight);
    }
    
    FinalColor.a = MapColor[OPACITY_MAP].a; // 알파값 설정
    
    FinalColor.rgb = pow(FinalColor.rgb, 1.0 / GAMMA);
    return saturate(FinalColor); // 최종 색상 제한
}

//float3 CalculateDirectionLight(LightProperty Light, MaterialProperty Material, float3 Normal, float3 View, float4 SpecMapColor)
//{
    
//}

/*
LightProp[i].AmbientStrength
주변광의 확산율이나 강도를 조정하는 변수로, 각 라이트의 환경에 따라 조정 가능.
예: 강한 태양광에서는 AmbientStrength가 높고, 실내 조명이나 약한 빛에서는 낮음.

LightProp[i].AmbientColor
라이트의 고유 주변광 색상.
예: 노을의 빛은 따뜻한 주황색, 밤에는 어두운 파란색 계열.

MaterialProp.AmbientColor
재질(Material)이 주변광에 얼마나 반응하는지를 나타내는 계수(색상).
예: 금속은 낮은 값, 플라스틱이나 천은 높은 값.

MapColor[AMBIENT_MAP]
재질 텍스처 맵으로, 표면마다 다른 주변광 반응을 표현할 수 있음.

AmbientStrength는 특정 라이트의 주변광 효과를 제어하므로, 환경 조명과 조화롭게 설정해야 합니다.

낮에는 AmbientStrength를 0.5~1.0 정도로 높게 설정.
밤에는 0.1~0.3처럼 낮게 설정.

*현실적인 반응을 위한 방향성 고려
현재 수식은 모든 표면에 균일하게 적용되므로, 법선 방향(Normal)과 관계없이 주변광이 반영됩니다. 이를 보완하려면, 주변광 확산에 법선 방향성(dot(N, Up))을 반영할 수도 있습니다:
*/