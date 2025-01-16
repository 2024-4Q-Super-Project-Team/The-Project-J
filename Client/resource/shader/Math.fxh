
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
// ǥ���� ��ĥ�⿡ ���� Microfacet���� �󸶳�  ���Ϳ� ���ĵǴ��� ����մϴ�.
float NormalDistributionFunction(float roughness, float HdotN)
{
    float A = roughness * roughness;
    float A2 = A * A;
    float D = (HdotN * HdotN) * (A2 - 1.0) + 1.0;
    return A2 / (PI * (D * D));
}
// cook-torrance : G
// Microfacet���� ���� Occlusion���� Shadowing�� Masking�� �߻��� ǥ���� �ݻ��ϴ� ���� ���� �� �ֽ��ϴ�.
float GeometryFunction(float roughness, float NdotL, float NdotV)
{
    float A = roughness + 1.0;
    float K = (A * A) / 8.0f;
    float G1 = NdotL / (NdotL * (1.0 - K) + K);
    float G2 = NdotV / (NdotV * (1.0 - K) + K);
    return G1 * G2;
}

// cook-torrance : F
// ���� �ٸ� ������ ǥ�鿡�� �ݻ�Ǵ� ���� ������ ��Ÿ���ϴ�.
// metalness�� 1�� ����� ���� ��� �ؽ�ó�� �ݻ��� �⺻������ �ȴ�. 
float3 FresnelEquation(float3 F0, float HdotV)
{
    return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}

// �׸��� ������ ��� (0�� �׸��ڰ� �ִ°Ŵ�)
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
    UV.y = -UV.y; // y�� ����
    UV = UV * 0.5 + 0.5; // (~1 ~ 1)�� ���� (0 ~ 1)���� ��ȯ
    
    // 0 ~ 1 ���̰��� �ƴ� �� ó������ ����
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
            float TexelSize = 1.0 / SHADOW_SRV_SIZE; //�ؼ� ũ��
            ShadowScale = 0;
            for (int i = 0; i < 9; ++i)
            {
                float2 SampleUV = UV + offset[i] * TexelSize;
                float SampleShadow = ShadowTexture[Index].SampleCmpLevelZero(LinearComparisonSampler, SampleUV, CurrShadowDepth - Epsilon);
                ShadowScale += SampleShadow;
            }
            // ��� ���� ���Ѵ�
            ShadowScale /= 9.0f;
        }
        else
        {
            // ShadowSRV�� ������ Depth�� ���ø�
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
    // [-1, 1] ������ Ŭ�� ���� ��ȯ
    float4 ClipSpacePos = float4(UV * 2.0f - 1.0f, Depth, 1.0f);
    ClipSpacePos.y = -ClipSpacePos.y;
    // Ŭ�� ���� �� ī�޶� ����
    float4 CameraSpacePos = mul(ClipSpacePos, inverseProjection);
    // ī�޶� ���� �� ���� ����
    float4 WorldSpacePos = mul(CameraSpacePos, inverseView);
    // ����ȭ
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
    float r = radius; // ������ ������
    
    // ���� �Ÿ�
    float distance = abs(length(lightDir));
     
    // ���̰Ÿ� - ������ �������� ���Ͽ� ��¥ �Ÿ� ���
    float d = max(distance - r , 0); // �Ÿ��� ������ 0���� ����
    
    // ���� ����� ���� �߰� �� ���� (�Ÿ� / ������ ������ + 1)
    float denom = (d / r) + 1;
    
    // ���� ���� ��� (�Ÿ��� ũ�� ����ǰ� �������� ũ�� ���� ���� ����)
    float atten = 1 / (denom * denom);
    
    // ���� ������ ������ �Ӱ谪
    // att�� 0�϶��� ������ ���� ��
    // att�� 1�� ���� ���� �߽ɿ� ���� �����
    atten = (atten - cutoff) / (1 - cutoff);
    
    
    // att�� ������ ������ �ʵ��� ����
    atten = saturate(atten);
    
    return atten;
}

float CaclulateSpotLight(float3 lightDir)
{
    return float3(1.f, 1.f, 1.f);

}