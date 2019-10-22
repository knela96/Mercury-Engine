#include "C_Transform.h"

C_Transform::C_Transform(GameObject* gameobject, Component_Type type) : Component(type, gameobject)
{
	name = "Transform";

}

C_Transform::~C_Transform()
{
}

bool C_Transform::Enable()
{
	active = true;
	return true;
}

void C_Transform::Update()
{

	if (ImGui::CollapsingHeader(name.c_str()))
	{
		ImGui::Text("Position:");
		ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 240*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("X", &position.x, 0.005f); ImGui::PopID();
		ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 160*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("Y", &position.y, 0.005f); ImGui::PopID();
		ImGui::SameLine(/*ImGui::GetWindowContentRegionWidth() - 80*/); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("Z", &position.z, 0.005f); ImGui::PopID();

		ImGui::Text("Rotation:");
		ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot"); ImGui::DragFloat("X", &rotation.x, 0.005f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot");  ImGui::DragFloat("Y", &rotation.y, 0.005f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot");  ImGui::DragFloat("Z", &rotation.z, 0.005f); ImGui::PopID();

		ImGui::Text("Scale:   ");
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("X", &scale.x, 0.005f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("Y", &scale.y, 0.005f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("Z", &scale.z, 0.005f); ImGui::PopID();
	}

	if (!unFold) {
		ImGui::GetStateStorage()->SetInt(ImGui::GetID(name.c_str()), 1);
		unFold = true;
	}
}

bool C_Transform::Disable()
{
	active = false;
	return true;
}
