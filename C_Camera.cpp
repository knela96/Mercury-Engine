#include "Globals.h"
#include "Application.h"
#include "C_Camera.h"
#include "ModuleGUI.h"
//#include "Gizmo.h"

C_Camera::C_Camera(GameObject* gameobject, Component_Type type) : Component(type, gameobject)
{
	name = "Camera";
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0, 0, 0);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 1.0f;
	frustum.farPlaneDistance = 2000.0f;
	frustum.verticalFov = 1.0f;
	frustum.horizontalFov = 1.0f;
	GetPlanes();
}

C_Camera::~C_Camera()
{
}

bool C_Camera::Enable()
{
	active = true;
	App->scene_intro->AddFrustum(&frustum, Color(1.0f, 0.0f, 0.0f, 1.0f));
	return true;
}

void C_Camera::Update()
{
	/*if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_AllowItemOverlap))
	{
		ImGui::DragInt("FOV", &fov,1,1,200);
		if (ImGui::IsItemEdited()) { SetFOV(frustum.verticalFov); }
	}*/

	//-- set 0.0 at game window left top
	
	SDL_GetMouseState(&MouseX, &MouseY);

	ImVec2 a = App->gui->game->position;

	ImVec2 mouse_;

	if (MouseX < a.x) {
		mouse_.x = 0;
	}
	else {
		mouse_.x = MouseX - a.x;
	}

	if (MouseY < a.y+37) {
		mouse_.y = 0;
	}
	else {
		mouse_.y = MouseY - a.y-37;
	}
	LOGC("X: %f Y: %f", mouse_.x, mouse_.y);


	//--

	
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_STATE::KEY_DOWN) {
		showray = true;	

		if (showray) {
			
			vec2 Mouse;
			/*int Mouse_X = Mouse.x+500;
			int Mouse_Y = Mouse.y;*/
			Mouse.x = mouse_.x-500;
			Mouse.y = mouse_.y-200;
			
			//SDL_GetMouseState(&Mouse_X, &Mouse_Y);
			vec2 MouseN = normalize(Mouse);
			
			
			picking = frustum.UnProjectLineSegment(MouseN.x, MouseN.y);
			picking.b = frustum.front * 100;
			hit = picking.Intersects(App->scene_intro->root->childs[1]->aabb);			
		}
	}

	if (showray) {		
		glBegin(GL_LINES);
		glDisable(GL_LIGHTING);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(picking.a.x, picking.a.y, picking.a.z);
		glVertex3f(picking.b.x, picking.b.y, picking.b.z);
		/*glVertex3f(0, 0, 0);
		glVertex3f(picking.b.x, picking.b.y, picking.b.z);*/
		glEnd();
		glBegin(GL_LIGHTING);
	}
}

void C_Camera::UpdateTransformPosition(float4x4 global) {

}

bool C_Camera::Disable()
{
	active = false;
	return true;
}

void C_Camera::GetPlanes()
{
	frustum.GetPlanes(planes);
}

void C_Camera::SetNearPlane(float distance) {
	if (distance > 0 && distance < frustum.farPlaneDistance)
		frustum.farPlaneDistance = distance;
	GetPlanes();
}

void C_Camera::SetFarPlane(float distance) {
	if (distance > 0 && distance > frustum.nearPlaneDistance)
		frustum.nearPlaneDistance = distance;
	GetPlanes();
}

float C_Camera::GetFOV() {
	return frustum.verticalFov * RADTODEG;
}

float C_Camera::GetAspectRatio() {
	return frustum.AspectRatio();
}

void C_Camera::SetFOV(float fov) {
	frustum.verticalFov = fov * DEGTORAD;
	frustum.horizontalFov = 2.f * Atan(Tan(frustum.verticalFov*0.5f) / GetAspectRatio());
	GetPlanes();
}

void C_Camera::SetPos(float3 position) {
	frustum.pos = position;
	GetPlanes();
}

void C_Camera::SetPos(vec3 position) {
	frustum.pos = float3(position.x, position.y, position.z);
	GetPlanes();
}

void C_Camera::SetAspectRatio(float ar) {
	frustum.verticalFov = 2.f * Atan(Tan(frustum.horizontalFov*0.5f) / ar);
	GetPlanes();
}

float* C_Camera::ViewMatrix() {
	float4x4 m = frustum.ViewMatrix();
	m.Transpose();
	return (float*)m.v;
}

mat4x4 C_Camera::ViewMatrix4x4() {
	float4x4 m = frustum.ViewMatrix();
	m.Transpose();
	mat4x4 m4 = { m.At(0,0), m.At(0,1), m.At(0,2), m.At(0,3),
				 m.At(1,0), m.At(1,1), m.At(1,2), m.At(1,3),
				 m.At(2,0), m.At(2,1), m.At(2,2), m.At(2,3),
				 m.At(3,0), m.At(3,1), m.At(3,2), m.At(3,3) };
	return m4;
}

float* C_Camera::ProjectionMatrix() {
	float4x4 m = frustum.ProjectionMatrix();
	m.Transpose();
	return (float*)m.v;
}
mat4x4 C_Camera::ProjectionMatrix4x4() {
	float4x4 m = frustum.ProjectionMatrix();
	m.Transpose();
	mat4x4 m4 = { m.At(0,0), m.At(0,1), m.At(0,2), m.At(0,3),
				 m.At(1,0), m.At(1,1), m.At(1,2), m.At(1,3),
				 m.At(2,0), m.At(2,1), m.At(2,2), m.At(2,3),
				 m.At(3,0), m.At(3,1), m.At(3,2), m.At(3,3) };
	return m4;
}
void C_Camera::Look(const float3 &Spot) {
	float3 v = Spot - frustum.pos;

	float3x3 matrix = float3x3::LookAt(frustum.front, v.Normalized(), frustum.up, float3::unitY);

	frustum.front = (matrix.MulDir(frustum.front).Normalized());
	frustum.up = (matrix.MulDir(frustum.up).Normalized());

	GetPlanes();
}

vec2 C_Camera::ScreenToWorld(vec2 p) {
	vec2 ret = p - vec2(App->gui->game->position.x, App->gui->game->position.y+37);
	ret = ret / (vec2(App->gui->game->size_Game.x, App->gui->game->size_Game.y) * vec2(SCREEN_WIDTH,SCREEN_HEIGHT));
	return ret;
}
//
//GameObject* C_Camera::Pick(float3* hit_point) const
//{
//	// The point (1, 1) corresponds to the top-right corner of the near plane
//	// (-1, -1) is bottom-left
//
//	float width = (float)SCREEN_WIDTH;
//	float height = (float)SCREEN_HEIGHT;
//
//	int mouse_x, mouse_y;
//	mouse_x = App->input->GetMouseX(); 
//	mouse_y = App->input->GetMouseY();
//
//	float normalized_x = -(1.0f - (float(mouse_x) * 2.0f) / width);
//	float normalized_y = 1.0f - (float(mouse_y) * 2.0f) / height;
//
//	LineSegment picking = frustum.UnProjectLineSegment(normalized_x, normalized_y);
//
//	float distance;
//	GameObject* hit = App->level->CastRay(picking, distance);
//
//	if (hit != nullptr && hit_point != nullptr)
//		*hit_point = picking.GetPoint(distance);
//
//	return hit;
//}