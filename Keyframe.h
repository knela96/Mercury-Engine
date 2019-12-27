#ifndef KEYFRAME_H_
#define KEYFRAME_H_

#include "Globals.h"
#include "Application.h"
#include "JointTransform.h"

//struct KeyInfo {
//	string name;
//	JointTransform jointTransform;
//};

class Keyframe
{
public:
	Keyframe() {};
	Keyframe(float TimePosition_, std::map<string, JointTransform> pose_) :TimePosition(TimePosition_), pose(pose_) {}; //WTF puc gualar una llista a un altre?
	~Keyframe() {};

private:
	float            TimePosition;
	std::map<string, JointTransform> pose;

	map pos
		map rot

	/*list<KeyInfo*>   KeyJoints;*/
	

public:

	float GetTimePosition() { return TimePosition; }
	
	std::map<string, JointTransform> GetJointKeyFrames() { return pose; }

};




#endif