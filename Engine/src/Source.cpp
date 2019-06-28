#include <Windows.h>
#include "Utils/SystemInfoUtil.h"
#include "Utils/Logger.h"
#include "Engine.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")

// Entry point for the Engine
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	Logger::Initialize();
	Logger::Debug("Initialized logger system.");
	SystemInfoUtil::LogInfo();

	Engine engine;
	engine.Initialize(hInstance);

	while (engine.CanUpdate())
	{
		engine.Update();
	}

	Logger::Debug("Preparing logger to free resources.");
	Logger::Free();

	return 0;
}