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

GameObject* WindowHierarchy::CreateHierarchy(GameObject* parent, int& node_clicked, int selection_mask, int& index, bool open) {
	++index;

	GameObject* gameObject = App->gui->inspector->active_gameObject;
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	
	bool is_selected = (selection_mask & (1 << index)) != 0;
	if (is_selected)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	for (int j = 0; j < parent->childs.size(); ++j) {
		bool node_open = false;
		GameObject* child = parent->childs[j];
		if (!child->active)
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

		if (child->childs.size() > 0)
			node_open = ImGui::TreeNode((void*)(intptr_t)index, child->name.c_str());
		else if(open)
			ImGui::TreeNodeEx((void*)(intptr_t)index, node_flags, child->name.c_str()); //change i by gameObject ID
		
		if (ImGui::IsItemClicked()) {
			gameObject = child;
			node_clicked = index + j;
		}

		if (!child->active)
			ImGui::PopStyleVar();

		if (node_open) {
			//node_clicked = index;
			GameObject* aux = CreateHierarchy(child, node_clicked, selection_mask, index, node_open);

			if (aux != nullptr)
				gameObject = aux;

			ImGui::TreePop();
		}
	}
	return gameObject;
}

bool WindowHierarchy::Draw()
{
	static int selection_mask = (1 << -1);
	int node_clicked = -1;

	if (App->gui->openHirearchy) {
		ImGui::Begin("Hierarchy",&App->gui->openHirearchy, ImGuiHoveredFlags_RootWindow);
		if (App->scene_intro->root->childs.size() > 0) {
			
			int i = 0; GameObject* child;
			if (ImGui::TreeNode((void*)(intptr_t)i, App->scene_intro->root->name.c_str())) {
				child = CreateHierarchy(App->scene_intro->root, node_clicked, selection_mask, i);
				ImGui::TreePop();
			}

			//Active Gameobject
			if (node_clicked != -1)
				App->gui->inspector->active_gameObject = child;
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
