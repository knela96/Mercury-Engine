#ifndef MODULERESOURCES_H_
#define MODULERESOURCES_H_

#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include "lib/MathGeoLib/include/Algorithm/Random/LCG.h"


#include <map>

struct Meta
{
	ResourceType type = noneR;
	std::string original_file = "";
	std::string resource_name = "";
	uint64 id = 0;

	bool Compare(const char* file, const char* name, ResourceType type)
	{
		return (original_file == file && resource_name == name && type == this->type);
	}

};

enum ResourceType;
class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool start_enabled = true);
	~ModuleResources();

	UID Find(const char* file_in_assets) const;
	UID ImportFile(const char* new_file_in_assets, bool force = false);
	void ImporSceneResource(const char * file, std::string origin_path);
	ResourceType GetType(const char * path);
	UID GenerateNewUID();
	void AddResource(Resources * resource);
	Meta GetMetaInfo(Resources * resource);
	void SaveMetaInfo(Resources * resource);
	void LoadResource(Resources * resource);
	const Resources* Get(UID uid) const;
	Resources* Get(UID uid);
	Resources* CreateNewResource(ResourceType type, UID force_uid = 0);
	Meta * FindMetaResource(const char * original_file, const char * name, ResourceType type);
private:
	std::map<UID, Resources*> resources;

	std::map<uint64, Meta> created_resources;
	//Just for quick info display
	std::map<uint64, Resources*> meshes;
	std::map<uint64, Resources*> materials;
	std::map<uint64, Resources*> textures;
	std::map<uint64, Resources*> scenes;
	std::map<uint64, Resources*> animations;

	LCG random;

};

#endif

