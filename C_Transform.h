#pragma once
#include "Component.h"
#include "glmath.h"

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
	float4x4 globalMatrix;
	float4x4 localMatrix;

	float4x4 translationMatrix;
	float4x4 rotationMatrix;
	float4x4 scalingMatrix;
	mat4x4 tr;

	float3 position = float3(0, 0, 0);
	Quat rotation = Quat(0, 0, 0, 0);
	float3 scale = float3(1, 1, 1);
};

