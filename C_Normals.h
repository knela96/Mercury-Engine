#pragma once
#include "Component.h"



class C_Normals : public Component
{
public:
	C_Normals(GameObject* gameobject, Component_Type type);
	~C_Normals();

	bool Enable();
	void Update();
	bool Disable();
	void Save(const char* gameObject, json &file);
	void Load(const char* gameObject, const json &file);

public:
	bool open_normals = true;
	float face_lenght;
	float vertex_lenght;
	Color face_color;
	Color vertex_color;
};

