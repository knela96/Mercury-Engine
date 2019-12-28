#ifndef ANIMATEDOBJECT_H_
#define ANIMATEDOBJECT_H_

#include "Globals.h"
#include "Application.h"
#include "Joint.h"
#include "MeshObject.h"


class AnimatedObject
{
public:
	AnimatedObject(GameObject AnimatedObject_, Joint RootJoint_, int JointCount_) :AnimatedObj(&AnimatedObject_), RootJoint(RootJoint_), JointCount(JointCount_)
	{ RootJoint.CalculateInverseBindTransform(IdentityMatrix); };

	~AnimatedObject() {};

public:

	//obj
	GameObject   *AnimatedObj;

	Joint        RootJoint;
	int          JointCount; 

	//Animator animator;

public:

	GameObject GetAnimatedObj() { return *AnimatedObj; }
	Joint GetRootJoint() { return RootJoint; }
	int GetJointCount() { return JointCount; }

	void SetAnimatedObject(GameObject AnimatedObj_) { *AnimatedObj = AnimatedObj_; }
	void SetRootJoint(Joint RootJoint_) { RootJoint = RootJoint_; }
	void SetJointCount(int JointCount_) { JointCount = JointCount_; }

	void DeleteAnimatedObject();

	//void DoAnimation(char* AnimName) { animator.animate(AnimName); }

	mat4x4* GetJointsTransforms();

	void AddJointsToArray(Joint* RootJoint_, mat4x4* JointsMatrices);

};

#endif



