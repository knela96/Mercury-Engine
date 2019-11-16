#pragma once
#include "Application.h"
#include "Module.h"
#include "MathGeoLib/include/MathGeoLib.h"

#ifdef _DEBUG
#pragma comment (lib, "lib/MathGeoLib/libx86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "lib/MathGeoLib/libx86/Release/MathGeoLib.lib")
#endif

class C_Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	void MoveCamera(float dt);
	void FocusTarget();
	void Orbit();
	void Pan();
	void Zoom();
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	float3 to_float3(vec3 vec);
	vec3 to_vec3(float3 vec);
	//void Move(const vec3 &Movement);
	//float* GetViewMatrix();

	//mat4x4 GetViewMatrix4x4();

private:

	//void CalculateViewMatrix();
	bool moving = false;
	vec3 newPos;
	float speed = 0.0f;

public:
	
	vec3 X, Y, Z, Position, Reference;
	C_Camera* camera = nullptr;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};