#include "pch.h"
#include "LowerPlatformCollisionScript.h"
#include "LowerPlatformButtonScript.h"

void LowerPlatformCollisionScript::Start()
{
   
}

void LowerPlatformCollisionScript::Update()
{
}

void _CALLBACK LowerPlatformCollisionScript::OnCollisionEnter(Rigidbody* _origin, Rigidbody* _destination)
{
    mOwnerScript->OnCollisionEnterCallback(_origin, _destination);
    return void _CALLBACK();
}