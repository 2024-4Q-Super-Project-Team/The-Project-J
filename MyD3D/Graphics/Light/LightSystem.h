#pragma once

#define NUM_MAX_LIGHTS 10
class LightSystem
{
public:
public:
    void Bind();
public:
    void AddLight(LightProperty& _lightProp);
private:
    LightCBuffer mLightCBuffer;
};