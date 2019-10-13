#ifndef IMPORTER_H
#define IMPORTER_H
#include "Application.h"
#include "Module.h"

#include "Module.h"
#include <vector>

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

class MeshObject;
class aiMesh;

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();
	
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	update_status Update(float dt);
	bool Draw();

	bool CleanUp();

	bool Load(const char * path);

	MeshObject ProcessMesh(aiMesh * new_mesh);

	void LoadTexture(uint Imageid);

	void PushObj(aiMesh* mesh);
private:
	std::vector<MeshObject> meshes;	
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	bool enabled;
};
#endif IMPORTER_H
