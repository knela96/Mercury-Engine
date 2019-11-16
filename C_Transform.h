#pragma once
#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

#ifdef _DEBUG
#pragma comment (lib, "lib/MathGeoLib/libx86/Debug/MathGeoLib.lib")
#else
#pragma comment (lib, "lib/MathGeoLib/libx86/Release/MathGeoLib.lib")
#endif

class C_Transform : public Component
{
public:
	C_Transform(GameObject* gameobject, Component_Type type);
	~C_Transform();

	bool Enable();
	void Update();
	void UpdateMatrices();
	bool Disable();

public:
	mat4x4 globalMatrix;
	mat4x4 localMatrix;

	float4x4 f4x4_2_mat4x4;

	float3 vposition = float3(0, 0, 0);
	Quat vrotation = Quat(0, 0, 0, 0);
	float3 vscale = float3(1, 1, 1);
};

