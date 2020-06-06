#pragma once
#include <vector>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "../Utils/SystemHolder.h"
#include <d3d11.h>
#include "Windows/BaseWindow.h"
#include "Windows/SystemsManagerWindow.h"
#include "Windows/FpsTimerWindow.h"
#include "Windows/LightingWindow.h"
#include "Windows/BackBufferWindow.h"
#include "Windows/KeyboardInputWindow.h"
#include "Windows/MouseInputWindow.h"
#include "Windows/GamePadWindow.h"
#include "Windows/CameraSpeedWindow.h"
#include "Windows/PostProcessingWindow.h"
#include "Windows/VehicleDetailsWindow.h"
#include <entt.hpp>
#include "Windows/PhysicsStatisticsWindow.h"
#include "Windows/EntityViewerWindow.h"
#include "Windows/PerSystemPerformanceWindow.h"
#include "Windows/ShadowPreviewerWindow.h"
#include "../ECS/Systems/RenderSystem.h"
#include "Windows/ConfigurationWindow.h"
#include "Windows/TerrainTessellationWindow.h"
#include "Windows/FogWindow.h"
#include "Windows/TextureCompressionWindow.h"
#include "Windows/LodDebugWindow.h"
#include "Windows/EntityEditorWindow.h"

#define UPDATE_USER_INTERFACE(WINDOW_INSTANCE) WINDOW_INSTANCE.Update(deltaTime, configData, dynamicRenderSettings, systems, renderSystemId, ioData, postProcessingSettings, registry, &SelectedEntity, &EntitySelected)

class DebugUserInterface final
{
public:
	void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, const ConfigData *configData,
		const std::function<void()> &functionCloseEngine);
	void Update(float deltaTime, ConfigData *configData, IOData *ioData, 
		std::vector<SystemHolder> *systems, int renderSystemId, DynamicRenderSettings *dynamicRenderSettings, 
		PostProcessingSettings *postProcessingSettings, entt::registry *registry);

	~DebugUserInterface();
private:
	void SetupStyle(const ConfigData *configData) const;

	void HandleKeyboardEvents(const IOData *ioData);
	void HandleMainDockingArea();

	void BeforeUpdate() const;
	void AfterUpdate(RenderSystem *renderSystem) const;

	void DrawMenuBar();

	void DrawMenuItemForWindow(const std::string& label, BaseWindow& window) const;
	void DrawMenuItemForFunction(const std::string& label, const std::function<void()>& function) const;

	void HandleClickingOnObjects(IOData *ioData, entt::registry *registry, ConfigData *configData);

	CameraComponent& GetMainCamera(entt::registry *registry) const;
	TransformComponent& GetMainCameraTransform(entt::registry *registry) const;

	inline const static std::string DOCKSPACE_AREA_NAME = "MainDockspaceArea";

	bool IsEnabled = false;
	bool IsDockingEnabled = false;

	// Always set `EntitySelected` to true, when changing this value
	entt::entity SelectedEntity = 0;
	bool EntitySelected = false;

	std::function<void()> FunctionCloseEngine;

	ConfigurationWindow ConfigurationWindowInstance;
	CameraSpeedWindow CameraSpeedWindowInstance;
	SystemsManagerWindow SystemsManagerWindowInstance;
	FpsTimerWindow FpsTimerWindowInstance;
	LightingWindow LightingWindowInstance;
	BackBufferWindow BackBufferWindowInstance;
	MouseInputWindow MouseInputWindowInstance;
	KeyboardInputWindow KeyboardInputWindowInstance;
	GamePadInputWindow GamePadInputWindowInstance;
	PostProcessingWindow PostProcessingWindowInstance;
	VehicleDetailsWindow VehicleDetailsWindowInstance;
	PhysicsStatisticsWindow PhysicsStatisticsWindowInstance;
	EntityViewerWindow EntityViewerWindowInstance;
	PerSystemPerformanceWindow PerSystemPerformanceWindowInstance;
	ShadowPreviewerWindow ShadowPreviewerWindowInstance;
	TerrainTessellationWindow TerrainTessellationWindowInstance;
	FogWindow FogWindowInstance;
	TextureCompressionWindow TextureCompressionWindowInstance;
	LodDebugWindow LodDebugWindowInstance;
	EntityEditorWindow EntityEditorWindowInstance;
};
