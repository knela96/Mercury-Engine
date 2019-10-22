#ifndef COMPONENT_H_
#define COMPONENT_H_
#include "Globals.h"
#include "Application.h"
#include "GameObject.h"

enum Component_Type {
	Transform,
	Mesh_Info,
	Normals,
	Material,
	Script
};

class Component
{
public:
	Component(Component_Type type);
	~Component();

	virtual bool Enable() { return true; }
	virtual void Update(){}
	virtual bool Disable() { return true; }

private:
	GameObject* gameobject;
	bool active;
	string name;
	Component_Type type;
};


#endif COMPONENT_H_

