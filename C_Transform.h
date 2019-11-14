#pragma once
#include "Component.h"

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

	float3 position;
	Quat rotation;
	float3 scale;
};

