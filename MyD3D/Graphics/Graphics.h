#pragma once

struct BackBufferVertex
{
    float Position[4]; // NDC ��ǥ
    float TexCoord[2]; // �ؽ�ó ��ǥ
};

enum class eShaderStage
{
    PASS,   // PASS
    VS,		// Vertex Shader
    PS,		// Pixel Shader
    GS,		// Geometry Shader
    HS,		// Hull Shader
    DS,		// Domain Shader
    CS,		// Compute Shader
    ALL,
};