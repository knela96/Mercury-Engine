#ifndef KEYFRAME_H_
#define KEYFRAME_H_

#include "Globals.h"
#include "Application.h"
#include "JointTransform.h"

struct KeyInfo {
	string name;
	JointTransform jointTransform;
};

class Keyframe
{
public:
	Keyframe(float When_, list<KeyInfo*> KeyJoints_) :When(When_), KeyJoints(KeyJoints_) {}; //WTF puc gualar una llista a un altre?
	~Keyframe();

private:
	float      When;
	list<KeyInfo*>   KeyJoints;
	

public:

	float GetWhen() { return When; }
	


};




#endif