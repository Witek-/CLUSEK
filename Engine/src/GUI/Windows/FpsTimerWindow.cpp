#include "FpsTimerWindow.h"
#include <imgui.h>
#include <algorithm>

void FpsTimerWindow::Draw(const float deltaTime)
{
	Calculate(deltaTime);

	const auto currentFps = HistoricalFps[IM_ARRAYSIZE(HistoricalFps) - 1];
	const auto maxFps = std::max_element(std::begin(HistoricalFps), std::end(HistoricalFps));
	const auto minFps = std::min_element(std::begin(HistoricalFps), std::end(HistoricalFps));

	ImGui::Begin("FPS Timer", &IsEnabled);
	ImGui::Text("Current: %.0f fps (%.2f m/s)", currentFps, 1000.0f / currentFps);
	ImGui::Text("Minimum: %.0f fps (%.2f m/s)", *minFps, 1000.0f / *minFps);
	ImGui::Text("Maximum: %.0f fps (%.2f m/s)", *maxFps, 1000.0f / *maxFps);
	ImGui::Separator();
	ImGui::PlotLines("", HistoricalFps, IM_ARRAYSIZE(HistoricalFps), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(200, 50));
	ImGui::End();
}

void FpsTimerWindow::Calculate(const float deltaTime)
{
	FramesCounter += 1;
	TimeCounter += deltaTime;

	if (TimeCounter > 1000.0f)
	{
		AddNewFps(FramesCounter);
		FramesCounter = 0;
		TimeCounter = 0.0f;
	}
}

void FpsTimerWindow::AddNewFps(const int fps)
{
	for (auto i = 0; i < IM_ARRAYSIZE(HistoricalFps) - 1; i++)
	{
		HistoricalFps[i] = HistoricalFps[i + 1];
	}

	HistoricalFps[IM_ARRAYSIZE(HistoricalFps) - 1] = fps;
}