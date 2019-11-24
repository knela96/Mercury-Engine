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
	Script,
	Camera
};

class Component
{
public:
	Component(Component_Type type, GameObject* gameobject, bool active = true) : type(type), gameobject(gameobject) {
	}
	~Component() {}

	virtual bool Start() { return true; }
	virtual bool Enable() { return true; }
	virtual void Update(){}
	virtual bool Disable() { return true; }
	virtual void Load(const char* gameObject, const json &file) {};
	virtual void Save(const char* gameObject, json &file) {};

	bool isActive() {
		return active;
	}

	Component_Type type;
protected:
	GameObject* gameobject;
	bool active;
	bool unFold = false;
	string name;
	UID ID = 0;
};


#endif COMPONENT_H_

