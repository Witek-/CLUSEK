#include "EntityEditorWindow.h"
#include "../../ECS/Components/TransformComponent.h"

void EntityEditorWindow::Draw()
{
	ImGui::Begin("Entity Editor", &IsEnabled);

	if (*EntitySelected)
	{
		DrawComponentDetails<TransformComponent>(SelectedEntity, "Transform Component");
	}
	else
	{
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x / 2.0f) - 117.0f);
		ImGui::Text("Please select entity to enable this editor.");
	}

	ImGui::End();
}