#ifndef TEXTURE_R_H_
#define TEXTURE_R_H_

#include "Resources.h"

class Texture_R : public Resources
{
public:
	Texture_R() : Resources(TextureR) {}
	~Texture_R() {}

public:
	uint tex_id;
	aiTextureType type;
	vec2 size;
};
#endif

