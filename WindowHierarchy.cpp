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

GameObject* WindowHierarchy::CreateHierarchy(GameObject* parent, int& node_clicked, int& index) {
	++index;
	GameObject* gameObject = App->gui->inspector->active_gameObject;

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth;
	
	if (parent == gameObject)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	if (parent->childs.size() == 0)
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	if (parent == nullptr)
		node_flags = ImGuiTreeNodeFlags_None;

	if (!parent->active)
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

	if (ImGui::TreeNodeEx((void*)(intptr_t)index, node_flags, parent->name.c_str())) {

		if (!parent->active)
			ImGui::PopStyleVar();

		if (ImGui::IsItemClicked()) {
			App->gui->inspector->active_gameObject = parent;
			node_clicked = index;
		}

		if (parent != App->scene_intro->root) {
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("GameObject", parent, sizeof(GameObject));    // Set payload to carry the index of our item (could be anything)
				ImGui::Text("%s", parent->name.c_str());
				dragGameObject = parent;
				ImGui::EndDragDropSource();
			}
		}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
				{
					App->scene_intro->setParent(parent, dragGameObject);
				}
				ImGui::EndDragDropTarget();
			}

		if (parent->childs.size() > 0) {
			for (int i = 0; i < parent->childs.size(); ++i) {
				CreateHierarchy(parent->childs[i], node_clicked, index);
			}
		}

		ImGui::TreePop();
	}
	return nullptr;
}

bool WindowHierarchy::Draw()
{
	static int selection_mask = (1 << -1);
	int node_clicked = -1;

	if (App->gui->openHirearchy) {
		ImGui::Begin("Hierarchy",&App->gui->openHirearchy, ImGuiHoveredFlags_RootWindow);
		if (App->scene_intro->root->childs.size() > 0) {
			
			int i = 0;
			CreateHierarchy(App->scene_intro->root, node_clicked, i);

			//Deactive Gameobject
			if (node_clicked == -1 && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow)) {
				App->gui->inspector->active_gameObject = nullptr; //Deselect GameObject
				selection_mask = (1 << -1);
			}

			//Visually Select Node in Hirearchy
			if (node_clicked != -1)
			{
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
