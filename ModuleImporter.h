#ifndef IMPORTER_H
#define IMPORTER_H
#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "Shader.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>
#include "Joint.h"
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
struct aiBone;
class aiScene;
class aiMaterial;
class Mesh_R;
class Resources;
class Joint;

enum FileFormats {
	NONE = -1,
	FBX,
	PNG,
	DDS
};
class Keyframe;
class Animation;

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

	GameObject * LoadHierarchy(aiNode * node, aiScene * scene, const char * str, GameObject * parent, vector<aiMesh*>* boned_meshes, vector<GameObject*>& objects);

	UID ImportResourceMesh(aiMesh * newMesh, const char * str, const char * fileName);

	GameObject * ProcessMesh(aiMesh * mesh, string * path = nullptr, const char* fileName = nullptr, const aiScene * scene = NULL);

	Resources * LoadObjectResource(UID id);

	Resources* ImportObject(const char* FileName, UID* id);

	void ImportObjectBones(const std::vector<aiMesh*>& meshes, const std::vector<GameObject*>& objects, GameObject * root, const char * source_file);

	void SaveGameObjectConfig(json & config, GameObject * gameObjects);

	void SaveGameObjectConfig(json & config, std::vector<GameObject*>& gameObjects);

	vector<Animation*> ImportAnimations(const aiScene *scene);

	Keyframe* FindNextFrame(uint index, string & name, std::map<uint, Keyframe*>& map);

	void InterpolateKeyFrames(Keyframe* prevFrame, Keyframe* nextFrame, bool empty, string& name, std::map<uint, Keyframe*>& map);

	Joint* ImportMeshBones(vector<aiMesh*>* newMesh, const char* str, const char* fileName, GameObject* root, int& size);
	void LoadHierarchyJoints(GameObject * gameobject, std::map<std::string, aiBone*>* bones, Joint *& joint, vector<Joint*>& joints);
	void CollectGameObjectNames(aiMesh * mesh, std::map<std::string, aiBone*>& map, int& count);
	UID	ImportResourceBones(aiMesh* newMesh, const char* str, const char* fileName);

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


public: //OUR OWN FILE FORMAT HERE
	void saveGOinFile(const GameObject *go);


};
#endif IMPORTER_H
