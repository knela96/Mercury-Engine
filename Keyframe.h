#ifndef KEYFRAME_H_
#define KEYFRAME_H_

#include "Globals.h"
#include "Application.h"
#include "JointTransform.h"



class Keyframe
{
public:
	Keyframe() {};
	Keyframe(float TimePosition_, std::map<string, JointTransform> pose_) :TimePosition(TimePosition_), pose(pose_) {}; //WTF puc gualar una llista a un altre?
	~Keyframe() {};

public:
	std::map<string, JointTransform> pose;
	float            TimePosition;


private:


	

public:

	float GetTimePosition() { return TimePosition; }
	
	std::map<string, JointTransform> GetJointKeyFrames() { return pose; }

};




#endif