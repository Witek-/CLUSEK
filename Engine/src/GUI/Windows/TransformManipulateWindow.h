#pragma once
#include "BaseWindow.h"
#include <imgui.h>
#include <ImGuizmo.h>

class TransformManipulateWindow final : public BaseWindow
{
protected:
	void Draw() override;
private:
	void DrawCombo();
	void DrawDetails();

	void ListComponent(std::string& list);

	ImGuizmo::OPERATION CurrentGizmoOperation = ImGuizmo::TRANSLATE;

	int SelectedId = -1;
	DirectX::XMMATRIX * CurrentWorldMatrix = nullptr;
};