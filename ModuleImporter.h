#ifndef IMPORTER_H
#define IMPORTER_H
#include "Application.h"
#include "Module.h"
#include "Shader.h"
#include "glmath.h"

#include <vector>

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

enum aiTextureType;

class GameObject;
class aiNode;
class aiScene;
struct Texture;
class aiMesh;
class aiScene;
class aiMaterial;

enum FileFormats {
	NONE = -1,
	FBX,
	PNG,
	DDS
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();
	
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	update_status Update(float dt);

	const string getFileExt(const string & s);

	string getRootPath(const string & s);

	const string getFileName(const string & s);

	bool CleanUp();

	bool LoadFile(const char * path);

	bool Load(const char * path);

	GameObject * LoadHierarchy(aiNode * node, aiScene * scene, string * FileName, string * str, GameObject * parent);

	GameObject * ProcessMesh(aiMesh * mesh, string * path = nullptr, const char* fileName = nullptr, const aiScene * scene = NULL);

	vector<Texture*> loadMaterialTextures(string * str, aiMaterial * mat, aiTextureType type);

	bool LoadTexture(const char * path, uint & texture, vec2 &size);

	Texture* SaveTexture(const char * str, aiTextureType type);

public:
	std::vector<Texture*> stored_textures;
	
private:
	GLubyte checkImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	bool enabled;
	uint Texture_id = 0;
public: 
	Shader* shader;
	uint checkImage_id;

};
#endif IMPORTER_H
