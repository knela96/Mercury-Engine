#ifndef __MODULERESOURCES_H__
#define __MODULERESOURCES_H__
#include "Application.h"
#include "Module.h"
#include "Globals.h"

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


	uint64 GetResource(const char * path);
};

#endif

