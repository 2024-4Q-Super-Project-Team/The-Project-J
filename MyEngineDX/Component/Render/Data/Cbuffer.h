#pragma once

using RGBA = Vector4;

struct CTransformBuffer
{

};

struct CMaterialBuffer
{
	RGBA	DiffuseRGB = { 0.0f,0.0f,0.0f,1.0f };
	RGBA	AmbientRGB = { 0.0f,0.0f,0.0f,1.0f };
	RGBA	SpecularRGB = { 0.0f,0.0f,0.0f,1.0f };
	FLOAT	SpecularPower = 0;
	BOOL	UseMap[static_cast<UINT>(Graphics::eMaterialMapType::SIZE)] = {TRUE,};
	Vector3 Padding;
};

struct CCameraBuffer
{

};

struct CBonePaletteBuffer
{

};