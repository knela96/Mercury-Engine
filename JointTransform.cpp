#include "JointTransform.h"

mat4x4 JointTransform::GetLocalTransform()
{

	mat4x4 LocalTransform;
	mat4x4 aux;

	LocalTransform.translate(Position.x,Position.y,Position.z);

	LocalTransform = LocalTransform * aux.rotate(Position.x, { 1,0,0 });
	LocalTransform = LocalTransform * aux.rotate(Position.y, { 0,1,0 });
	LocalTransform = LocalTransform * aux.rotate(Position.z, { 0,0,1 });

	return LocalTransform;
}

JointTransform JointTransform::Interpolate(JointTransform PointA, JointTransform PointB, float Progression) {
	
	vec3 pos = Interpolate(PointA.Position, PointB.Position, Progression);
	Quat rot;

}

vec3 JointTransform::Interpolate(vec3 start, vec3 end, float progression)
{
	vec3 ret;
	ret.x = start.x + (end.x - start.x)*progression;
	ret.y = start.y + (end.y - start.y)*progression;
	ret.z = start.z + (end.z - start.z)*progression;
	
	return ret;
}
