#include "JointTransform.h"

mat4x4 JointTransform::GetLocalTransform()
{
	mat4x4 LocalTransform;
	mat4x4 aux;

	LocalTransform.translate(Position.x,Position.y,Position.z);

	LocalTransform = LocalTransform * aux.rotate(Position.x, { 1,0,0 });
	LocalTransform = LocalTransform * aux.rotate(Position.y, { 0,1,0 });
	LocalTransform = LocalTransform * aux.rotate(Position.z, { 0,0,1 });

	LocalTransform.scale(100.0f, 200.0f, 300.0f);

	return LocalTransform;
}

JointTransform JointTransform::Interpolate(JointTransform* PointA, JointTransform* PointB, float Progression) {
	
	vec3 pos = Interpolate(PointA->Position, PointB->Position, Progression);
	Quat rot = Interpolate(PointA->Rotation, PointB->Rotation, Progression);
	vec3 scale = Interpolate(PointA->Scale, PointB->Scale, Progression);
	return JointTransform(pos, rot,scale);
}

vec3 JointTransform::Interpolate(vec3 start, vec3 end, float progression)
{
	vec3 ret;
	ret.x = start.x + (end.x - start.x)*progression;
	ret.y = start.y + (end.y - start.y)*progression;
	ret.z = start.z + (end.z - start.z)*progression;
	
	return ret;
}

Quat JointTransform::Interpolate(Quat qa, Quat qb, float blend) {
	
	Quat ret = { 0, 0, 0, 1 };
	float dotP = qa.w*qb.w + qa.x*qb.x*+qa.y*qb.y + qa.z*qb.z;
	float blendI = 1.0f  - blend;

	if (dotP > 0) {
		ret.w = blendI * qa.w + blend * -qb.w;
		ret.x = blendI * qa.x + blend * -qb.x;
		ret.y = blendI * qa.y + blend * -qb.y;
		ret.z = blendI * qa.z + blend * -qb.z;
	}
	else if (blendI > 0) {
		ret.w = blendI * qa.w + blend * qb.w;
		ret.x = blendI * qa.x + blend * qb.x;
		ret.y = blendI * qa.y + blend * qb.y;
		ret.z = blendI * qa.z + blend * qb.z;
	}
	ret.Normalize();
	return ret;
}
