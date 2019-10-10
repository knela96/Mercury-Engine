#ifndef IMPORTER_H
#define IMPORTER_H
#include "Application.h"
#include "Module.h"

#include "Module.h"
#include <vector>

class MeshObject;


class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();
	
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool Draw();

	bool CleanUp();

	bool Load(const char * path);

private:
	std::vector<MeshObject*> meshes;
	bool enabled;
};
#endif IMPORTER_H
