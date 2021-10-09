#pragma once

#include "Thunder/Core/Core.h"

#ifdef TD_PLATFORM_WINDOWS

extern Thunder::Application* Thunder::CreateApplication();

int main(int argc, char** argv)
{
	TD_BEGIN_PROFILING_SESSION("Thunder-Startup");
	Thunder::Log::Init();
	Thunder::Application* app = Thunder::CreateApplication();
	TD_END_PROFILING_SESSION();

	TD_BEGIN_PROFILING_SESSION("Thunder-Run");
	app->Run();
	Thunder::Instrumentor::EndSession();
	TD_END_PROFILING_SESSION();

	TD_BEGIN_PROFILING_SESSION("Thunder-Shutdown");
	delete app;
	TD_END_PROFILING_SESSION();
}

#endif