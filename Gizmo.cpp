#include "Application.h"
#include "Gizmo.h"
#include "C_Transform.h"
#include "C_Camera.h"
#include "GameObject.h"
#include "Globals.h"







Gizmo::Gizmo() {
	/*ImGuizmo::Enable(true);*/
}
Gizmo::~Gizmo() {

}
void Gizmo::Update() {

	//ImGui::SetCursorPos(ImVec2(img_offset.x, img_offset.y));
	//img_corner = vec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y) + vec2(0, img_size.y);
	//img_corner.y = SCREEN_HEIGHT - img_corner.y;

	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	//	gizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	//if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	//	gizmoOperation = ImGuizmo::OPERATION::ROTATE;
	//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	//	gizmoOperation = ImGuizmo::OPERATION::SCALE;

	//HandleGuizmo();
	//Draw();
	
}

void Gizmo::Draw() {
	

	



}

void Gizmo::HandleGuizmo() {
	///*  1) comprobar si hi han objectes seleccionats
	//	2) assignar el primer objecte de la llista de seleccionats a un gameobject temporal q crearem en aquesta funció
	//	3) Guardem la matriu q esta a App.camera.getcamera.frustrum.viumatrix en un float4x4 viewMatrix
	//	4) la transposem
	//	5)projection matrix es frustum.projectionmatrix.transposada
	//	6)modelprojection matrix es el gameobject q hem creat al pas 2.getcomponent<C_Transform>()->GetGlobalTransform().Transposed();
	//	7) llamar al setdrawlist del imguizmo
	//*/

	//if (App->gui->inspector->active_gameObject != nullptr) {
	//	GameObject* gameObject = App->gui->inspector->active_gameObject;

	//	float4x4 viewM = App->camera->camera->frustum.ViewMatrix();
	//	viewM.Transpose();

	//	float4x4 projectionM = App->camera->camera->frustum.ProjectionMatrix().Transposed();
	//	float4x4 modelProjection = gameObject->mat2float4(gameObject->transform->localMatrix);

	//	ImGuizmo::SetDrawlist();
	//	cornerPos = vec2(img_corner.x, SCREEN_HEIGHT - img_corner.y - img_size.y);
	//	ImGuizmo::SetRect(img_corner.x, cornerPos.y, img_size.x, img_size.y);

	//	float modelPtr[16];
	//	memcpy(modelPtr, modelProjection.ptr(), 16 * sizeof(float));
	//	ImGuizmo::MODE finalMode = (gizmoOperation == ImGuizmo::OPERATION::SCALE ? ImGuizmo::MODE::LOCAL : gizmoMode);
	//	ImGuizmo::Manipulate(viewM.ptr(), projectionM.ptr(), gizmoOperation, finalMode, modelPtr);

	//	if (ImGuizmo::IsUsing())
	//	{
	//		float4x4 newMatrix;
	//		newMatrix.Set(modelPtr);
	//		modelProjection = newMatrix.Transposed();
	//		/*primer pillar quina variacio de x o y o z ha fer
	//	despres podrem fer un updatre matrices per actualitzar posicions			
	//		*/
	//		//gameObject->GetComponent<C_Transform>()->SetGlobalMatrix(modelProjection);
	//		float3 Position; 
	//		Quat Rotation;
	//		float3 Scale;

	//		modelProjection.Decompose(Position,Rotation,Scale);

	//		gameObject->transform->vposition = Position;
	//		gameObject->transform->vrotation = Rotation;
	//		gameObject->transform->vscale = Scale;
	//		
	//		gameObject->transform->UpdateMatrices();
	//	}
	//	
	//}
}

















void Gizmo::DrawCubeBox(float3* points, Color color)
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glLineWidth(25.0f);
	glColor3f(color.r, color.g, color.b);
	glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
	glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
	glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
	glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
	glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
	glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
	glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
	glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));


	glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
	glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
	glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
	glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
	glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
	glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
	glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
	glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));

	glVertex3f(points[0].At(0), points[0].At(1), points[0].At(2));
	glVertex3f(points[2].At(0), points[2].At(1), points[2].At(2));
	glVertex3f(points[1].At(0), points[1].At(1), points[1].At(2));
	glVertex3f(points[3].At(0), points[3].At(1), points[3].At(2));
	glVertex3f(points[4].At(0), points[4].At(1), points[4].At(2));
	glVertex3f(points[6].At(0), points[6].At(1), points[6].At(2));
	glVertex3f(points[5].At(0), points[5].At(1), points[5].At(2));
	glVertex3f(points[7].At(0), points[7].At(1), points[7].At(2));

	glLineWidth(1);
	glEnd();
	glEnable(GL_LIGHTING);

}


