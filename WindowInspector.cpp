#include "Application.h"
#include "WindowInspector.h"
#include "GameObject.h"
#include "MeshObject.h"

WindowInspector::WindowInspector(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	enabled = true;
}


WindowInspector::~WindowInspector()
{
}


bool WindowInspector::Start()
{
	face_lenght = 1.0f;
	vertex_lenght = 1.0f;
	face_color = { 0.0f,1.0f,1.0f,1.0f };
	vertex_color = { 1.0f,0.0f,0.0f,1.0f };
	return true;
}

bool WindowInspector::Draw()
{
	if (App->gui->openInspector) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(1000, -1));
		ImGui::Begin("Inspector",&App->gui->openInspector);
		if (active_gameObject != nullptr) {

			ImGui::InputText("Name", (char*)&active_gameObject->name, 20); 

			if (ImGui::IsItemActive()) {
				App->input->writting = true;
			}
			else {
				App->input->writting = false;
			}

			ImGui::SameLine(); ImGui::Checkbox("Show", &active_gameObject->active);

			if (ImGui::CollapsingHeader("Mesh Info", open_mesh_info))
			{
				ImGui::Text("Vertices: %i", active_gameObject->mesh->vertices.size());
				ImGui::Text("Faces: %i", active_gameObject->mesh->indices.size() / 3);
			}


			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::Text("Position:");
				ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 240*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("X", &active_gameObject->position.x, 0.005f); ImGui::PopID();
				ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 160*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("Y", &active_gameObject->position.y, 0.005f); ImGui::PopID();
				ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 80*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("Z", &active_gameObject->position.z, 0.005f); ImGui::PopID();

				ImGui::Text("Rotation:");
				ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot"); ImGui::DragFloat("X", &active_gameObject->rotation.x, 0.005f); ImGui::PopID();
				ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot");  ImGui::DragFloat("Y", &active_gameObject->rotation.y, 0.005f); ImGui::PopID();
				ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot");  ImGui::DragFloat("Z", &active_gameObject->rotation.z, 0.005f); ImGui::PopID();

				ImGui::Text("Scale:   ");
				ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("X", &active_gameObject->scale.x, 0.005f); ImGui::PopID();
				ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("Y", &active_gameObject->scale.y, 0.005f); ImGui::PopID();
				ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("Z", &active_gameObject->scale.z, 0.005f); ImGui::PopID();
			}

			if (ImGui::CollapsingHeader("Draw Normals", open_normals))
			{
				static bool alpha_preview = true;
				static bool alpha_half_preview = false;
				static bool drag_and_drop = true;
				static bool options_menu = true;
				static bool hdr = false;

				ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
				if (ImGui::TreeNode("Face Normals:")) {
					int item = active_gameObject->face_normals;
					ImGui::Text("Draw:  "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("draw_faces"); ImGui::Combo("", &item, "Never\0Always\0"); ImGui::PopID();
					ImGui::Text("Length:");	ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("face_length"); ImGui::DragFloat(" ", &face_lenght, 0.005f, 0.0f, 5.0f); ImGui::PopID();
					ImGui::Text("Color: "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::ColorEdit4("Face Color", (float*)&face_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

					active_gameObject->face_normals = item;
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Vertex Normals:")) {
					int item = active_gameObject->vertex_normals;
					ImGui::Text("Draw:  "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("draw_vertex"); ImGui::Combo("", &item, "Never\0Always\0"); ImGui::PopID();
					ImGui::Text("Length:");	ImGui::SameLine(); ImGui::PushItemWidth(120);  ImGui::PushID("vertex_length"); ImGui::DragFloat(" ", &vertex_lenght, 0.005f, 0.0f, 5.0f); ImGui::PopID();
					ImGui::Text("Color:"); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::ColorEdit4("Vertex Color", (float*)&vertex_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
					active_gameObject->vertex_normals = item;
					ImGui::TreePop();
				}
			}

			if (ImGui::CollapsingHeader("Material", open_material))
			{
				for (int i = 0; i < active_gameObject->textures.size(); ++i) {
					ImGui::Columns(2, "teas", false);
					ImGui::Text("%s", active_gameObject->getType(active_gameObject->textures[i].type));
					ImGui::Text("%s", active_gameObject->textures[i].path.c_str()); ImGui::Text("");
					ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 80); ImGui::Text("%ix%i px ", (uint)active_gameObject->textures[i].size.x, (uint)active_gameObject->textures[i].size.y);
					ImGui::NextColumn();
					ImGui::Image((ImTextureID*)active_gameObject->textures[i].id, ImVec2(60, 60), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
				}
				ImGui::Columns(1);
			}

			if (!unFold) {
				ImGui::GetStateStorage()->SetInt(ImGui::GetID("Transform"), 1);
				ImGui::GetStateStorage()->SetInt(ImGui::GetID("Draw Normals"), 1);
				ImGui::GetStateStorage()->SetInt(ImGui::GetID("Face Normals:"), 1);
				ImGui::GetStateStorage()->SetInt(ImGui::GetID("Vertex Normals:"), 1);
				ImGui::GetStateStorage()->SetInt(ImGui::GetID("Material"), 1);
				unFold = true;
			}
		}
		

		ImGui::End();
	}
	return false;
}

bool WindowInspector::Cleanup()
{
	return true;
}

bool WindowInspector::isEnabled()
{
	return enabled;
}
