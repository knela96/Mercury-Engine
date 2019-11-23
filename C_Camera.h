#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"
#ifdef _DEBUG
#pragma comment (lib, "lib/MathGeoLib/libx86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "lib/MathGeoLib/libx86/Release/MathGeoLib.lib")
#endif

class C_Camera : public Component
{
public:
	C_Camera(GameObject* gameobject, Component_Type type);
	~C_Camera();

	bool Enable();
	void Update();
	void UpdateTransformPosition(float4x4 global);
	bool Disable();
	void GetPlanes();

	void SetNearPlane(float distance);

	void SetFarPlane(float distance);

	float GetFOV();

	float GetAspectRatio();

	void SetFOV(float fov, float width = 16.0f, float height = 9.0f);

	void SetPos(float3 position);

	void SetPos(vec3 position);

	//void SetAspectRatio(float ar, float hFov);

	void SetAspectRatio(float ar);

	float* ViewMatrix();
	mat4x4 ViewMatrix4x4();
	float* ProjectionMatrix();

	mat4x4 ProjectionMatrix4x4();

	void Look(const float3 & Spot);

	bool CullFace(GameObject * gameobject);

public:
	Plane planes[6];
	Frustum frustum;
	int fov = 60;
	bool open_camera_info = true;
	bool culling = false;
};

#endif