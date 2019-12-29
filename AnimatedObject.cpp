#include "AnimatedObject.h"


void AnimatedObject::DeleteAnimatedObject()
{
	
}

mat4x4* AnimatedObject::GetJointsTransforms()
{
	mat4x4* JointsMatrices = new mat4x4[JointCount];
	AddJointsToArray(RootJoint, JointsMatrices);


	return JointsMatrices;
}

void AnimatedObject::AddJointsToArray(Joint* ParentJoint, mat4x4* JointsMatrices) {

	JointsMatrices[ParentJoint->index] = ParentJoint->GetAnimationTransform();
	for (int i = 0; i < ParentJoint->children.size(); ++i) {
		AddJointsToArray(ParentJoint->children[i], JointsMatrices);
	}
}