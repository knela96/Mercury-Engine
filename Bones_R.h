#ifndef BONES_R_H_
#define BONES_R_H_

#include "Globals.h"
#include "Application.h"
#include "Resources.h"


enum Buffers
{
	indices_size,
	vertices_size,
	normals_size,
	tex_coords_size,
	buffer_size
};

class Bones_R : public Resources
{
public:
	Bones_R() : Resources(BonesR) {}
	~Bones_R() {}

public:

	int             index; //ID and also the order in the joints list
	UID*			children; //joint's childrens
	mat4x4          InverseBindTransform;
	uint*			WeightIndex;
	uint*			WeightValues;

};
#endif

