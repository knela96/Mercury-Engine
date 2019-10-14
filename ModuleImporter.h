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

	void PushObj(aiMesh* mesh);

	std::vector<MeshObject> meshes;

	void LoadTexture(uint Imageid,const char* path);
	ILboolean ilLoadImage(const char *FileName);
private:
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	bool enabled;
	uint Texture_id = 0;
};
#endif IMPORTER_H
