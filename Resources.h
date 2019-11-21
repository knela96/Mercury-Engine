#ifndef RESOURCES_H_
#define RESOURCES_H_
#include "Application.h"

enum ResourceType {
	noneR = -1,
	MeshR,
	TextureR,
	MaterialR
};

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


class Resources
{
public:
	Resources(ResourceType type);
	~Resources();

public:

	ResourceType type = noneR;
	std::string name = "";
	u64 ID;
	std::string resource_path = "";
	std::string original_path = "";
};
#endif

