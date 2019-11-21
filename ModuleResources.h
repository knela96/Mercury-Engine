#ifndef MODULERESOURCES_H_
#define MODULERESOURCES_H_

#include "Application.h"
#include "Module.h"

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();
	uint64 GetResource(const char * path);
};

#endif

