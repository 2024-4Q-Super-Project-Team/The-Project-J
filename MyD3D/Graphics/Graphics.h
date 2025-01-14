#pragma once

struct GraphicsVertex2D
{
    float Position[4]; // NDC ÁÂÇ¥
    float TexCoord[2]; // ÅØ½ºÃ³ ÁÂÇ¥
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