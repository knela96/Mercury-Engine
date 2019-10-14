#include "Application.h"
#include "WindowInspector.h"

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
	face_normals = false;
	vertex_normals = false;
	return true;
}

bool WindowInspector::Draw()
{
	if (App->gui->openInspector) {
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(1000, -1));
		ImGui::Begin("Inspector",&App->gui->openInspector);
		//ImGui::SetNextTreeNodeOpen(open_transform);
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::Text("Position:");
			ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 240*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("X", &pos.x, 0.005f); ImGui::PopID();
			ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 160*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("Y", &pos.y, 0.005f); ImGui::PopID();
			ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 80*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("Z", &pos.z, 0.005f); ImGui::PopID();

			ImGui::Text("Rotation:");
			ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot"); ImGui::DragFloat("X", &rot.x, 0.005f); ImGui::PopID();
			ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot");  ImGui::DragFloat("Y", &rot.y, 0.005f); ImGui::PopID();
			ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot");  ImGui::DragFloat("Z", &rot.z, 0.005f); ImGui::PopID();

			ImGui::Text("Scale:   ");
			ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("X", &scale.x, 0.005f); ImGui::PopID();
			ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("Y", &scale.y, 0.005f); ImGui::PopID();
			ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("Z", &scale.z, 0.005f); ImGui::PopID();

			//ImGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);

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
				static int item = 0;
				ImGui::Text("Draw:  "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("draw_faces"); ImGui::Combo("", &item, "Never\0Always\0"); ImGui::PopID();
				ImGui::Text("Length:");	ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("face_length"); ImGui::DragFloat(" ", &face_lenght, 0.005f, 0.0f, 5.0f); ImGui::PopID();
				ImGui::Text("Color: "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::ColorEdit4("Face Color", (float*)&face_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

				if (!item)
					face_normals = false;
				else
					face_normals = true;

				ImGui::TreePop();
			}


			if (ImGui::TreeNode("Vertex Normals:")) {
				static int item = 0;
				ImGui::Text("Draw:  "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("draw_vertex"); ImGui::Combo("", &item, "Never\0Always\0"); ImGui::PopID();
				ImGui::Text("Length:");	ImGui::SameLine(); ImGui::PushItemWidth(120);  ImGui::PushID("vertex_length"); ImGui::DragFloat(" ", &vertex_lenght, 0.005f, 0.0f, 5.0f); ImGui::PopID();
				ImGui::Text("Color:"); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::ColorEdit4("Vertex Color", (float*)&vertex_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
				if (!item)
					vertex_normals = false;
				else
					vertex_normals = true;
				ImGui::TreePop();
			}
		}

		if (!unFold) {
			ImGui::GetStateStorage()->SetInt(ImGui::GetID("Transform"), 1);
			ImGui::GetStateStorage()->SetInt(ImGui::GetID("Draw Normals"), 1);
			ImGui::GetStateStorage()->SetInt(ImGui::GetID("Face Normals:"), 1);
			ImGui::GetStateStorage()->SetInt(ImGui::GetID("Vertex Normals:"), 1);
			unFold = true;
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
