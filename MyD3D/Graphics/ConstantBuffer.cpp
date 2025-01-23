#include "pch.h"
#include "ConstantBuffer.h"

void MaterialCBuffer::SetMaterial(MaterialResource* _pMaterial)
{
    if (_pMaterial)
    {
        MatProp = _pMaterial->mMaterialProperty;
        for (int i = 0; i < MATERIAL_MAP_SIZE; ++i)
        {
            bool hasMap = _pMaterial->mMaterialMapTexture[i];
            SetUsingMap((eMaterialMapType)i, hasMap);
        }
    }
   
}

BOOL MaterialCBuffer::GetUsingMap(eMaterialMapType _type)
{
    if ((UseMapFlag & (1 << (UINT)_type)) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void MaterialCBuffer::SetUsingMap(eMaterialMapType _type, BOOL _bValue)
{
    if (_bValue == TRUE)
    {
        // _type에 해당하는 비트를 활성화
        UseMapFlag |= (1 << (UINT)_type);
    }
    else
    {
        // _type에 해당하는 비트를 비활성화
        UseMapFlag &= ~(1 << (UINT)_type);
    }
}

void MaterialCBuffer::ToggleUsingMap(eMaterialMapType _type)
{
    // _type에 해당하는 비트를 토글(켜져 있으면 끄고, 꺼져 있으면 켜기)
    UseMapFlag ^= (1 << (UINT)_type);
}

BOOL LightCBuffer::GetLightFlag(UINT _flag)
{
    if ((LightFlags & (1 << _flag)) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void LightCBuffer::SetLightFlag(UINT _flag, BOOL _bValue)
{
    if (_bValue == TRUE)
    {
        // _type에 해당하는 비트를 활성화
        LightFlags |= (1 << _flag);
    }
    else
    {
        // _type에 해당하는 비트를 비활성화
        LightFlags &= ~(1 << _flag);
    }
}

BOOL LightCBuffer::GetShadowFlag(UINT _flag)
{
    if ((ShadowFlags & (1 << _flag)) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void LightCBuffer::SetShadowFlag(UINT _flag, BOOL _bValue)
{
    if (_bValue == TRUE)
    {
        // _type에 해당하는 비트를 활성화
        ShadowFlags |= (1 << _flag);
    }
    else
    {
        // _type에 해당하는 비트를 비활성화
        ShadowFlags &= ~(1 << _flag);
    }
}
