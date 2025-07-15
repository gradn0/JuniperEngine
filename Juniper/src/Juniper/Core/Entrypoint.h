#pragma once

#ifdef JP_PLATFORM_WINDOWS

#include "Application.h"
#include "Log.h"

extern Juniper::Application* Juniper::CreateApplication();

int main(int argc, char** argv)
{
	Juniper::Log::Init();

	auto app = Juniper::CreateApplication(); // Get handle to client's application
	app->Run();
	delete app;
}

#endif