#include "AnimatedObject.h"





void AnimatedObject::DeleteAnimatedObject()
{
	
}

mat4x4* AnimatedObject::GetJointsTransforms()
{
	mat4x4* JointsMatrices = new mat4x4[JointCount];
	AddJointsToArray(&RootJoint, JointsMatrices);


	return JointsMatrices;
}

void AnimatedObject::AddJointsToArray(Joint* ParentJoint, mat4x4* JointsMatrices) {

	JointsMatrices[ParentJoint->index] = ParentJoint->GetAnimationTransform();
	for (list<Joint*>::iterator it = ParentJoint->children.begin(); it != ParentJoint->children.end();) {
		AddJointsToArray(*it, JointsMatrices);
	}
}