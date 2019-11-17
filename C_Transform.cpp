#include "C_Transform.h"
#include "C_Camera.h"

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
		float3 last_position = vposition;
		ImGui::Text("Position:");
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("X", &vposition.x, 0.1f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("Y", &vposition.y, 0.1f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("pos"); ImGui::DragFloat("Z", &vposition.z, 0.1f); ImGui::PopID();
	
		Quat last_rotation = vrotation;
		ImGui::Text("Rotation:");
		ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot"); ImGui::DragFloat("X", &vrotation.x, 0.1f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot");  ImGui::DragFloat("Y", &vrotation.y, 0.1f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60); ImGui::PushID("rot");  ImGui::DragFloat("Z", &vrotation.z, 0.1f); ImGui::PopID();
		
		float3 last_scale = vscale;
		ImGui::Text("Scale:   ");
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("X", &vscale.x, 0.1f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("Y", &vscale.y, 0.1f); ImGui::PopID();
		ImGui::SameLine(); ImGui::PushItemWidth(60);  ImGui::PushID("scale"); ImGui::DragFloat("Z", &vscale.z, 0.1f); ImGui::PopID();
		
		if (last_position.x != vposition.x || last_position.y != vposition.y || last_position.z != vposition.z ||
			last_rotation.x != vrotation.x || last_rotation.y != vrotation.y || last_rotation.z != vrotation.z ||
			last_scale.x != vscale.x || last_scale.y != vscale.y || last_scale.z != vscale.z) 
		{
			UpdateMatrices();
		}

	}

	if (!unFold) {
		ImGui::GetStateStorage()->SetInt(ImGui::GetID(name.c_str()), 1);
		unFold = true;
	}

}

void C_Transform::UpdateMatrices() {

	C_Camera* camera = (C_Camera*)gameobject->getComponent(Component_Type::Camera);

	mat4x4 translation = translate(vposition.x, vposition.y, vposition.z);
	mat4x4 rotation, aux;
	rotation = rotation * aux.rotate(vrotation.x, { 1,0,0 });
	rotation = rotation * aux.rotate(vrotation.y, { 0,1,0 }); 
	rotation = rotation * aux.rotate(vrotation.z, { 0,0,1 });

	mat4x4 scaling = scale(vscale.x, vscale.y, vscale.z);

	if (camera != nullptr)
		localMatrix = translation * rotation;
	else
		localMatrix = translation * rotation * scaling;


	if(gameobject->parent != nullptr)
		globalMatrix = gameobject->parent->transform->globalMatrix * localMatrix;
	else
		globalMatrix = localMatrix;

	gameobject->UpdateChilds();

	if (camera != nullptr) { 
		camera->UpdateTransformPosition(GameObject::mat2float4(globalMatrix)); 
	}

	gameobject->UpdateBox();

}

bool C_Transform::Disable()
{
	active = false;
	return true;
}
