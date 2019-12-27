#ifndef ANIMATOR_H_
#define ANIMATOR_H_

#include "Globals.h"
#include "Application.h"
#include "Keyframe.h"
#include "AnimatedObject.h"
#include "Animation.h"
#include "Joint.h"
#include "Component.h"


class Animator : public Component
{
public:
	Animator(GameObject* gameobject, Component_Type type) ;
	
	//Animator(AnimatedObject myAnimatedBody_) : myAnimatedBody(myAnimatedBody_), AnimationTime(0), Component(ResourceType::AnimationR,myAnimatedBody.AnimatedObj){};
	~Animator();

private:
	
	float AnimationTime; //curr anim time

public:

	AnimatedObject* myAnimatedBody;

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
	UID id = 0;
	string resource_name;

};

#endif



