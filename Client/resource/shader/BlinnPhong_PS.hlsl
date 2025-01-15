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
    // ���� ���
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
        // N��L * ���� Diff * ��Ƽ���� Diff * ��Ƽ���� Diff_Map(ǥ�� ����)
        DiffuseLight = NDotL * MaterialProp.DiffuseColor * pow(MapColor[DIFFUSE_MAP], GAMMA);
        // �ֺ��� Ȯ���� * ���� Ambi * ��Ƽ���� Ambi * ��Ƽ���� Ambi_Map
        AmbientLight = MaterialProp.AmbientColor * MapColor[AMBIENT_MAP];
        // ( H��N (or R��V) ^ ��Ƽ���� Shin(����) ) * ���� Spec * ��Ƽ���� Spec
        SpecularLight = pow(HDotN, MaterialProp.RoughnessScale) * MaterialProp.SpecularColor * MapColor[SPECULAR_MAP];
        // ��Ƽ���� Emissive_Map
        EmissiveLight = MapColor[EMISSIVE_MAP];
        FinalColor += (DiffuseLight + AmbientLight + SpecularLight + EmissiveLight);
    }
    
    FinalColor.a = MapColor[OPACITY_MAP].a; // ���İ� ����
    
    FinalColor.rgb = pow(FinalColor.rgb, 1.0 / GAMMA);
    return saturate(FinalColor); // ���� ���� ����
}

//float3 CalculateDirectionLight(LightProperty Light, MaterialProperty Material, float3 Normal, float3 View, float4 SpecMapColor)
//{
    
//}

/*
LightProp[i].AmbientStrength
�ֺ����� Ȯ�����̳� ������ �����ϴ� ������, �� ����Ʈ�� ȯ�濡 ���� ���� ����.
��: ���� �¾籤������ AmbientStrength�� ����, �ǳ� �����̳� ���� �������� ����.

LightProp[i].AmbientColor
����Ʈ�� ���� �ֺ��� ����.
��: ������ ���� ������ ��Ȳ��, �㿡�� ��ο� �Ķ��� �迭.

MaterialProp.AmbientColor
����(Material)�� �ֺ����� �󸶳� �����ϴ����� ��Ÿ���� ���(����).
��: �ݼ��� ���� ��, �ö�ƽ�̳� õ�� ���� ��.

MapColor[AMBIENT_MAP]
���� �ؽ�ó ������, ǥ�鸶�� �ٸ� �ֺ��� ������ ǥ���� �� ����.

AmbientStrength�� Ư�� ����Ʈ�� �ֺ��� ȿ���� �����ϹǷ�, ȯ�� ����� ��ȭ�Ӱ� �����ؾ� �մϴ�.

������ AmbientStrength�� 0.5~1.0 ������ ���� ����.
�㿡�� 0.1~0.3ó�� ���� ����.

*�������� ������ ���� ���⼺ ���
���� ������ ��� ǥ�鿡 �����ϰ� ����ǹǷ�, ���� ����(Normal)�� ������� �ֺ����� �ݿ��˴ϴ�. �̸� �����Ϸ���, �ֺ��� Ȯ�꿡 ���� ���⼺(dot(N, Up))�� �ݿ��� ���� �ֽ��ϴ�:
*/