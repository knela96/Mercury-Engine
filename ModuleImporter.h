#ifndef IMPORTER_H
#define IMPORTER_H
#include "Application.h"
#include "Module.h"

#include <vector>

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64



enum aiTextureType;

class MeshObject;
struct Texture;
class aiMesh;
class aiScene;
class aiMaterial;

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

	MeshObject ProcessMesh(aiMesh * new_mesh,const aiScene * scene = NULL);

	void PushObj(aiMesh* mesh);

	uint LoadTexture(const char * path, uint & texture);

	vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type);
	
	bool CreateTexture();

public:
	std::vector<MeshObject> meshes;
	std::vector<Texture> stored_textures;
	
private:
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];
	bool enabled;
	uint Texture_id = 0;

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 vertices;\n"
		"layout (location = 1) in vec3 Normal;\n"
		"layout (location = 2) in vec3 TexCoords;\n"
		/*"layout (location = 3) in vec4 Colors;\n"*/
		/*"out vec3 ourColor;\n"*/
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(vertices, 1.0);\n"
		/*"   ourColor = aColor;\n"*/
		"}\0";

public: 

	int shaderProgram;

};
#endif IMPORTER_H
