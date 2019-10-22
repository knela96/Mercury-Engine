#include "Application.h"
#include "WindowHierarchy.h"
#include "GameObject.h"

WindowHierarchy::WindowHierarchy(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowHierarchy::~WindowHierarchy()
{
}


bool WindowHierarchy::Start()
{
	return true;
}


bool WindowHierarchy::Draw()
{
	static int selection_mask = (1 << -1);
	int node_clicked = -1;

	if (App->gui->openHirearchy) {
		ImGui::Begin("Hierarchy",&App->gui->openHirearchy, ImGuiHoveredFlags_RootWindow);
		if (App->importer->gameObjects.size() > 0) {
			for (int i = 0; i < App->importer->gameObjects.size(); ++i) {

				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
				bool is_selected = (selection_mask & (1 << i)) != 0;
				if (is_selected)
					node_flags |= ImGuiTreeNodeFlags_Selected;


				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				GameObject* gameObject = App->importer->gameObjects[i];
				if (!gameObject->active)
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObject->name.c_str());

				if (!gameObject->active)
					ImGui::PopStyleVar();

				if (ImGui::IsItemClicked())
					node_clicked = i;
			}

			//Active Gameobject
			if(node_clicked != -1)
				App->gui->inspector->active_gameObject = App->importer->gameObjects[node_clicked];
			else if (node_clicked == -1 && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow)) {
				App->gui->inspector->active_gameObject = nullptr; //Deselect GameObject
				selection_mask = (1 << -1);
			}

			//Visually Select Node in Hirearchy
			if (node_clicked != -1)
			{
				if (ImGui::GetIO().KeyCtrl)
					selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
				else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
					selection_mask = (1 << node_clicked);           // Click to single-select
			}
		}
		ImGui::End();
	}
	return false;
}

bool WindowHierarchy::Cleanup()
{
	return true;
}

bool WindowHierarchy::isEnabled()
{
	return enabled;
}
