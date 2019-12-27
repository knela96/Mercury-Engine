#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include "Globals.h"
#include "Application.h"
#include "Keyframe.h"
#include "AnimatedObject.h"
#include "Animation.h"
#include "Joint.h"
#include "Component.h"

class Animator /*: public Component*/
{
public:
	Animator(AnimatedObject myAnimatedBody_) : myAnimatedBody(myAnimatedBody_), AnimationTime(0)/*, Component(ResourceType::AnimationR,myAnimatedBody.AnimatedObj)*/{};
	~Animator();

private:

	AnimatedObject myAnimatedBody;	
	float AnimationTime; //curr anim time

public:

	void doAnimation(Animation *animation);
	void IncreaseAnimationTime();
	map<string, mat4x4>  CalculateCurrAnimationPose();
	void ApplyPoseToJoints(map<string, mat4x4> currPose, Joint *RootJoint, mat4x4 mat);
	void UpdateAnim();
	void GetPreviousAndNextFrame(Keyframe* frames);
	float CalculateProgression(Keyframe a, Keyframe b);
	map<string, mat4x4> InterpolatePoses(Keyframe a, Keyframe b, float progression);

public:
	Animation* currAnimation;

};

#endif



