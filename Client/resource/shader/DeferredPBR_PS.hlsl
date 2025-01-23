#include "Shared.fxh"
#include "Math.fxh"


float4 main(QUAD_VS_OUTPUT input) : SV_TARGET
{
    float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // ������ ���ø��ϸ� ��δϱ� �ѹ� ���صΰ� ������ �ʱ�ȭ
    float4 SampleAlbedo         = GBuffer_Albedo.Sample(LinearWrapSampler, input.uv);
    float4 SampleNormal         = GBuffer_Normal.Sample(LinearWrapSampler, input.uv);
    float4 SampleMaterial       = GBuffer_Material.Sample(LinearWrapSampler, input.uv);
    float4 SampleEmessive       = GBuffer_Emessive.Sample(LinearWrapSampler, input.uv);
    float4 SampleWorld          = GBuffer_WorldPos.Sample(LinearWrapSampler, input.uv);
    
    float3  Albedo              = SampleAlbedo.rgb;
    float   Opacity             = SampleAlbedo.a;
    float3  Normal              = SampleNormal.rgb;
    float   Depth               = SampleNormal.a;
    float   Metalness           = SampleMaterial.r;
    float   Specular            = SampleMaterial.g;
    float   Roughness           = SampleMaterial.b;
    float   AmbientOcclusion    = SampleMaterial.a;
    float3  Emessive            = SampleEmessive.rgb;
    
    if(Depth >= 1.0f)
        discard;
    if (Opacity < 0.01)
        discard;
    
    float4 WorldPosition = SampleWorld;
    // ���� ReconstructWorldPosition�Լ��� World��ȯ �� ������ ����. �ӽ÷� World���ø� ������ ���
    //float3 WorldPosition = ReconstructWorldPosition(input.uv, Depth, ivCameraView, ivCameraProjection);
    float3 V = normalize(CameraPosition.xyz - WorldPosition.xyz); // ViewDirection (World To Eye)
    float  NdotV = saturate(dot(Normal, V));
    float atten = 1.0f; // ���� ��
    float3 L = float3(0.f, 0.f, 0.f);
    
    // =====    ���� ���   =====
    float4 DirectLight  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 AmbientLight = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 F0 = lerp(Fdielectric, Albedo, Metalness);
    float3 VrefN = reflect(-V, Normal); // �ݻ� ����
    
    // ===== ���� ���� ��� =====
    [unroll]
    for (int i = 0; i < NumLights; i++)
    {
        if (LightProp[i].LightType == 0)
        {
            L = -normalize(LightProp[i].Direction.xyz);
        }
        else if (LightProp[i].LightType == 1)
        {
            L = WorldPosition.xyz - LightProp[i].Position.xyz;
            atten = CaclulateAttenuation(L, LightProp[i].LightRadius, LightProp[i].LightCutOff);
            L = normalize(-L);
        }
        else if (LightProp[i].LightType == 2)
        {
            // L = -normalize(LightProp[i].Direction.xyz);
            // CaclulateSpotLight(L,);
        }
        // HalfVector
        float3 H    = normalize(L + V);
        float NdotL = saturate(dot(Normal, L));
        float HdotN = saturate(dot(H, Normal));
        float HdotV = saturate(dot(H, V));
         // Fresnel
        float3 F = FresnelEquation(F0, HdotV);
         // Microfacet Distribution (GGX)
        float  D = NormalDistributionFunction(Roughness, HdotN);
        // Geometry (Smith)
        float G = GeometryFunction(Roughness, NdotL, NdotV);
        
        float3 DifuuseBRDF  = CalcuateDiffuseBRDF(F, Albedo, Metalness);
        float3 SpecularBRDF = CalcuateSpecularBRDF(D, F, G, NdotL, NdotV);
        
        float ShadowScale = CaclulateShadowScale(i, WorldPosition);
        
        DirectLight.rgb += (DifuuseBRDF + SpecularBRDF) * atten * LightProp[i].Radiance.rgb * LightProp[i].LightStrengh * NdotL * ShadowScale;
        ACESToneMapping(DirectLight.rgb);
        // �̰� �������� ���ؼ� �������� ������?
        // ���� ����Ʈ ���� �����־� �ڿ� ���� �׸��ڰ� ������ �ȵȴ�.
        //DirectLight.rgb *= ;
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
        float3 Irradiance = IBLDiffuseMap.Sample(LinearWrapSampler, Normal).rgb;
        float3 ReflectedLighColor = IBLSpecularMap.SampleLevel(LinearWrapSampler, VrefN, Roughness * SpecularTexLevels).rgb;
        float2 SpecularBRDF = BRDFLookUpTable.Sample(LinearClampSampler, float2(NdotV, Roughness)).rg;
        
        float3 DiffuseIBL = CalcuateDiffuseIBL(F, Albedo, Metalness, Irradiance);
        float3 SpecularIBL = CalcuateSpecularIBL(F0, SpecularBRDF, ReflectedLighColor);
    
        AmbientLight.rgb += (DiffuseIBL + SpecularIBL) * AmbientOcclusion * AmbientIntensity;
    }
    // �������� ���� ������ Emissive ���� ���Ѵ�.
    FinalColor.rgb += Emessive.rgb;
    FinalColor.rgb += DirectLight.rgb;
    FinalColor.rgb += AmbientLight.rgb;
    // ���� ���ڵ� �۾��� ���ش�.
    FinalColor.rgb  = pow(FinalColor.rgb, 1.0 / GAMMA);
    FinalColor.a = Opacity;
    return saturate(FinalColor);
}

