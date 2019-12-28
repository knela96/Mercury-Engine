#include "Application.h"
#include "Animator.h"



Animator::Animator(GameObject * gameobject, Component_Type type) : Component(type, gameobject){}

Animator::~Animator()
{
}


void Animator::UpdateAnim()
{
	if (currAnimation == nullptr) {
		return;
	}
	else {
		IncreaseAnimationTime();
		std::map<string, mat4x4> currPose = CalculateCurrAnimationPose();
		ApplyPoseToJoints(currPose, &myAnimatedBody->GetRootJoint(), mat4x4());
	}

}

void Animator::doAnimation(uint index) {
	AnimationTime = 0;
	currAnimation = Animations.at(index);
}
void Animator::IncreaseAnimationTime() {
	AnimationTime += 1; //need to create a separated stable 30fps system for animation, not good idea to make them depend of the engine framerate;
	if (AnimationTime > currAnimation->GetLength()) {
		AnimationTime = 0;
	}
}
map<string, mat4x4> Animator::CalculateCurrAnimationPose()
{
	Keyframe frames[2];
	GetPreviousAndNextFrame(frames);
	float progression = CalculateProgression(frames[0], frames[1]);
	return InterpolatePoses(frames[0],frames[1],progression);
}
void Animator::ApplyPoseToJoints(map<string, mat4x4> currPose, Joint *ParentJoint, mat4x4 Parentmat) {
	mat4x4 currLocalTransform = currPose[ParentJoint->name];
	mat4x4 currTransform = Parentmat * currLocalTransform;


	list<Joint*>::iterator it = ParentJoint->children.begin();
	
	for (it; it != ParentJoint->children.end(); it++) {
		ApplyPoseToJoints(currPose, *it, currTransform);
	}
	mat4x4 container = currTransform * ParentJoint->GetInverseBindTransform();
	ParentJoint->SetAnimationTransform(container);

		//WTF
}

void Animator::GetPreviousAndNextFrame(Keyframe* frames) {

	vector<Keyframe*>* allFrames = currAnimation->GetKeyframes();
	Keyframe* prevFrame = allFrames->at(0);
	Keyframe* nextFrame = allFrames->at(0);

	for (int i = 0; i < allFrames->size(); i++) {
		nextFrame = allFrames->at(i);
		if (nextFrame->GetTimePosition() > AnimationTime) {
			break;
		}
		prevFrame = allFrames->at(i);
	}
	frames[0] = *prevFrame;
	frames[1] = *nextFrame;
	
}
float Animator::CalculateProgression(Keyframe prev, Keyframe next) {
	float Totaltime = next.GetTimePosition() - prev.GetTimePosition();
	float currentTime = AnimationTime - prev.GetTimePosition();
	return currentTime / Totaltime;
}

map<string, mat4x4> Animator::InterpolatePoses(Keyframe prevFrame, Keyframe nextFrame, float progression)
{
	map<string, mat4x4> currPose;
	map<string, JointTransform*>::iterator it;

	for (it = prevFrame.GetJointKeyFrames().begin(); it != prevFrame.GetJointKeyFrames().end(); it++)
	{
		JointTransform* prevTransform = prevFrame.GetJointKeyFrames()[it->first];
		JointTransform* nextTransform = nextFrame.GetJointKeyFrames()[it->first];
		JointTransform* currentTransform = &currentTransform->Interpolate(prevTransform, prevTransform, progression);
		
		prevFrame.GetJointKeyFrames()[it->first] = currentTransform;	
	}

	return currPose;
}

