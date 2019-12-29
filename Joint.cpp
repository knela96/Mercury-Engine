#include "Joint.h"


Joint::~Joint()
{
}

void Joint::CalculateInverseBindTransform(mat4x4 ParentBindTransform)
{
	mat4x4 InverseBindTransform = (ParentBindTransform * LocalBindTransform).inverse(); 
	for (int i = 0; i < children.size(); i++) {
		CalculateInverseBindTransform(InverseBindTransform);
	}
}
