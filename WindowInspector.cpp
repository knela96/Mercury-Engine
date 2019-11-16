#include "Application.h"
#include "WindowInspector.h"
#include "GameObject.h"
#include "MeshObject.h"
#include "Component.h"

WindowInspector::WindowInspector(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowInspector::~WindowInspector()
{
}


bool WindowInspector::Start()
{/*
	face_lenght = 1.0f;
	vertex_lenght = 1.0f;
	face_color = { 0.0f,1.0f,1.0f,1.0f };
	vertex_color = { 1.0f,0.0f,0.0f,1.0f };*/
	return true;
}

bool WindowInspector::Draw()
{
	if (App->gui->openInspector) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(1000, -1));
		ImGui::Begin("Inspector",&App->gui->openInspector);
		if (active_gameObject != nullptr) {
			bool aux = active_gameObject->active;
			ImGui::Checkbox("Active", &aux); ImGui::SameLine();

			if (active_gameObject->parent != nullptr) {
				if (active_gameObject->parent->active)//FIX
					active_gameObject->active = aux;
			}
			else {
				active_gameObject->active = aux;
			}

			if (ImGui::IsItemEdited()) {
				for (int i = 0; i < active_gameObject->childs.size(); ++i) {
					active_gameObject->childs[i]->active = active_gameObject->active;
					active_gameObject->childs[i]->UpdateChilds();
				}
			}
			ImGui::SetNextItemWidth(200);
			ImGui::InputText("Name", (char*)&active_gameObject->name, 20); 
			
			if (ImGui::IsItemActive()) {
				App->input->writting = true;
			}
			else {
				App->input->writting = false;
			}

			for (int i = 0; i < active_gameObject->components.size(); ++i) {
				active_gameObject->components[i]->Update();
			}

		}
		


		ImGui::End();
	}
	return false;
}

bool WindowInspector::Cleanup()
{
	active_gameObject = nullptr;
	return true;
}

bool WindowInspector::isEnabled()
{
	return enabled;
}
