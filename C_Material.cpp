#include "C_Material.h"

C_Material::C_Material(GameObject* gameobject, Component_Type type) : Component(type, gameobject)
{
	name = "Material";
}

C_Material::~C_Material()
{
}

bool C_Material::Enable()
{
	active = true;
	return true;
}

void C_Material::Update()
{
	ImGui::PushID("active_material"); ImGui::Checkbox("", &active); ImGui::PopID(); ImGui::SameLine();
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_AllowItemOverlap))
	{
		ImGui::Columns(2, NULL, false);
		for (int i = 0; i < gameobject->textures.size(); ++i) {
			ImGui::Text("%s", gameobject->getType(gameobject->textures[i]->type));
			ImGui::Text("%s", gameobject->textures[i]->path.c_str());
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::Text("%s", gameobject->textures[i]->path.c_str());
				ImGui::EndTooltip();
			}
			ImGui::Text("");
			ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 80); ImGui::Text("%ix%i px ", (uint)gameobject->textures[i]->size.x, (uint)gameobject->textures[i]->size.y);
			ImGui::NextColumn();
			ImGui::Image((ImTextureID*)gameobject->textures[i]->id, ImVec2(60, 60), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
			if (i == 0) {
				ImGui::SameLine();
				ImGui::Checkbox("Debug", &gameobject->debug_tex);
			}
			ImGui::NextColumn();
		}
		if (gameobject->textures.size() == 0) {
			ImGui::Columns(2, NULL, false);
			ImGui::NewLine();
			ImGui::NextColumn();
			ImGui::SameLine(78); ImGui::Checkbox("Debug", &gameobject->debug_tex);
		}
		ImGui::Columns(1);
	}

	if (!unFold) {
		ImGui::GetStateStorage()->SetInt(ImGui::GetID(name.c_str()), 1);
		unFold = true;
	}
}

bool C_Material::Disable()
{
	active = false;
	return true;
}
