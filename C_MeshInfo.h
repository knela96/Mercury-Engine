#pragma once
#include "Component.h"
#include "Globals.h"

class C_MeshInfo : public Component
{
public:
	C_MeshInfo(GameObject* gameobject, Component_Type type);
	~C_MeshInfo();

	bool Enable();
	void Update();
	bool Disable();
	void Save(const char* gameObject, json &file);
	void Load(const char* gameObject, const json &file);

public:
	bool open_mesh_info = true;
	UID id = 0;
	string resource_name;
};