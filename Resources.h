#ifndef RESOURCES_H_
#define RESOURCES_H_
#include "Application.h"

enum ResourceType {
	noneR = -1,
	ScenenR,
	MeshR,
	ObjectR,
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
	virtual void FreeMemory() {};

public:
	uint instances = 0;
	bool need_save = false;
	ResourceType type = noneR;
	std::string name = "";
	UID ID;
	std::string resource_path = "";
	std::string original_path = "";
};
#endif

