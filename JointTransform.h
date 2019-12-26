#ifndef JOINTTRANSFORM_H_
#define JOINTTRANSFORM_H_

#include "Globals.h"
#include "Application.h"


class JointTransform
{
public:
	JointTransform(vec3 Position_, Quat Rotation_) :Position(Position_), Rotation(Rotation_) {};
	~JointTransform();

private:
	vec3 Position;
	Quat Rotation;
public:

	mat4x4 GetLocalTransform();

	JointTransform Interpolate(JointTransform PointA, JointTransform PointB, float Progression);
	vec3 Interpolate(vec3 start, vec3 end, float progression);
};



#endif
