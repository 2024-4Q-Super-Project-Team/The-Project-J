#pragma once
#include "../TestApp.h"

class TestWorld
	: public DXWorld
{
public:
	using DXWorld::DXWorld;
public:
	virtual void _CALLBACK OnEnable() override;
	virtual void _CALLBACK OnDisable() override;
	virtual void _CALLBACK OnCreate() override;
	virtual void _CALLBACK OnDestroy() override;
	virtual void _CALLBACK OnLoad() override;
	virtual void _CALLBACK OnUnLoad() override;

	virtual void _CALLBACK OnFixedUpdate() override;
	virtual void _CALLBACK OnPreUpdate() override;
	virtual void _CALLBACK OnUpdate() override;
	virtual void _CALLBACK OnPostUpdate() override;
	virtual void _CALLBACK OnPreRender() override;
	virtual void _CALLBACK OnRender() override;
	virtual void _CALLBACK OnPostRender() override;
private:
	GameManager* mGMng;
};

