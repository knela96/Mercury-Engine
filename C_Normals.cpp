#include "C_Normals.h"

C_Normals::C_Normals(GameObject* gameobject, Component_Type type) : Component(type, gameobject)
{
	name = "Normals";
	face_lenght = 1.0f;
	vertex_lenght = 1.0f;
	face_color = { 0.0f,1.0f,1.0f,1.0f };
	vertex_color = { 1.0f,0.0f,0.0f,1.0f };
	ID = App->RandomNumberGenerator.GetIntRNInRange();

}

C_Normals::~C_Normals()
{
}

bool C_Normals::Enable()
{
	active = true;
	return true;
}

void C_Normals::Update()
{
	ImGui::PushID("active_normal"); ImGui::Checkbox("", &active); ImGui::PopID(); ImGui::SameLine();
	if (ImGui::CollapsingHeader(name.c_str()))
	{
		static bool alpha_preview = true;
		static bool alpha_half_preview = false;
		static bool drag_and_drop = true;
		static bool options_menu = true;
		static bool hdr = false;

		ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
		if (ImGui::TreeNode("Face Normals:")) {
			int item = gameobject->face_normals;
			ImGui::Text("Draw:  "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("draw_faces"); ImGui::Combo("", &item, "Never\0Always\0"); ImGui::PopID();
			ImGui::Text("Length:");	ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("face_length"); ImGui::DragFloat(" ", &face_lenght, 0.005f, 0.0f, 5.0f); ImGui::PopID();
			ImGui::Text("Color: "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::ColorEdit4("Face Color", (float*)&face_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

			gameobject->face_normals = item;
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Vertex Normals:")) {
			int item = gameobject->vertex_normals;
			ImGui::Text("Draw:  "); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::PushID("draw_vertex"); ImGui::Combo("", &item, "Never\0Always\0"); ImGui::PopID();
			ImGui::Text("Length:");	ImGui::SameLine(); ImGui::PushItemWidth(120);  ImGui::PushID("vertex_length"); ImGui::DragFloat(" ", &vertex_lenght, 0.005f, 0.0f, 5.0f); ImGui::PopID();
			ImGui::Text("Color:"); ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::ColorEdit4("Vertex Color", (float*)&vertex_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);
			gameobject->vertex_normals = item;
			ImGui::TreePop();
		}
	}

	if (!unFold) {
		ImGui::GetStateStorage()->SetInt(ImGui::GetID(name.c_str()), 1);
		ImGui::GetStateStorage()->SetInt(ImGui::GetID("Face Normals:"), 1);
		ImGui::GetStateStorage()->SetInt(ImGui::GetID("Vertex Normals:"), 1);
		unFold = true;
	}
		
}

bool C_Normals::Disable()
{
	active = false;
	return true;
}

void C_Normals::Save(const char * _name, json & file)
{
	file["Game Objects"][_name]["Components"]["Normals"]["Active"] = active;
	file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Active"] = gameobject->face_normals;
	file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Lenght"] = face_lenght;
	file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][0] = face_color.r;
	file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][1] = face_color.g;
	file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][2] = face_color.b;
	file["Game Objects"][_name]["Components"]["Normals"]["Vertex Normals"]["Active"] = gameobject->vertex_normals;
	file["Game Objects"][_name]["Components"]["Normals"]["Vertex Normals"]["Lenght"] = vertex_lenght;
	file["Game Objects"][_name]["Components"]["Normals"]["Vertex Normals"]["Color"][0] = vertex_color.r;
	file["Game Objects"][_name]["Components"]["Normals"]["Vertex Normals"]["Color"][1] = vertex_color.g;
	file["Game Objects"][_name]["Components"]["Normals"]["Vertex Normals"]["Color"][2] = vertex_color.b;

}

void C_Normals::Load(const char * _name, const json & file)
{
	active = file["Game Objects"][_name]["Components"]["Normals"]["Active"].get<bool>();
	gameobject->face_normals = file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Active"].get<bool>();
	face_lenght = file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Lenght"].get<float>();
	face_color = Color(
		file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][0],
		file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][1],
		file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][2],
		1.0f
	);

	gameobject->vertex_normals = file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Active"].get<bool>();
	vertex_lenght = file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Lenght"].get<float>();
	vertex_color = Color(
		file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][0],
		file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][1],
		file["Game Objects"][_name]["Components"]["Normals"]["Face Normals"]["Color"][2],
		1.0f
	);
}
