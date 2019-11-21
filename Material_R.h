#ifndef MATERIAL_R_H_
#define MATERIAL_R_H_

#include "Resources.h"

struct Color;

class Material_R : public Resources
{
public:
	Material_R() : Resources(MaterialR) {}
	~Material_R() {}

public:
	Color color;
	uint64 tex_ID_D;
	uint64 tex_ID_S;
	uint64 tex_ID_H;
	uint64 tex_ID_AO;
};
#endif

