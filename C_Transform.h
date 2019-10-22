#pragma once
#include "Component.h"

class C_Transform : public Component
{
public:
	C_Transform(GameObject* gameobject, Component_Type type);
	~C_Transform();

	bool Enable();
	void Update();
	bool Disable();

private:
	vec3 position;
	vec3 rotation;
	vec3 scale;
};

