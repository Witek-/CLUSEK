#include "DebugUserInterface.h"
#include "../Utils/Logger.h"
#include "Windows/SystemsManagerWindow.h"
#include "Windows/FpsTimerWindow.h"
#include "Windows/LightingWindow.h"

void DebugUserInterface::Initialize(const HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
	const std::function<void()> &functionCloseEngine)
{
	Logger::Debug("Preparing to initialize ImGui...");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);
	ImGui::StyleColorsDark();

	FunctionCloseEngine = functionCloseEngine;
}

void DebugUserInterface::BeforeUpdate() const
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugUserInterface::Update(const float deltaTime, const IOData& ioData, std::vector<SystemHolder>& systems, LightSettings& lightSettings)
{
	BeforeUpdate();

	HandleKeyboardEvents(ioData);
	if (!IsEnabled)
	{
		AfterUpdate();
		return;
	}

	HandleMainDockingArea();

	if (SystemsManagerWindow::IsEnabled)
		SystemsManagerWindow::Draw(systems);
	if (FpsTimerWindow::IsEnabled)
		FpsTimerWindow::Draw(deltaTime);
	if (LightingWindow::IsEnabled)
		LightingWindow::Draw(lightSettings);

	AfterUpdate();
}

void DebugUserInterface::AfterUpdate() const
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void DebugUserInterface::DrawMenuBar() const
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				FunctionCloseEngine();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Managers"))
		{
			if (ImGui::MenuItem("Systems manager", nullptr, SystemsManagerWindow::IsEnabled))
			{
				SystemsManagerWindow::IsEnabled = !SystemsManagerWindow::IsEnabled;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Renderer"))
		{
			if (ImGui::MenuItem("Lighting settings", nullptr, LightingWindow::IsEnabled))
			{
				LightingWindow::IsEnabled = !LightingWindow::IsEnabled;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Performance"))
		{
			if (ImGui::MenuItem("FPS Timer", nullptr, FpsTimerWindow::IsEnabled))
			{
				FpsTimerWindow::IsEnabled = !FpsTimerWindow::IsEnabled;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

DebugUserInterface::~DebugUserInterface()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUserInterface::HandleKeyboardEvents(const IOData& ioData)
{
	if (ioData.KeyboardTracker.released.OemTilde)
		IsEnabled = !IsEnabled;
}

void DebugUserInterface::HandleMainDockingArea()
{
	auto dockingEnabled = true;
	const auto windowFlags =	ImGuiWindowFlags_MenuBar | 
								ImGuiWindowFlags_NoDocking | 
								ImGuiWindowFlags_NoTitleBar | 
								ImGuiWindowFlags_NoCollapse | 
								ImGuiWindowFlags_NoResize | 
								ImGuiWindowFlags_NoMove | 
								ImGuiWindowFlags_NoBringToFrontOnFocus | 
								ImGuiWindowFlags_NoNavFocus |
								ImGuiWindowFlags_NoBackground;

	const auto dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode | 
								ImGuiDockNodeFlags_NoDockingInCentralNode;

	const auto viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("MainDockspaceArea", &IsDockingEnabled, windowFlags);
	ImGui::PopStyleVar(3);

	DrawMenuBar();

	const auto mainDockspaceId = ImGui::GetID("MainDockspaceArea");
	ImGui::DockSpace(mainDockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);

	ImGui::End();
}