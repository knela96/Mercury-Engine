#ifndef JOINT_H_
#define JOINT_H_

#include "Globals.h"
#include "Application.h"



class Joint
{
public:
	Joint() {}
	Joint(int index_, string name_, mat4x4 LocalBindTransform_) :index(index_), name(name_), LocalBindTransform(LocalBindTransform_) {};
	~Joint();

public:

	int             index; //ID and also the order in the joints list
	string          name; //name that joint recieve from .dae file

	list<Joint*>    children; //joint's childrens

	mat4x4          AnimationTransform;

	mat4x4          LocalBindTransform; //joint's local transform based on parent's transform
	mat4x4          InverseBindTransform; //calculated using "LocalBindTransform" and is the bind transform of the jont in model space (inverted)

public:

	void AddChild(Joint Child) { this->children.push_back(&Child); }

	mat4x4 GetAnimationTransform() { return AnimationTransform; }

	mat4x4 GetInverseBindTransform() { return InverseBindTransform; }

	void SetAnimationTransform(mat4x4 AnimTransform) { AnimationTransform = AnimTransform; }

	void CalculateInverseBindTransform(mat4x4 ParentBindTransform); //this functon is called once, (n the joints set up)



};





#endif // !JOINT_H_

