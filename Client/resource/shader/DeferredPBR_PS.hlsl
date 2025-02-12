#include "Shared.fxh"
#include "Math.fxh"


float4 main(QUAD_VS_OUTPUT input) : SV_TARGET
{
    float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // 여러번 샘플링하면 비싸니까 한번 구해두고 나눠서 초기화
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
    // 현재 ReconstructWorldPosition함수로 World변환 시 문제가 있음. 임시로 World샘플링 값으로 사용
    //float3 WorldPosition = ReconstructWorldPosition(input.uv, Depth, ivCameraView, ivCameraProjection);
    float3 V = normalize(CameraPosition.xyz - WorldPosition.xyz); // ViewDirection (World To Eye)
    float  NdotV = saturate(dot(Normal, V));
    float atten = 1.0f; // 감쇠 값
    float3 L = float3(0.f, 0.f, 0.f);
    
    // =====    조명 계산   =====
    float4 DirectLight  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 AmbientLight = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 F0 = lerp(Fdielectric, Albedo, Metalness);
    float3 VrefN = reflect(-V, Normal); // 반사 벡터
    
    // ===== 직접 조명 계산 =====
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
        // 이걸 마지막에 곱해서 대입하지 않으면?
        // 이전 라이트 값이 남아있어 뒤에 조명 그림자가 적용이 안된다.
        //DirectLight.rgb *= ;
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
        float3 Irradiance = IBLDiffuseMap.Sample(LinearWrapSampler, Normal).rgb;
        float3 ReflectedLighColor = IBLSpecularMap.SampleLevel(LinearWrapSampler, VrefN, Roughness * SpecularTexLevels).rgb;
        float2 SpecularBRDF = BRDFLookUpTable.Sample(LinearClampSampler, float2(NdotV, Roughness)).rg;
        
        float3 DiffuseIBL = CalcuateDiffuseIBL(F, Albedo, Metalness, Irradiance);
        float3 SpecularIBL = CalcuateSpecularIBL(F0, SpecularBRDF, ReflectedLighColor);
    
        AmbientLight.rgb += (DiffuseIBL + SpecularIBL) * AmbientOcclusion * AmbientIntensity;
    }
    // 최종색상에 계산된 조명값과 Emissive 등을 합한다.
    FinalColor.rgb += Emessive.rgb;
    FinalColor.rgb += DirectLight.rgb;
    FinalColor.rgb += AmbientLight.rgb;
    // 감마 인코드 작업을 해준다.
    FinalColor.rgb  = pow(FinalColor.rgb, 1.0 / GAMMA);
    FinalColor.a = Opacity;
    return saturate(FinalColor);
}

