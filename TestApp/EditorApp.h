#pragma once
#include "../MyEngineDX/Application/Application.h"

class EditorApp : public Engine::Application
{
public:
	static EditorApp* Instance;
	using Application::Application;
protected:
	virtual BOOL OnPreInitialize() override final;
	virtual BOOL OnPostInitialize() override final;
	virtual void OnPreFinalization() override final;
	virtual void OnPostFinalization() override final;
};