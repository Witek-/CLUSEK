#include "Engine.h"
#include "Utils/Logger.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Components/ModelRenderComponent.h"
#include "Loaders/MapLoader.h"
#include "ECS/Systems/SkyboxSystem.h"
#include "ECS/Systems/PhysicsSystem.h"
#include "ECS/Systems/TransformSystem.h"
#include "ECS/Systems/VehiclePlayerControllerSystem.h"

bool Engine::Initialize(const HINSTANCE hInstance, const ConfigData& configData)
{
	Logger::Debug("Engine initialization...");

	Timer engineInitializationTimer;
	engineInitializationTimer.Restart();

	const auto hr = CoInitialize(nullptr);
	if (FAILED(hr))
	{
		Logger::Error("Error when initializing COM libs.");
		return false;
	}

	Config = configData;

	if (!Window.Initialize(hInstance, Config.WindowTitle, Config.WindowClassName, Config.WindowWidth, Config.WindowHeight))
		return false;

	InitializeScene();
	CreateSystems();
	InitializeSystems();
	InitializeUserInterface();

	Logger::Debug("Resetting timer...");
	Time.Restart();

	const auto engineInitializationTime = static_cast<int>(ceil(engineInitializationTimer.GetDeltaTimeAndRestart()));
	Logger::Debug("Engine initialization took: " + std::to_string(engineInitializationTime) + " ms.");

	return true;
}

bool Engine::CanUpdate()
{
	return Window.ProcessMessages();
}

void Engine::Update()
{
	const auto deltaTime = Time.GetDeltaTimeAndRestart();
	const auto renderSystem = dynamic_cast<RenderSystem*>(Systems[RenderSystemId].System.get());

	UpdateInputOutputDevices();
	renderSystem->RenderFrameBegin();
	UpdateSystems(deltaTime);

	DebugUI.Update(	deltaTime, 
					&Config, 
					&DataFromIODevices, 
					&Systems, 
					RenderSystemId, 
					&CurrentRenderSettings, 
					&CurrentPostProcessingSettings, 
					&Registry);

	renderSystem->RenderFrameEnd();
	HandleClosingWithButton();
}

void Engine::InitializeScene()
{
	MapLoader::CreateEntitiesFromMapFile(Config.PathToMap, Registry);
}

void Engine::CreateSystems()
{
	std::unique_ptr<BaseSystem> systemBasePtr;

	// Render system should be first
	systemBasePtr = std::make_unique<RenderSystem>();
	Systems.emplace_back(SystemHolder("Render System", systemBasePtr, true));

	systemBasePtr = std::make_unique<PhysicsSystem>();
	Systems.emplace_back(SystemHolder("Physics System", systemBasePtr, true));

	systemBasePtr = std::make_unique<CameraSystem>();
	Systems.emplace_back(SystemHolder("Camera System", systemBasePtr, true));

	systemBasePtr = std::make_unique<SkyboxSystem>();
	Systems.emplace_back(SystemHolder("Skybox System", systemBasePtr, true));

	systemBasePtr = std::make_unique<VehiclePlayerControllerSystem>();
	Systems.emplace_back(SystemHolder("Vehicle Player Controller System", systemBasePtr, true));

	// Transform system should be last
	systemBasePtr = std::make_unique<TransformSystem>();
	Systems.emplace_back(SystemHolder("Transform System", systemBasePtr, true));

	for (auto i = 0; i < Systems.size(); i++)
	{
		if (Systems[i].Name == "Render System")
		{
			RenderSystemId = i;
			return;
		}
	}

	Logger::Error("Render system not registered as system!");
}

void Engine::InitializeSystems()
{
	for (auto& system : Systems)
	{
		system.System->Initialize(&Registry, &Window, &Config, &CurrentRenderSettings, 
			&DataFromIODevices, &InputOutputDevices, &CurrentPostProcessingSettings);
		system.System->Start();
	}
}

void Engine::InitializeUserInterface()
{
	const auto renderSystem = dynamic_cast<RenderSystem*>(Systems[RenderSystemId].System.get());
	DebugUI.Initialize(Window.GetHandle(), renderSystem->GetPointerToDevice(), renderSystem->GetPointerToDeviceContext(), &Config, [this]()
	{
		Window.UserRequestedQuit();
	});
}

void Engine::UpdateInputOutputDevices()
{
	InputOutputDevices.Update();
	DataFromIODevices = InputOutputDevices.Get();
}

void Engine::UpdateSystems(const float deltaTime)
{
	for (auto& system : Systems)
	{
		if (!system.Enabled)
			continue;

		system.System->Update(deltaTime);
	}
}

void Engine::HandleClosingWithButton() const
{
	if (DataFromIODevices.KeyboardState.Escape)
	{
		Logger::Debug("Escape key pressed. Sending quit message...");
		Window.UserRequestedQuit();
	}
	if (DataFromIODevices.GamePadState.IsConnected() && DataFromIODevices.GamePadState.IsBackPressed())
	{
		Logger::Debug("Back button pressed. Sending quit message...");
		Window.UserRequestedQuit();
	}
}
