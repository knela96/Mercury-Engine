#ifndef RESOURCES_H_
#define RESOURCES_H_
#include "Application.h"

enum ResourceType {
	noneR = -1,
	ScenenR,
	MeshR,
	AudioR,
	TextureR,
	MaterialR
};


class Resources
{
public:
	Resources(ResourceType type);
	~Resources();
	ResourceType getType() {
		return type;
	}

	virtual void LoadOnMemory() {};

public:

	ResourceType type = noneR;
	std::string name = "";
	u64 ID;
	std::string resource_path = "";
	std::string original_path = "";
};
#endif

