#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"


#include "SystemInfo/SystemInfo.h"
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	
	//Engine::SystemInfo s_info;

public:
};
