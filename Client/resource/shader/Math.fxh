
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
float3 FresnelEquation(float3 F0, float HdotV)
{
    return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}

// 그림자 스케일 계산 (0이 그림자가 있는거다)
float CaclulateShadowScale(int Index, float4 WorldPosition)
{
    if (LightProp[Index].UseShadow == FALSE)
    {
        return 1;
    }
    float ShadowScale = 0;
    float4 ShadowPositon = float4(1.0f, 1.0f, 1.0f, 1.0f);
    ShadowPositon = mul(WorldPosition, LightProp[Index].ShadowViewMatrix);
    ShadowPositon = mul(ShadowPositon, LightProp[Index].ShadowProjectionMatrix);
    float CurrShadowDepth = ShadowPositon.z / ShadowPositon.w;
    float2 UV = ShadowPositon.xy / ShadowPositon.w;
    UV.y = -UV.y; // y축 반전
    UV = UV * 0.5 + 0.5; // (~1 ~ 1)의 값을 (0 ~ 1)으로 변환
    
    // 0 ~ 1 사이값이 아닐 시 처리하지 않음
    if (UV.x >= 0 && UV.x <= 1 && UV.y >= 0 && UV.y <= 1)
    {
        // uSE tcp
        if (true)
        {
            float2 offset[9] =
            {
                float2(-1, -1), float2(0, -1), float2(1, -1),
                float2(-1, 0), float2(0, 0), float2(1, 0),
                float2(-1, 1), float2(0, 1), float2(1, 1)
            };
            float TexelSize = 1.0 / SHADOW_SRV_SIZE; //텍셀 크기
            ShadowScale = 0;
            for (int i = 0; i < 9; ++i)
            {
                float2 SampleUV = UV + offset[i] * TexelSize;
                float SampleShadow = ShadowTexture[Index].SampleCmpLevelZero(LinearComparisonSampler, SampleUV, CurrShadowDepth - Epsilon);
                ShadowScale += SampleShadow;
            }
            // 평균 값을 구한다
            ShadowScale /= 9.0f;
        }
        else
        {
            // ShadowSRV에 저장한 Depth값 샘플링
            float SampleShadowDepth = ShadowTexture[SHADOW_SRV_OFFSET + Index].Sample(LinearWrapSampler, UV).r;
            if (CurrShadowDepth >= SampleShadowDepth + Epsilon)
            {
                ShadowScale = 0;
            }
            else
            {
                ShadowScale = 1;
            }
        }
    }
    return saturate(ShadowScale);
}

float3 ReconstructWorldPosition(float2 UV, float Depth, matrix inverseProjection, matrix inverseView)
{
    // [-1, 1] 범위의 클립 공간 변환
    float4 ClipSpacePos = float4(UV * 2.0f - 1.0f, Depth, 1.0f);
    ClipSpacePos.y = -ClipSpacePos.y;
    // 클립 공간 → 카메라 공간
    float4 CameraSpacePos = mul(ClipSpacePos, inverseProjection);
    // 카메라 공간 → 월드 공간
    float4 WorldSpacePos = mul(CameraSpacePos, inverseView);
    // 정규화
    WorldSpacePos /= WorldSpacePos.w;
    
    return WorldSpacePos;
}

float3 ACESToneMapping(float3 accumulatedLight)
{
    float3 mappedColor = (accumulatedLight * (2.51 * accumulatedLight + 0.03)) /
                   (accumulatedLight * (2.43 * accumulatedLight + 0.59) + 0.14);
    return mappedColor;

}

float CaclulateAttenuation(float3 lightDir, float radius, float cutoff = 0.001f)
{
    float r = radius; // 광원의 반지름
    
    // 사이 거리
    float distance = abs(length(lightDir));
     
    // 사이거리 - 광원의 반지름을 구하여 진짜 거리 계산
    float d = max(distance - r , 0); // 거리가 음수면 0으로 설정
    
    // 감쇠 계산을 위한 중간 값 설정 (거리 / 광원의 반지름 + 1)
    float denom = (d / r) + 1;
    
    // 감쇠 강도 계산 (거리가 크면 감쇠되고 반지름이 크면 빛을 많이 받음)
    float atten = 1 / (denom * denom);
    
    // 감쇠 스케일 재조정 임계값
    // att가 0일때는 광원과 가장 멈
    // att가 1일 때는 광원 중심에 가장 가까움
    atten = (atten - cutoff) / (1 - cutoff);
    
    
    // att가 음수가 나오지 않도록 조정
    atten = saturate(atten);
    
    return atten;
}

float CaclulateSpotLight(float3 lightDir)
{
    return float3(1.f, 1.f, 1.f);

}