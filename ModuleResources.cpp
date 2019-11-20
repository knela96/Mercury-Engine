
#include "Application.h"
#include "ModuleResources.h"



ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}


ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init()
{
	return true;
}

update_status ModuleResources::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleResources::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleResources::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	return true;
}

uint64 ModuleResources::GetResource(const char* path) {

	return -1;
}
