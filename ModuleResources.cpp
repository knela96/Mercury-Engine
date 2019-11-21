#include "Application.h"
#include "ModuleResources.h"

ModuleResources::ModuleResources(Application * app, bool start_enabled) : Module(app,start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

uint64 ModuleResources::GetResource(const char* path) {

	return -1;
}