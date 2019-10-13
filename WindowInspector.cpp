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
	return true;
}

bool WindowInspector::Draw()
{
	ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(1000, -1));
	ImGui::Begin("Inspector"); 
	ImGui::SetNextTreeNodeOpen(open_transform);
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

	ImGui::SetNextTreeNodeOpen(open_transform);
	if (ImGui::CollapsingHeader("Draw Normals"))
	{
		static bool alpha_preview = true;
		static bool alpha_half_preview = false;
		static bool drag_and_drop = true;
		static bool options_menu = true;
		static bool hdr = false;

		ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
		ImGui::Text("Face Normals:");

		ImGui::Text("Color:");
		ImGui::SameLine(); ImGui::PushItemWidth(200);
		ImGui::ColorEdit4("MyColor##3", (float*)&face_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

		ImGui::Text("Vertex Normals:");
	}

	ImGui::End();
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
