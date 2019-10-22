#pragma once
#include "Component.h"

class C_MeshInfo : public Component
{
public:
	C_MeshInfo(GameObject* gameobject, Component_Type type);
	~C_MeshInfo();

	bool Enable();
	void Update();
	bool Disable();

public:
	bool open_mesh_info = true;
};