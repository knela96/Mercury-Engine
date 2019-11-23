#include "Globals.h"
#include "Application.h"
#include "C_Camera.h"
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
	if (gameobject == nullptr) {
		frustum.verticalFov = 1.0f;
		frustum.horizontalFov = 1.0f;
	}
	else {
		SetFOV(60);
	}
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
	if (gameobject != nullptr) {
		if (ImGui::CollapsingHeader("Camera"), open_camera_info)
		{
			ImGui::DragInt("FOV", &fov, 1, 1, 200);
			if (ImGui::IsItemEdited()) { SetFOV(fov); }

			ImGui::Checkbox("Culling", &culling);
		}
	}

}

void C_Camera::UpdateTransformPosition(float4x4 global) {
	frustum.front = global.WorldZ();
	frustum.up = global.WorldY();

	float3 position;
	float3 scale;
	Quat quat;
	global.Decompose(position, quat, scale);

	SetPos(position);
	GetPlanes();
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
		frustum.nearPlaneDistance = distance;
	GetPlanes();
}

void C_Camera::SetFarPlane(float distance) {
	if (distance > 0 && distance > frustum.nearPlaneDistance)
		frustum.farPlaneDistance = distance;
	GetPlanes();
}

float C_Camera::GetFOV() {
	return frustum.verticalFov * RADTODEG;
}

float C_Camera::GetAspectRatio() {
	return frustum.AspectRatio();
}

void C_Camera::SetFOV(float fov, float width, float height) {
	float ar = height / width;
	frustum.verticalFov = fov * DEGTORAD;
	frustum.horizontalFov = 2.f * Atan(Tan(frustum.verticalFov*0.5f) / ar);
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

bool C_Camera::CullFace(GameObject * gameobject)
{
	if (culling) {
		float3 vCorner[8];
		int iTotalIn = 0;
		gameobject->aabb.GetCornerPoints(vCorner); // get the corners of the box into the vCorner array
		// test all 8 corners against the 6 sides
		// if all points are behind 1 specific plane, we are out
		// if we are in with all points, then we are fully in
		for (int p = 0; p < 6; ++p) {
			int iInCount = 8;
			int iPtIn = 1;
			for (int i = 0; i < 8; ++i) {
				// test this point against the planes
				if (planes[p].IsOnPositiveSide(vCorner[i])) {
					iPtIn = 0;
					--iInCount;
				}
			}
			// were all the points outside of plane p?
			if (iInCount == 0)
				return(false);
			// check if they were all on the right side of the plane
			iTotalIn += iPtIn;
		}
		// so if iTotalIn is 6, then all are inside the view
		if (iTotalIn == 6)
			return(true);
		// we must be partly in then otherwise
	}
	return(true);
}
