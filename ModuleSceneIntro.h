#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"


#include "SystemInfo.h"

class GameObject;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Draw();
	bool setParent(GameObject* to_parent, GameObject* to_child);

	//Engine::SystemInfo s_info;

public:
	GameObject* root;
};
